#include "client.hpp"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;
using std::flush;
using std::cin;

// send buf
char send_buf[BUFFER];

void message_handler (int socket_fd) {
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