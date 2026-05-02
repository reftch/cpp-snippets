#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

namespace http {

    struct client_details {
        int32_t clientfd;              // client file descriptor
        int32_t serverfd;              // server file descriptor
        std::vector<int> client_list;  // for storing all the client fd

        client_details(void) {  // initializing the variable
            this->clientfd = -1;
            this->serverfd = -1;
        }
    };

    // Structure to hold route information
    struct route_info {
        std::string method;
        std::string pattern;
        std::function<int(int)> handler;
    };

    class server {
       public:
        server(const std::string& host, const int port) : host_(host), port_(port) {}

        const int max_connections = 1024;

        void start() {
            auto start_time = std::chrono::high_resolution_clock::now();
            //
            std::unique_ptr<client_details> c_ptr = std::make_unique<client_details>();
            client = c_ptr.get();

            client->serverfd = socket(AF_INET, SOCK_STREAM, 0);  // for tcp connection
            // error handling
            if (client->serverfd <= 0) {
                std::cerr << "socket creation error\n";
                exit(1);
            }

            // setting serverFd to allow multiple connection
            int opt = 1;
            if (setsockopt(client->serverfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof opt) < 0) {
                std::cerr << "setSocketopt error\n";
                exit(2);
            }

            // setting the server address
            struct sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(port_);
            inet_pton(AF_INET, host_.data(), &serverAddr.sin_addr);

            // binding the server address
            if (bind(client->serverfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
                std::cerr << "bind error\n";
                exit(3);
            }

            // listening to the port
            if (listen(client->serverfd, max_connections) < 0) {
                std::cerr << "listen error\n";
                exit(4);
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

            std::cout << "server listening on http://" << host_ << ":" << port_ << " in " << duration << '\n';

            handle_requests();
        }

        void stop() {}

        void register_handler(const std::string& method, const std::string& path, std::function<int(int)> handler) {
            // Create route_info structure
            route_info info;
            info.method = method;
            info.pattern = path;
            info.handler = handler;

            routes_.push_back(info);
        }

       private:
        std::string host_;
        int port_;

        std::vector<struct pollfd> pollfds;
        client_details* client;

        // map for routes
        std::vector<route_info> routes_;

        void handle_requests() {
            // Vector to hold pollfd structures
            std::vector<struct pollfd> pollfds;

            while (true) {
                // Clear and rebuild pollfds vector
                pollfds.clear();

                // Add server socket
                struct pollfd server_pollfd;
                server_pollfd.fd = client->serverfd;
                server_pollfd.events = POLLIN;
                server_pollfd.revents = 0;
                pollfds.push_back(server_pollfd);

                // Add all client sockets
                for (auto sd : client->client_list) {
                    struct pollfd pfd;
                    pfd.fd = sd;
                    pfd.events = POLLIN;
                    pfd.revents = 0;
                    pollfds.push_back(pfd);
                }

                // Using poll for listening to multiple clients
                int activity = poll(pollfds.data(), pollfds.size(), -1);
                if (activity < 0) {
                    std::cerr << "poll error\n";
                    continue;
                }

                // Check for new connection on server socket
                if (pollfds[0].revents & POLLIN) {
                    client->clientfd = accept(client->serverfd, (struct sockaddr*)NULL, NULL);
                    if (client->clientfd < 0) {
                        std::cerr << "accept error\n";
                        continue;
                    }
                    // Set non-blocking mode for client socket
                    int flags = fcntl(client->clientfd, F_GETFL, 0);
                    fcntl(client->clientfd, F_SETFL, flags | O_NONBLOCK);

                    // adding client to list
                    client->client_list.push_back(client->clientfd);
                    // std::cout << "new client connected, fd: " << client->clientfd << std::endl;
                }

                // check for activity on client sockets, process each client socket
                for (size_t i = 0; i < client->client_list.size();) {
                    int pollfd_index = i + 1;  // +1 because server socket is at index 0

                    if (pollfd_index < pollfds.size() && (pollfds[pollfd_index].revents & POLLIN)) {
                        int sd = client->client_list[i];
                        char message[1024];
                        ssize_t valread = read(sd, message, sizeof(message) - 1);

                        if (valread > 0) {
                            message[valread] = '\0';  // Null terminate the message
                            // Handle message
                            auto route = routes_[0];
                            route.handler(sd);
                        } else if (valread == 0) {
                            // Client disconnected
                            close(sd);
                            // Remove from client list
                            client->client_list.erase(client->client_list.begin() + i);
                            continue;  // Don't increment i since we removed an element
                        } else {
                            // Error or would block (non-blocking socket)
                            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                                // std::cerr << "read error: " << strerror(errno) << '\n';
                            }
                            i++;  // Increment if no error
                        }
                    } else {
                        i++;  // Increment if no activity
                    }
                }
            }
        }
    };

}  // namespace http

http::server* server_ptr = nullptr;

static const char* okResp =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain; charset=utf-8\r\n"
    "Content-Length: 19\r\n"
    "Connection: keep-alive\r\n\r\n"
    "hello from pure c++";

int main() {
    // create server instance
    std::unique_ptr<http::server> s_ptr = std::make_unique<http::server>("127.0.0.1", 8080);
    server_ptr = s_ptr.get();

    auto home_handler = [](int fd) {
        if (write(fd, okResp, strlen(okResp)) == -1) {
            perror("error writing response body");
        }
        return fd;
    };

    server_ptr->register_handler("GET", "/", home_handler);
    server_ptr->start();

    return 0;
}
