#include "client.hpp"
#include <iostream>
#include <string>
#include <thread>
#include "terminal_setting.hpp"

using std::string;
using std::cout;
using std::endl;
using std::flush;
using std::cin;
using std::thread;
using std::mutex;

namespace {
    // send buf
    char send_buf[BUFFER];
    // recv buf
    char recv_buf[BUFFER];
    // recv_bytes
    int recv_bytes;
    // global lock
    mutex global_mtx;
}

void recv_handler (int sock_fd) {
    for (;;) {
        recv_bytes = recv(sock_fd, recv_buf, sizeof(recv_buf), 0);
        if (recv_bytes == -1) {
            tty_set.tty_error();
            cout << "error: recv!" << strerror(errno) << endl;
        }
        tty_set.tty_output();
        cout << string(recv_buf) << flush;
    }
}

void message_handler (int socket_fd) {
    thread recv_handler_thread(recv_handler, socket_fd);
    recv_handler_thread.detach();
    for (;;) {
        tty_set.tty_input();
        cin.getline(send_buf, BUFFER);
        if (string(send_buf) == "QUIT") {
            return;
        }
        if (send(socket_fd, send_buf, sizeof(send_buf), 0) == -1 ) {
            tty_set.tty_error();
            cout << "error: send!" << strerror(errno) << endl;
        }
    }
}