#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <string>
#include <mutex>

#define SERVER_PORT 7777
#define SERVER_IP "127.0.0.1"
#define MAX_CLIENT_NUM 5
#define BUFFER 1024

using std::map;
using std::string;
using std::lock_guard;
using std::mutex;


// server's socket address
struct sockaddr_in server_socket = {
    server_socket.sin_family = AF_INET,
    server_socket.sin_port = htons(7777),
    server_socket.sin_addr.s_addr = inet_addr("127.0.0.1")
};
// server listen socket fd
int server_listen_socket_fd;
// server chat socket fd
int server_chat_socket_fd;
// logic_unit function
void logic_unit (int server_chat_socket_fd);
// online user list
map<string, int> online_users;
// global lock
mutex global_mtx;
// recv bytes
int recv_bytes;
// mem_reset
template <size_t N>
void mem_reset (char (&buf)[N]);
// broadcast_message
int broadcast_message ();