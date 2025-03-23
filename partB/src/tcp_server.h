#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <netinet/in.h>
#include <unordered_map>
#include <string>

#ifdef __linux__
    #include <sys/epoll.h>
#elif __APPLE__
    #include <sys/event.h>
#endif

class TCPServer {
private:
    int server_fd;
    struct sockaddr_in address;

#ifdef __linux__
    int epoll_fd;
#elif __APPLE__
    int kqueue_fd;
#endif

    std::unordered_map<std::string, std::string> database;  // Simple key-value store

    void add_to_event_loop(int fd);
    void accept_new_connection();
    void handle_client(int client_fd);
    std::string process_command(const std::string &request);

public:
    explicit TCPServer(int port);
    ~TCPServer();
    void run();
};

#endif
