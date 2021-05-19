#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define PORT 54306

int sockfd;
struct sockaddr_in cl_addr;
struct hostent *he;

void client_start();

int client_verify(char *user_id, char *user_password);

void client_communicate();

void kill_signal(int signo);
