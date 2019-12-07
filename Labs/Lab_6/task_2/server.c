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


int PORT = 1234;
#define MAXLINE 1024

void signal_handler(int sig) {
	printf("\nTerminating server!!\n");
	exit(-1);
}

struct ClientInformation {
	char *ip;
	int port;
	int file_descriptor;
};

void* serverThread(void* parameter) {
	char *ip = ((struct ClientInformation *) parameter)->ip;
	int port = ((struct ClientInformation *) parameter)->port;
	int file_descriptor = ((struct ClientInformation *) parameter)->file_descriptor;

	char port_str[30];
	sprintf(port_str, "%i", port);
	printf("\nNew Client Connected\n");
	printf("Client IP Address: %s\n", ip);
	printf("Client Port: %s\n", port_str);
	printf("Client File Descriptor: %d\n", file_descriptor);


	if(write(file_descriptor, "Hello from Server\n", MAXLINE) < 0) {
		perror("Write Error\n");
		exit(0);
	}	

	while(1) {

		char buffer[MAXLINE];
		if(read(file_descriptor, buffer, MAXLINE) <= 0) {	// = comparision for error handling 
			printf("Client with IP Address: %s and Port: %s left\n\n", ip, port_str);
			pthread_exit(NULL);
		}

		if(strcmp(buffer, "exit") == 0)
			break;
		
		printf("%s\n", buffer);
	}
	
	printf("Client with IP Address: %s and Port: %s left\n\n", ip, port_str);
	close(file_descriptor);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

	signal(SIGTSTP, signal_handler);
	signal(SIGINT, signal_handler);

	int sockfd; 
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

	if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		perror("Error while binding\n");
		exit(0);
	}

	if(listen(sockfd, 5)) {
		perror("Error while listening\n");
		exit(0);
	}

	while(1){
		unsigned int len = sizeof(cliaddr);
		int newfd = accept(sockfd, (struct sockaddr *) &cliaddr, &len);

		//extracting client information from clientaddr struct 
		
		struct ClientInformation s_info;
		char *ip = inet_ntoa(cliaddr.sin_addr);
		s_info.ip = (char*) malloc((strlen(ip) + 1) * sizeof(char));
		strcpy(s_info.ip , ip);
		s_info.port = ntohs(cliaddr.sin_port);
		s_info.file_descriptor = newfd;

		//executing a thread for client-server communication
		pthread_t tid;
		if(pthread_create(&tid, NULL, serverThread, (void*) &s_info) != 0)  {
			perror("Error while creating thread\n");
			exit(0);
		}
	}
	
	return 0;
}
