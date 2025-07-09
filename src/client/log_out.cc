#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include "terminal_setting.hpp"

using std::cout;
using std::endl;

int log_out (int socket_fd) {
    if (shutdown(socket_fd ,SHUT_RDWR) == -1) {
        tty_set.tty_error();
        cout << "error: shutdown!" << strerror(errno) << endl;
        return -1;
    }
    return 0;
}