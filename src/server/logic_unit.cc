#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include "server.hpp"

using std::cout;
using std::endl;
using std::cin;

// recv buf
char recv_buf[BUFFER];

// username
string username;

void logic_unit (int server_chat_socket_fd) {
    // 接收对应客户端的用户名
    recv_bytes = recv(server_chat_socket_fd, &recv_buf, BUFFER, 0);
    if (recv_bytes == -1) {
        cout << "error: recv!" << strerror(errno) << endl;
    }
    // 维护在线成员列表
    recv_buf[recv_bytes] = '\0';
    username = string(recv_buf);
    {
        lock_guard<mutex> lock(global_mtx);
        online_users.insert({username, server_chat_socket_fd});
    }
    mem_reset(recv_buf);
    // 消息收发
    for (;;) {
        if (recv(server_chat_socket_fd, &recv_buf, BUFFER, 0) == -1) {
            cout << "error: recv!" << strerror(errno) << endl;
        }
        // 广播给所有的客户端
        if (broadcast_message() == -1) {
            cout << "error: broadcast!" << endl;
        }


    }
    

}