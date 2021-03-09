#include "../Inc/main.h"

int main(int argc, char *argv[]){
	pthread_t worker_thread;
	char c;
	char *user_type;
	if(argc < 2){
		perror("kullanim: -u <client|server>");
		return 1;
	}
	while((c = getopt(argc, argv, "p:u:p:k:c:")) != -1){
		switch(c){
			/*case 'p':
				if(port == -1){
					port = (short)atoi(optarg);
					printf("%d\n", port);
				}
				else{
					password = optarg;
					printf("%s\n", password);
				}
				break;*/

			case 'u':
				user_type = optarg;
				if(strcmp(user_type, "client") && strcmp(user_type, "server") && strcmp(user_type, "server_kill")){
					perror("kullanim: -u <client|server>");
					return 1;
				}
				if(!strcmp(user_type, "server_kill")){
					tmp_file = fopen("/tmp/RPi-Console/pid.dat", "r");
					if(tmp_file == NULL){
						perror("sunucu prosesi kapatilamiyor, lutfen islemi ps ve kill komutlarini kullanarak elle yapiniz");
						return 1;
					}
					int pid;
					char *kill_args[] = {"kill", "-9", NULL, NULL};
					char *del_args[] = {"rm", "-rf", "/tmp/RPi-Console", NULL};
					while(!feof(tmp_file)){
						fscanf(tmp_file, "%d", &pid);
						sprintf(kill_args[2],"%d",pid);
						execvp(kill_args[0], kill_args);
					}
					execvp(del_args[0], del_args);
				}
				break;

			case 'c':
				DUMMY(0);//c mutlu olsun diye
				char *rm[] = {"rm", "-rf", "/tmp/RPi-Console", NULL};
				execvp(rm[0], rm);
				break;

			case '?':
				perror("kullanim: -u <client|server>");
				return 1;

			default:
				perror("kullanim: -u <client|server>");
				return 1;
		}
	}

	if(!strcmp(user_type, "server")){
		printf("%s\n", user_type);
		server_start();
	}
	if(!strcmp(user_type, "client")){
		client_start("localhost");//deneme amacli local host diyorum
		client_communicate();
	}
}
