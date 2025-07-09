#include "client.hpp"
#include <iostream>
#include <string>
#include <thread>

using std::string;
using std::cout;
using std::endl;
using std::flush;
using std::cin;
using std::thread;

namespace {
    // send buf
    char send_buf[BUFFER];
    // recv buf
    char recv_buf[BUFFER];
    // recv_bytes
    int recv_bytes;
}

void recv_handler (int sock_fd) {
    for (;;) {
        recv_bytes = recv(sock_fd, recv_buf, sizeof(recv_buf), 0);
        if (recv_bytes == -1) {
            cout << "error: recv!" << strerror(errno) << endl;
        }
        cout << "---->" + string(recv_buf) << endl;
    }
}

void message_handler (int socket_fd) {
    thread recv_handler_thread(recv_handler, socket_fd);
    recv_handler_thread.detach();
    for (;;) {
        cout << "---->" << flush;
        cin.getline(send_buf, BUFFER);
        if (string(send_buf) == "QUIT") {
            return;
        }
        if (send(socket_fd, send_buf, sizeof(send_buf), 0) == -1 ) {
            cout << "error: send!" << strerror(errno) << endl;
        }
    }
}