#include "server.hpp"
#include <iostream>

using std::cout;
using std::endl;

// send buf
char send_buf[BUFFER];

int broadcast_message () {
    for (const auto& pair : online_users) {
        if (send(pair.second, &send_buf, sizeof(send_buf), 0) == -1) {
            cout << "error: send!" << strerror(errno) << endl;
            return -1;
        }
    }
    return 0;
}