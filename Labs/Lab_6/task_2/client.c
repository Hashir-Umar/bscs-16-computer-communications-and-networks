// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <signal.h>
#include <pthread.h>


int PORT = 1234;
#define MAXLINE 1024


void* readThread(void* parameter) {
	int sockfd = *((int*) parameter);
	char buffer[MAXLINE]; 

	while(1) {

		memset(&buffer, 0, sizeof(buffer)); 
		if(read(sockfd, buffer, MAXLINE) < 0) {
			return NULL;
		}
		
		//checking weather a buffer is empty or not
		if(strlen(buffer) > 0)	
			printf("Server: %s\n", buffer);
	}
}

void* writeThread(void* parameter) {
	int sockfd = *((int*) parameter);
	char buffer[MAXLINE]; 

	while(1) {
		
		scanf("%[^\t\n]", buffer);
		getchar();
		if(write(sockfd, buffer, MAXLINE) < 0) {
			perror("Read Error\n");
			exit(0);
		}


		if(strcmp(buffer, "exit") == 0)
			break;
	}

	close(sockfd);
}

// Driver code 
int main() { 

	int sockfd; 
	struct sockaddr_in	 servaddr; 

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
	

	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	//executing a thread for writing buffers
	pthread_t tid1;
	if(pthread_create(&tid1, NULL, writeThread, (void*) &sockfd) != 0)  {
		perror("Error while creating writeThread\n");
		exit(0);
	}

	//executing a thread for reading buffers
	pthread_t tid2;
	if(pthread_create(&tid2, NULL, readThread, (void*) &sockfd) != 0)  {
		perror("Error while creating readThread\n");
		exit(0);
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0; 
} 

