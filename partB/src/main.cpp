#include "tcp_server.h"

int main() {
    TCPServer server(9004, 1000);
    server.run();
    return 0;
}