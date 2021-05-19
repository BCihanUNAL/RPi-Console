#include "../Inc/main.h"

int main(int argc, char *argv[]){
	pthread_t worker_thread;
	pthread_mutex_t kill_mutex;
	int is_ftp_enabled = 0;
	const char *err_msg_1 = "kullanim: -u username -p password -t <client|server> -o <ftp_server>";
	char c;

	if(argc < 2){
		perror(err_msg_1);
		return 1;
	}

	while((c = getopt(argc, argv, "u:p:o:a:t:k:c")) != -1){
		switch(c){
			case 'u':
				user_name = optarg;
				break;

			case 'p':
				user_password = optarg;
				break;

			case 't':
				user_type = optarg;
				if(strcmp(user_type, "client") && strcmp(user_type, "server") && strcmp(user_type, "server_kill") &&
						 !user_name && !user_password){
					perror(err_msg_1);
					return 1;
				}
				if(!strcmp(user_type, "server_kill")){
					tmp_file = fopen("/tmp/RPi-Console/pid.dat", "r");
					if(tmp_file == NULL){
						perror("sunucu prosesi kapatilamiyor, lutfen islemi ps ve kill komutlarini kullanarak elle yapiniz");
						return 1;
					}
					int pid;
					char *del_args[] = {"rm", "-rf", "/tmp/RPi-Console", NULL};
					while(!feof(tmp_file)){
						if(!fork()){
							char *kill_args[] = {"kill", "-9", NULL, NULL};
							kill_args[2] = (char*)malloc(10); //pid numarasi max 32 bit (9 hane) \0 icin + 1 byte eklendi
							fscanf(tmp_file, "%d", &pid);
							sprintf(kill_args[2],"%d",pid);
							execvp(kill_args[0], kill_args);
							exit(0);
						}
					}
					if(!fork()){
						execvp(del_args[0], del_args);
						exit(0);
					}
				}
				break;

			case 'a':
				server_addr = optarg;
				break;

			case 'c':
				DUMMY(0);//c mutlu olsun diye
				if(!fork()){
					char *rm[] = {"rm", "-rf", "/tmp/RPi-Console", NULL};
					execvp(rm[0], rm);
					exit(0);
				}
				break;

			case 'o':
				if(!strcmp(optarg, "ftp_server")){
					DUMMY(0);
					is_ftp_enabled = 1;
					if(!fork()){
						char *script_executable[] = {"chmod", "+x", "Scripts/ftp_server.sh", NULL};
						execvp(script_executable[0], script_executable);
						exit(0);
					}
					if(!fork()){
						char *start_ftp[] = {"bash", "Scripts/ftp_server.sh", NULL};
						execvp(start_ftp[0], start_ftp);
						exit(0);
					}
				}
				break;

			case '?':
				perror(err_msg_1);
				return 1;

			default:
				perror(err_msg_1);
				return 1;
		}
	}

	if(!strcmp(user_type, "server")){
		server_start(user_name, user_password, is_ftp_enabled);
	}

	if(!strcmp(user_type, "client")){
		if(!server_addr){
			perror(err_msg_1);
			return 1;
		}
		client_start(server_addr);//deneme amacli local host diyorum
		int ret_val = client_verify(user_name, user_password);
		if((ret_val & 1)){ // username password uyustu
			if((ret_val & 2)) // ftp sunucusu acik
				printf("ftp sunucusuna ftp://%s adresinden erisebilirsiniz", server_addr);
			client_communicate();
		}
	}

}
