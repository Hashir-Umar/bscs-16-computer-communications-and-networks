#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <semaphore.h>

void signal_handler(int sig) {
	printf("\nTerminating server!!\n");
	exit(-1);
}

int PORT = 1234;
#define MAXLINE 1024
int main(int argc, char *argv[]) {

	signal(SIGTSTP, signal_handler);
	signal(SIGINT, signal_handler);

    int size = 0;
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Your msg has been deliverd"; 
	struct sockaddr_in	 servaddr, cliaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 

	bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(sockfd, 5);

	while(1){
		int len = sizeof(cliaddr);
		int newfd = accept(sockfd, (struct sockaddr *) &cliaddr, &len);
		//add code here
		
		if(!fork()) {
			//closeing listening socket descriptor
			close(sockfd);
			int n = read(newfd, buffer, MAXLINE);

			if(n < 0) {
				perror("Read Error\n");
				exit(0);
			}

			buffer[strlen(buffer)] = '\0';

			int clientPortNumber = ntohs(cliaddr.sin_port);
			printf("Client with port number: %d sent \"%s\" Concurrent Server using fork\n", clientPortNumber, buffer);

			if(write(newfd, hello, strlen(hello) + 1) < 0) {
				perror("Write Error\n");
				exit(0);
			}

			return 0;
		}
		else {
			close(newfd);
		}

	}
	
	return 0;
}
