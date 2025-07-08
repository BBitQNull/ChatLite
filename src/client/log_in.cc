#include "client.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

using std::cout;
using std::endl;

void log_in () {
    // 创建socket
    client_listen_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (client_listen_socket_fd == -1) {
        cout << "error: socket!" << strerror(errno) << endl;
    }


}