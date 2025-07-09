#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include "client.hpp"
#include <string>
#include "mem_reset.hpp"
#include "terminal_setting.hpp"

using std::cout;
using std::endl;
using std::string;

namespace {
    // recv buf
    char username_recv_buf[USERNAME_BUFFER];
    // send buf
    char username_send_buf[USERNAME_BUFFER];
}

void username_input (int socket_fd, string &username) {
    strcpy(username_send_buf, username.c_str());
    if (send(socket_fd, username_send_buf, sizeof(username_send_buf), 0) == -1) {
        tty_set.tty_error();
        cout << "error: send username!" << strerror(errno) << endl;
    }
    mem_reset(username_send_buf);
}