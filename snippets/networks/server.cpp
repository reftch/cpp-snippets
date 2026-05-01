#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

namespace http {

    class server {
       public:
        server() : server("8080") {}
        server(const std::string& port) : port_(port) {}

        int start() {
            // start time
            auto start_time = std::chrono::high_resolution_clock::now();

            // create server socket.
            int server_socket_fd;            // file descriptior of server socket.
            struct sockaddr_in server_addr;  // structure for server address it helps to config and bind server socket.
            server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (server_socket_fd < 0) {
                std::cerr << "Failed to create server socket." << '\n';
                exit(1);
            }

            // config server socket.
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(std::stoi(this->port_));
            server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

            // bind  socket to port.
            if (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
                std::cerr << "Failed to bind server socket." << std::endl;
                exit(1);
            }

            // listens on socket.
            if (listen(server_socket_fd, 5) < 0) {
                std::cerr << "Failed to listen on server socket." << std::endl;
                exit(1);
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

            std::cout << "server listening on http://127.0.0.1:" << this->port_ << " in " << duration << '\n';

            return server_socket_fd;
        }

        int set_nonblocking(int fd) {
            int flags = fcntl(fd, F_GETFL, 0);
            if (flags == -1) return -1;
            if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) return -1;
            return 0;
        }

       private:
        std::string port_;
    };

}  // namespace http

static const char* okResp =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain; charset=utf-8\r\n"
    "Content-Length: 19\r\n"
    "Connection: keep-alive\r\n\r\n"
    "hello from pure c++";

int main() {
    http::server s;
    int s_fd = s.start();

    struct sockaddr_in client_addr;
    socklen_t client_addr_size;
    client_addr_size = sizeof(struct sockaddr_in);

    auto handle_client = [](int fd) {
        write(fd, okResp, strlen(okResp));
        close(fd);
        return fd;
    };

    constexpr int pool_size = 100;
    std::vector<std::future<int>> tasks;

    // "Async pool" of std::async
    for (int i = 1; i <= pool_size; ++i) {
        int client_socket_fd = accept(s_fd, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_socket_fd < 0) {
            std::cerr << "Failed to accept client request." << std::endl;
            exit(1);
        }

        tasks.push_back(std::async(std::launch::async, handle_client, client_socket_fd));
    }

    while (1) {
        for (int i = 1; i <= pool_size; ++i) {
            int client_socket_fd = accept(s_fd, (struct sockaddr*)&client_addr, &client_addr_size);
            if (client_socket_fd < 0) {
                std::cerr << "Failed to accept client request." << std::endl;
                exit(1);
            }

            if (s.set_nonblocking(client_socket_fd) < 0) {
                close(client_socket_fd);
                continue;
            }

            tasks.push_back(std::async(std::launch::async, handle_client, client_socket_fd));
        }

        tasks.clear();

        // handle client requests.
        // Accept system call .

        // int client_socket_fd = accept(s_fd, (struct sockaddr*)&client_addr, &client_addr_size);
        // if (client_socket_fd < 0) {
        //     std::cerr << "Failed to accept client request." << std::endl;
        //     exit(1);
        // }

        // write(client_socket_fd, okResp, strlen(okResp));
        // close(client_socket_fd);
        // Create a new thread to handle the client
        // std::thread clientThread(handleClient, client_socket_fd);
        // clientThread.detach();  // Detach the thread to allow it to run independently
    }

    return 0;
}