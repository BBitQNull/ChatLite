#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

using std::cout;
using std::endl;

int log_out (int socket_fd) {
    if (shutdown(socket_fd ,SHUT_RDWR) == -1) {
        cout << "error: shutdown!" << strerror(errno) << endl;
        return -1;
    }
    return 0;
}