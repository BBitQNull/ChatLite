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

// username imput function
extern void username_input (int socket_fd, string &username);
// log out function
extern int log_out (int socket_fd);
// message handler function
extern void message_handler (int socket_fd);
// login function
extern void log_in ();