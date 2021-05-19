#include "../Inc/server_connection.h"

void server_start(char *name, char *password, int ftp_enabled){
	server_name = name;
	server_password = password;
	is_ftp_enabled = ftp_enabled;
	if(!fork()){
		char *tmp_dir[] = {"mkdir", "/tmp/RPi-Console", NULL};
		execvp(tmp_dir[0], tmp_dir);
		exit(0);
	}
	if(fopen("/tmp/RPi-Console/pid.dat", "r")){
		puts("Bir baska sunucu prosesi calisiyor. Yine de giris yapmak isterseniz programi -c flagi ile calistirin");
		exit(1);
	}
	tmp_file = fopen("/tmp/RPi-Console/pid.dat", "w");

	memset(clientfdmask, 0, MAX_CLIENTS * sizeof(int));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("Soket objesi oluşturulamadi");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(serv_addr.sin_zero), 0, 8);

	if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr)) == -1){
		perror("Bind islemi yapilamadi");
		exit(1);
	}

	if(listen(sockfd, BACKLOG) == -1){
		perror("Dinleme yapilamiyor");
		exit(1);
	}
	if(!fork()){
		fprintf(tmp_file, "%d\n", (int)getpid());
		server_accept(NULL);
	}
}

void* server_accept(void* params){
	while(1){
		struct sockaddr_in cl_addr;
		socklen_t sin_size = sizeof(struct sockaddr_in);
		int incomingfd = accept(sockfd, (struct sockaddr *) &cl_addr, &sin_size);
		if(incomingfd == -1){
			perror("Accept yapilamadi");
			continue;
		}
		for(int i = 0; i < MAX_CLIENTS; i++){
			if(!clientfdmask[i]){
				pthread_mutex_lock(&mutex);
				clientfdmask[i] = 1;
				clientfdlist[i] = incomingfd;//sinyal cak
				pthread_mutex_unlock(&mutex);
				long long int val = (long long int)incomingfd;
				val |= (long long int)i << 32;
				pthread_create(&threads[i], NULL, server_communicate, (void*)&val);
				break;
			}
		}
	}
	return NULL;
}

void* server_communicate(void* params){
	long long int val = *((long long int*)params);
	int clientfd = (int)(val & 0x7FFFFFFFL);
	int indice = (int)(val >> 32);
	uint8_t command_buffer[512];
	uint8_t output_buffer[4096];

	memset(command_buffer, 0, sizeof(command_buffer));
	memset(output_buffer, 0, sizeof(command_buffer));
	recv(clientfd, command_buffer, sizeof(command_buffer), 0); //recv username, passwd

	char *user_name = strtok(command_buffer, " ");
	char *user_password = strtok(NULL, "\0");

	if(!strcmp(user_name, server_name) && !strcmp(user_password, server_password)){
		output_buffer[0] = 1;
		if(is_ftp_enabled)
			output_buffer[1] = 1;
	}

	send(clientfd, output_buffer, sizeof(output_buffer), 0);

	if(strcmp(user_name, server_name) || strcmp(user_password, server_password)){

		close(clientfdmask[indice]);
		return NULL;
	}


	while(1){
		for(int i = 0; i < MAX_ARGS; i++){
			args[i] = NULL;
		}
		memset(output_buffer, 0, sizeof(output_buffer));
		recv(clientfd, command_buffer, sizeof(command_buffer), 0);//flag yok
		//printf("%s\n", command_buffer);
		char *command = strtok(command_buffer, " ");
		args[0] = command;
		if(!strcmp(command, "kill_console")){
			pthread_mutex_lock(&mutex); //mutex
			close(clientfdmask[indice]);
			clientfdmask[indice] = 0;
			pthread_mutex_unlock(&mutex);
			pthread_exit((void*)0); //finish thread
		}
		for(int i = 1; i < MAX_ARGS && args[i-1] != NULL; i++){
			char *arg = strtok(NULL, " ");
			args[i] = arg;
		}
		for(int i = 0; i < MAX_ARGS && args[i] != NULL; i++){
			for(int j = 0; args[i][j] != '\0'; j++){
				if(args[i][j] == '\n'){
					args[i][j] = '\0';
				}
			}
		}
		int pipes[2];
		pipe(pipes);

		if(!fork()){
			fprintf(tmp_file, "%d", (int)getpid());
			close(pipes[0]);
			dup2(pipes[1], 1);
			execvp(command, args);
			exit(-1);
		}
		else{
			close(pipes[1]);
			FILE *f = fdopen(pipes[0],"r");
			fread(output_buffer, sizeof(output_buffer), 1, f);
			close(pipes[0]);
			send(clientfd, output_buffer, sizeof(output_buffer), 0);
		}
	}
	return NULL;
}
