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



int PORT = 1234;
#define MAXLINE 1024

// Driver code 
int main() { 

	int sockfd; 
	char buffer[MAXLINE]; 
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

	char *hello = "Hello from client"; 
	//do send recv here
	if(write(sockfd, hello, strlen(hello) + 1) < 0) {
		perror("Write Error\n");
		exit(0);
	}

	if(read(sockfd, buffer, MAXLINE) < 0) {
		perror("Read Error\n");
		exit(0);
	}

	buffer[strlen(buffer)] = '\0';
	int serverPortNumber = ntohs(servaddr.sin_port);
	printf("From Server: %s to server with port number: %d\n", buffer, serverPortNumber);

	close(sockfd); 
	return 0; 
} 

