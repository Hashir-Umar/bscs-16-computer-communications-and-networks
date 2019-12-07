// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 8080 
#define MAXLINE 1024 

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char hello[30] = "Hello from client"; 
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
	
	// Binding socket 
	if ( connect(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) { 
		perror("connection failed"); 
		exit(EXIT_FAILURE); 
	} 

	int n;	
	n = write(sockfd, hello, sizeof(hello));
	if(n < 0) {
		printf("Error: writing to socket");
		exit(EXIT_FAILURE); 	
	}	

	
	bzero(buffer, MAXLINE);	
	n = read(sockfd, buffer, sizeof(buffer));
	if(n < 0) {
		printf("Error: writing to socket");	
		exit(EXIT_FAILURE); 
	}

	printf("Client: %s\n", buffer);

	close(sockfd); 
	return 0; 
} 

