#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 54306
#define BACKLOG 5
#define MAX_CLIENTS 5
#define MAX_ARGS 20

int sockfd;
struct sockaddr_in serv_addr;
int clientfdlist[MAX_CLIENTS], clientfdmask[MAX_CLIENTS];
char *args[20];
pthread_t threads[MAX_CLIENTS];
pthread_mutex_t mutex;
FILE *tmp_file;

void server_start();

void* server_accept(void*);

void* server_communicate(void*);
