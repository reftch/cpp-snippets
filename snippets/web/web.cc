#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cctype>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

constexpr char RESPONSE_CLOSE[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain; charset=utf-8\r\n"
    "Content-Length: 14\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello, World!\n";

constexpr char RESPONSE_KEEPALIVE[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain; charset=utf-8\r\n"
    "Content-Length: 14\r\n"
    "Connection: keep-alive\r\n"
    "\r\n"
    "Hello, World!\n";

struct Connection {
    int sock = -1;
    std::vector<uint8_t> read_buf;
    std::string_view write_buf;
    size_t write_pos = 0;
    bool keep_alive = false;
};

bool set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return false;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1;
}

std::optional<size_t> find_header_end(const std::vector<uint8_t>& buf) {
    for (size_t i = 0; i + 4 <= buf.size(); ++i) {
        if (buf[i] == '\r' && buf[i + 1] == '\n' && buf[i + 2] == '\r' && buf[i + 3] == '\n') {
            return i + 4;
        }
    }
    for (size_t i = 0; i + 2 <= buf.size(); ++i) {
        if (buf[i] == '\n' && buf[i + 1] == '\n') {
            return i + 2;
        }
    }
    return std::nullopt;
}

std::string to_lowercase_bytes(const uint8_t* data, size_t len) {
    std::string s;
    s.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        s.push_back(static_cast<char>(std::tolower(data[i])));
    }
    return s;
}

bool parse_keep_alive(const std::vector<uint8_t>& headers) {
    std::string hdr = to_lowercase_bytes(headers.data(), headers.size());

    if (hdr.find("connection: close") != std::string::npos) {
        return false;
    }
    if (hdr.find("connection: keep-alive") != std::string::npos) {
        return true;
    }
    if (hdr.find("http/1.1") != std::string::npos) {
        return true;
    }
    return false;
}

void remove_connection(size_t index, std::vector<Connection>& conns, std::unordered_map<int, size_t>& fd_index,
                       std::vector<pollfd>& pollfds) {
    int fd = conns[index].sock;
    if (fd != -1) {
        close(fd);
    }

    size_t last_conn = conns.size() - 1;
    size_t last_poll = pollfds.size() - 1;
    size_t poll_index = index + 1;

    fd_index.erase(fd);

    if (index != last_conn) {
        std::swap(conns[index], conns[last_conn]);
        std::swap(pollfds[poll_index], pollfds[last_poll]);
        fd_index[conns[index].sock] = index;
    }

    conns.pop_back();
    pollfds.pop_back();
}

void worker_loop(int listener_fd) {
    if (!set_nonblocking(listener_fd)) {
        std::perror("set_nonblocking(listener)");
        return;
    }

    std::vector<Connection> conns;
    std::unordered_map<int, size_t> fd_index;
    std::vector<pollfd> pollfds;

    pollfds.push_back(pollfd{listener_fd, POLLIN, 0});

    while (true) {
        int ready = poll(pollfds.data(), pollfds.size(), 100);
        if (ready < 0) {
            if (errno == EINTR) {
                continue;
            }
            std::perror("poll");
            break;
        }

        if (ready == 0) {
            continue;
        }

        size_t current_poll_count = pollfds.size();
        for (size_t i = 0; i < current_poll_count && ready > 0; ++i) {
            if (pollfds[i].revents == 0) {
                continue;
            }

            --ready;

            if (i == 0) {
                while (true) {
                    sockaddr_in client_addr;
                    socklen_t addrlen = sizeof(client_addr);
                    int client_fd = accept(listener_fd, reinterpret_cast<sockaddr*>(&client_addr), &addrlen);
                    if (client_fd < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;
                        }
                        std::perror("accept");
                        break;
                    }

                    if (!set_nonblocking(client_fd)) {
                        std::perror("set_nonblocking(client)");
                        close(client_fd);
                        continue;
                    }

                    Connection conn;
                    conn.sock = client_fd;
                    conn.read_buf.reserve(1024);
                    conn.write_buf = std::string_view();
                    conn.write_pos = 0;
                    conn.keep_alive = false;

                    size_t index = conns.size();
                    conns.push_back(std::move(conn));
                    fd_index[client_fd] = index;
                    pollfds.push_back(pollfd{client_fd, POLLIN | POLLOUT, 0});
                }
            } else {
                size_t conn_index = i - 1;
                if (conn_index >= conns.size()) {
                    continue;
                }

                Connection& conn = conns[conn_index];
                bool done = false;

                if (pollfds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                    done = true;
                }

                if (!done && (pollfds[i].revents & POLLIN)) {
                    uint8_t buf[1024];
                    while (true) {
                        ssize_t n = read(conn.sock, buf, sizeof(buf));
                        if (n == 0) {
                            done = true;
                            break;
                        }
                        if (n < 0) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                                break;
                            }
                            std::perror("read");
                            done = true;
                            break;
                        }

                        conn.read_buf.insert(conn.read_buf.end(), buf, buf + n);
                        if (auto pos = find_header_end(conn.read_buf)) {
                            const auto headers =
                                std::vector<uint8_t>(conn.read_buf.begin(), conn.read_buf.begin() + *pos);

                            conn.keep_alive = parse_keep_alive(headers);
                            if (conn.keep_alive) {
                                conn.write_buf = RESPONSE_KEEPALIVE;
                            } else {
                                conn.write_buf = RESPONSE_CLOSE;
                            }
                            conn.write_pos = 0;
                            conn.read_buf.clear();
                            break;
                        }
                    }
                }

                if (!done && (pollfds[i].revents & POLLOUT) && !conn.write_buf.empty()) {
                    while (conn.write_pos < conn.write_buf.size()) {
                        ssize_t n = write(conn.sock, conn.write_buf.data() + conn.write_pos,
                                          conn.write_buf.size() - conn.write_pos);
                        if (n < 0) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                                break;
                            }
                            std::perror("write");
                            done = true;
                            break;
                        }
                        if (n == 0) {
                            done = true;
                            break;
                        }
                        conn.write_pos += static_cast<size_t>(n);
                    }

                    if (conn.write_pos >= conn.write_buf.size()) {
                        if (conn.keep_alive) {
                            conn.write_buf = std::string_view();
                            conn.write_pos = 0;
                        } else {
                            done = true;
                        }
                    }
                }

                if (done) {
                    remove_connection(conn_index, conns, fd_index, pollfds);
                }
            }
        }
    }
}

int create_listening_socket(const char* ip, uint16_t port, int backlog) {
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        std::perror("socket");
        return -1;
    }

    int opt = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::perror("setsockopt");
        close(listener);
        return -1;
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
        std::cerr << "invalid address\n";
        close(listener);
        return -1;
    }

    if (bind(listener, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::cerr << "bind failed: " << strerror(errno) << " (" << errno << ")\n";
        // std::perror("bind");
        close(listener);
        return -1;
    }

    if (listen(listener, backlog) < 0) {
        std::perror("listen");
        close(listener);
        return -1;
    }

    return listener;
}

int main() {
    const char* host = "0.0.0.0";
    uint16_t port = 8082;
    int workers = std::max(1u, std::thread::hardware_concurrency());

    int listener = create_listening_socket(host, port, 1024);
    if (listener < 0) {
        return 1;
    }

    std::cout << "Starting " << workers << " worker threads on http://" << host << ":" << port << "\n";

    std::vector<std::thread> threads;
    threads.reserve(workers);
    for (int i = 0; i < workers; ++i) {
        int worker_listener = dup(listener);
        if (worker_listener < 0) {
            std::perror("dup");
            break;
        }

        threads.emplace_back([worker_listener]() {
            worker_loop(worker_listener);
            close(worker_listener);
        });
    }

    close(listener);
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    return 0;
}