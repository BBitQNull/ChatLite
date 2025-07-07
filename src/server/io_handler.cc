#include "server.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <errno.h>
#include <string>

using std::cout;
using std::endl;

void io_handler () {
    // 创建socket
    server_listen_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    // 命名socket
    const struct sockaddr* addr_ptr = reinterpret_cast<const struct sockaddr*>(&server_socket);
    if (bind(server_listen_socket_fd, addr_ptr, sizeof(server_socket)) == -1) {
        cout << "error: bind error!" << strerror(errno) << endl;
    }

}