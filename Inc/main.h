#include <stdio.h>
#include <pthread.h>
#include "client_connection.h"
#include "server_connection.h"
#define DUMMY(x) (x)

char *user_name = NULL;
char *user_password = NULL;
char *user_type = NULL;
char *server_addr = NULL;

int main(int argc, char *argv[]);
