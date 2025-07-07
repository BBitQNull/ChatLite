#include "server.hpp"
#include <iostream>

using std::cout;
using std::endl;

// send buf
char send_buf[BUFFER];

template <int N>
int broadcast_message (const char (&buf)[N]) {

    for (const auto& pair : online_users) {
        strcpy(send_buf, (pair.first + "-->" + buf).c_str());
        if (send(pair.second, &send_buf, sizeof(send_buf), 0) == -1) {
            cout << "error: send!" << strerror(errno) << endl;
            return -1;
        }
    }
    return 0;
}