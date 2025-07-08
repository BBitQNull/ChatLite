#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#define SERVER_PORT 7777
#define SERVER_IP "127.0.0.1"
#define BUFFER 1024
#define USERNAME_BUFFER 77

using std::string;

// server's socket address
struct sockaddr_in server_socket = {
    server_socket.sin_family = AF_INET,
    server_socket.sin_port = htons(SERVER_PORT),
    server_socket.sin_addr.s_addr = inet_addr(SERVER_IP)
};
// client listen socket fd
int client_listen_socket_fd;
// user's choose
int choose = 0;
// username
string username;
// username imput function
void username_input (int socket_fd, string &username);