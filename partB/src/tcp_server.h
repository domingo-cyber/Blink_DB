#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <netinet/in.h>
#include <string>
#include "storage_engine.h"

#ifdef __linux__
    #include <sys/epoll.h>
#elif __APPLE__
    #include <sys/event.h>
#endif

class TCPServer {
private:
    int server_fd;
    struct sockaddr_in address;
    StorageEngine storage;

#ifdef __linux__
    int epoll_fd;
#elif __APPLE__
    int kqueue_fd;
#endif

    void add_to_event_loop(int fd);
    void accept_new_connection();
    void handle_client(int client_fd);
    std::string process_command(const std::string &request);

public:
    explicit TCPServer(int port, size_t storage_capacity = 1000);
    ~TCPServer();
    void run();
};

#endif