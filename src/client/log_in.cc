#include "client.hpp"
#include "terminal_setting.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <string>


using std::cout;
using std::endl;
using std::cin;
using std::flush;

namespace {
    // recv buf
    char recv_buf[BUFFER];
    // send buf
    char send_buf[BUFFER];
    // client listen socket fd
    int client_listen_socket_fd;
    // server's socket address
    struct sockaddr_in server_socket = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT),
        .sin_addr.s_addr = inet_addr(SERVER_IP)
    };
    // username
    string username;
    // user's choose
    int choose = 0;
}
NcursesUI ui;

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
    "Welcome to ChatLite!\n1:Login with a username\n2:Enter '2' to Logout\nEnter 'QUIT' to Logout\n";
    cout << "-------------------------------------" << endl;
    cout << "-------Welcome to ChatLite!----------" << endl;
    cout << "-------1:Login with a username-------" << endl;
    cout << "-------2:Enter '2' to Logout---------" << endl;
    cout << "-------Enter 'QUIT' to Logout--------" << endl;
    cout << "-->" << flush;
    cin >> choose;
    switch (choose) {
        case 1 :
            cout << "Please input username:" << endl;
            cout << "-->" << flush;
            cin >> username;
            username_input(client_listen_socket_fd, username);
            // 消息收发
            message_handler (client_listen_socket_fd);
            break;
        case 2 :
            if (log_out(client_listen_socket_fd) == -1) {
                return;
            }
            cout << "Expect to meet you next!" << endl;
            break;
        default :
            cout << "error: Login!" << endl;
            break;
    }
}