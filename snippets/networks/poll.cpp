#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

struct clientDetails {
    int32_t clientfd;             // client file descriptor
    int32_t serverfd;             // server file descriptor
    std::vector<int> clientList;  // for storing all the client fd
    clientDetails(void) {         // initializing the variable
        this->clientfd = -1;
        this->serverfd = -1;
    }
};

const int port = 8080;
// const char ip[] = "127.0.0.1";  // for local host
const char ip[] = "0.0.0.0";  // for allowing all incomming connection from internet
const int backlog = 1024;     // maximum number of connection allowed

static const char* okResp =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain; charset=utf-8\r\n"
    "Content-Length: 19\r\n"
    "Connection: keep-alive\r\n\r\n"
    "hello from pure c++";

int main() {
    auto handle_message = [](int fd) {
        write(fd, okResp, strlen(okResp));
        return fd;
    };

    auto client = new clientDetails();

    client->serverfd = socket(AF_INET, SOCK_STREAM, 0);  // for tcp connection
    // error handling
    if (client->serverfd <= 0) {
        std::cerr << "socket creation error\n";
        delete client;
        exit(1);
    } else {
        std::cout << "socket created\n";
    }

    // setting serverFd to allow multiple connection
    int opt = 1;
    if (setsockopt(client->serverfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof opt) < 0) {
        std::cerr << "setSocketopt error\n";
        delete client;
        exit(2);
    }

    // setting the server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    // binding the server address
    if (bind(client->serverfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "bind error\n";
        delete client;
        exit(3);
    } else {
        std::cout << "server binded\n";
    }

    // listening to the port
    if (listen(client->serverfd, backlog) < 0) {
        std::cerr << "listen error\n";
        delete client;
        exit(4);
    } else {
        std::cout << "server is listening\n";
    }

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
        for (auto sd : client->clientList) {
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
            client->clientList.push_back(client->clientfd);
            // std::cout << "new client connected, fd: " << client->clientfd << std::endl;
        }

        // check for activity on client sockets
        // process each client socket
        for (size_t i = 0; i < client->clientList.size();) {
            int pollfd_index = i + 1;  // +1 because server socket is at index 0

            if (pollfd_index < pollfds.size() && (pollfds[pollfd_index].revents & POLLIN)) {
                int sd = client->clientList[i];
                char message[1024];
                ssize_t valread = read(sd, message, sizeof(message) - 1);

                if (valread > 0) {
                    message[valread] = '\0';  // Null terminate the message
                    // Handle message
                    if (write(sd, okResp, strlen(okResp)) == -1) {
                        perror("error writing response body");
                    }
                } else if (valread == 0) {
                    // Client disconnected
                    // std::cout << "client disconnected\n";
                    close(sd);
                    // Remove from client list
                    client->clientList.erase(client->clientList.begin() + i);
                    continue;  // Don't increment i since we removed an element
                } else {
                    // Error or would block (non-blocking socket)
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        std::cerr << "read error: " << strerror(errno) << std::endl;
                    }
                    i++;  // Increment if no error
                }
            } else {
                i++;  // Increment if no activity
            }
        }
    }

    delete client;
    return 0;
}