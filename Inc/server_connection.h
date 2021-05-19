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
int is_ftp_enabled;
struct sockaddr_in serv_addr;
int clientfdlist[MAX_CLIENTS];
int clientfdmask[MAX_CLIENTS];
char *args[20];
char *server_name;
char *server_password;
pthread_t threads[MAX_CLIENTS];
pthread_mutex_t mutex;
FILE *tmp_file;

void server_start(char *name, char *password, int ftp_enabled);

int server_validate();

void* server_accept(void*);

void* server_communicate(void*);
