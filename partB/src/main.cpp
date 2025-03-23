#include "tcp_server.h"

int main() {
    TCPServer server(9002);  // Server on port 9002
    server.run();
    return 0;
}
