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
	signal(SIGINT, kill_signal);
	signal(SIGQUIT, kill_signal);
	signal(SIGABRT, kill_signal);
}

void client_communicate(){
	while(1){
			char output_buffer[512];
			char input_buffer[4096];
			char *line;
			int line_len;
			getline(&line, &line_len, stdin);
			memcpy(output_buffer, line, line_len);
			output_buffer[line_len] = '\0';
			send(sockfd, output_buffer, sizeof(output_buffer), 0);
			recv(sockfd, input_buffer, sizeof(input_buffer), 0);
			printf("%s\n", input_buffer);
	}
}

void kill_signal(int signo){
	char output_buffer[512];
	char kill_msg[] = "kill_console";
	memcpy(output_buffer, kill_msg, strlen(kill_msg));
	send(sockfd, output_buffer, sizeof(output_buffer), 0);
	exit(0);
}
