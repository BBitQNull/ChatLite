#include "client.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <string>


using std::cout;
using std::endl;
using std::cin;
using std::flush;

// recv buf
char recv_buf[BUFFER];
// send buf
char send_buf[BUFFER];

void log_in () {
    // 创建socket
    client_listen_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (client_listen_socket_fd == -1) {
        cout << "error: socket!" << strerror(errno) << endl;
    }
    // connect
    const struct sockaddr* srv_addr_ptr = reinterpret_cast<const struct sockaddr*>(&server_socket);
    if (connect(client_listen_socket_fd, srv_addr_ptr, sizeof(server_socket)) == -1) {
        cout << "error: connect!" << strerror(errno) << endl;
    }
    // 欢迎界面
    cout << "-------------------------------------" << endl;
    cout << "-------Welcome to ChatLite!----------" << endl;
    cout << "-------1:Login with a username-------" << endl;
    cout << "-------2:Enter 'QUIT' to Logout------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "------->" << flush;
    cin >> choose;
    switch (choose) {
        case 1 :
            cout << "Please input username:" << endl;
            cout << "------->" << flush;
            cin >> username;
            username_input(client_listen_socket_fd, username);
            break;
    }
}