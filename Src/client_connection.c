#include "../Inc/client_connection.h"

void client_start(char *host_name){
	he = gethostbyname(host_name);
	if(he == NULL){
		perror("Sunucu adresi yanlis");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("Soket objesi oluşturulamadi");
		exit(1);
	}
	cl_addr.sin_family = AF_INET;
	cl_addr.sin_port = htons(PORT);
	cl_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(&(cl_addr.sin_zero), '\0', 8);

	if(connect(sockfd, (struct sockaddr *)&cl_addr, sizeof(struct sockaddr)) == -1){
		perror("Sunucuya baglanilamadi");
		exit(1);
	}
	signal(SIGINT, client_kill_signal);
	signal(SIGQUIT, client_kill_signal);
	signal(SIGABRT, client_kill_signal);
}

int client_verify(char *user_id, char *user_password){
	char output_buffer[512];
	char input_buffer[4096];
	char space = ' ';

	memcpy(output_buffer, user_id, strlen(user_id)); //send username & passwd
	memcpy(output_buffer + strlen(user_id), &space, sizeof(space));
	memcpy(output_buffer + strlen(user_id) + sizeof(space), user_password, strlen(user_password));

	send(sockfd, output_buffer, sizeof(output_buffer), 0);
	recv(sockfd, input_buffer, sizeof(input_buffer), 0);

	return input_buffer[0] | (2 * input_buffer[1]); //return result
}

void client_communicate(){
	char output_buffer[512];
	char input_buffer[4096];
	while(1){
			char *line = NULL;
			int line_len;
			getline(&line, &line_len, stdin);
			memcpy(output_buffer, line, line_len);
			free(line);
			output_buffer[line_len] = '\0';
			send(sockfd, output_buffer, sizeof(output_buffer), 0);
			recv(sockfd, input_buffer, sizeof(input_buffer), 0);
			if(!input_buffer[0]){ //server kapandi ise 0 gonderecek
				close(sockfd);
				exit(3);
			}
			printf("%s\n", input_buffer);
	}
}

void client_kill_signal(int signo){
	char output_buffer[512];
	char kill_msg[] = "kill_console";
	memcpy(output_buffer, kill_msg, strlen(kill_msg));
	send(sockfd, output_buffer, sizeof(output_buffer), 0);
	close(sockfd);
	exit(0);
}
