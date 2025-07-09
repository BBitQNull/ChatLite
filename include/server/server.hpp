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

// logic_unit function
extern void logic_unit (int server_chat_socket_fd);
// broadcast_message
extern int broadcast_message (const char (&buf)[BUFFER]);
// io_handler
extern void io_handler ();
// online user list
extern map<string, int> online_users;