#include "tcp_server.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <vector>
#include <algorithm>

#ifdef __linux__
    #include <sys/epoll.h>
#elif __APPLE__
    #include <sys/event.h>
#endif

#define MAX_EVENTS 1024

TCPServer::TCPServer(int port, size_t storage_capacity) : storage(storage_capacity) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << std::endl;

#ifdef __linux__
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Epoll creation failed");
        exit(EXIT_FAILURE);
    }
#elif __APPLE__
    kqueue_fd = kqueue();
    if (kqueue_fd == -1) {
        perror("kqueue creation failed");
        exit(EXIT_FAILURE);
    }
#endif

    add_to_event_loop(server_fd);
}

void TCPServer::add_to_event_loop(int fd) {
#ifdef __linux__
    epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
#elif __APPLE__
    struct kevent event;
    EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    kevent(kqueue_fd, &event, 1, NULL, 0, NULL);
#endif
}

void TCPServer::run() {
#ifdef __linux__
    epoll_event events[MAX_EVENTS];
#elif __APPLE__
    struct kevent events[MAX_EVENTS];
#endif

    while (true) {
#ifdef __linux__
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
#elif __APPLE__
        int num_events = kevent(kqueue_fd, NULL, 0, events, MAX_EVENTS, NULL);
#endif
        if (num_events == -1) {
            perror("Error in event loop");
            continue;
        }

        for (int i = 0; i < num_events; i++) {
#ifdef __linux__
            int fd = events[i].data.fd;
#elif __APPLE__
            int fd = (int)events[i].ident;
#endif
            if (fd == server_fd) {
                accept_new_connection();
            } else {
                handle_client(fd);
            }
        }
    }
}

void TCPServer::handle_client(int client_fd) {
    char buffer[1024];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0) {
        close(client_fd);
        return;
    }

    buffer[bytes_read] = '\0';
    std::string request(buffer);
    std::string response = process_command(request);

    send(client_fd, response.c_str(), response.size(), 0);
}

std::string TCPServer::process_command(const std::string &request) {
    std::vector<std::string> tokens;
    size_t pos = 0, next;

    // Split request into tokens
    while ((next = request.find("\r\n", pos)) != std::string::npos) {
        tokens.push_back(request.substr(pos, next - pos));
        pos = next + 2;
    }

    if (tokens.empty() || tokens[0].empty() || tokens[0][0] != '*')
        return "-ERR Invalid RESP request\r\n";

    int num_args = std::stoi(tokens[0].substr(1));
    if (static_cast<int>(tokens.size()) < num_args * 2 + 1)
        return "-ERR Malformed request\r\n";

    std::string command = tokens[2];
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    if (command == "SET" && num_args >= 3) {
        std::string key = tokens[4];
        std::string value = tokens[6];
        storage.set(key, value);  // Use the storage engine
        return "+OK\r\n";
    }

    if (command == "GET" && num_args >= 2) {
        std::string key = tokens[4];
        std::string value = storage.get(key);
        if (value == "NULL") return "$-1\r\n";  // Key not found
        return "$" + std::to_string(value.size()) + "\r\n" + value + "\r\n";
    }

    if (command == "DEL" && num_args >= 2) {
        std::string key = tokens[4];
        storage.del(key);
        return ":1\r\n";  // Redis returns 1 for successful delete
    }

    if (command == "PING") return "+PONG\r\n";

    if (command == "INFO") return "$5\r\nredis\r\n";

    if (command == "CONFIG") return "$-1\r\n";  // Fake response to prevent errors

    return "-ERR Unknown command\r\n";
}
void TCPServer::accept_new_connection() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

    if (client_fd == -1) {
        perror("Accept failed");
        return;
    }

    fcntl(client_fd, F_SETFL, O_NONBLOCK);
    add_to_event_loop(client_fd);
}

TCPServer::~TCPServer() {
    close(server_fd);
#ifdef __linux__
    close(epoll_fd);
#elif __APPLE__
    close(kqueue_fd);
#endif
}