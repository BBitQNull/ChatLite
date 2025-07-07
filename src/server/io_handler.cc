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
    // 监听socket
    if (listen(server_listen_socket_fd, MAX_CLIENT_NUM) == -1) {
        cout << "error: listen error!" << strerror(errno) << endl;
    }
    // accept阻塞
    for (;;) {
        server_chat_socket_fd = accept(server_listen_socket_fd, NULL, NULL);
        logic_unit(server_chat_socket_fd);
    }
}