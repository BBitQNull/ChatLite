#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <algorithm>
#include "server.hpp"
#include "mem_reset.hpp"

using std::cout;
using std::endl;
using std::cin;
using std::copy;

namespace {
    // recv buf
    char recv_buf[BUFFER];
    // send buf
    char send_buf[BUFFER];
    // username
    string username;
    // global lock
    mutex global_mtx;
    // recv bytes
    int recv_bytes;
}
map<string, int> online_users;

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
        recv_bytes = recv(server_chat_socket_fd, &recv_buf, BUFFER, 0);
        if (recv_bytes == -1) {
            cout << "error: recv!" << strerror(errno) << endl;
        }
        recv_buf[recv_bytes] = '\0';
        copy(recv_buf, recv_buf + BUFFER, send_buf);
        // 广播给所有的客户端
        if (broadcast_message(send_buf) == -1) {
            cout << "error: broadcast!" << endl;
        }
        // 关闭连接
        if (recv_bytes == 0) {
            shutdown(server_chat_socket_fd, SHUT_RDWR);
            {
                lock_guard<mutex> lock(global_mtx);
                if (online_users.erase(username) == 0) {
                    cout << "error: Delete user of log out failed!" << endl;
                }
            }
            return;
        }
    }
}