// Server side implementation of UDP client-server model 
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
	char hello[MAXLINE] = "Message delivered"; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	{ 
		perror("Error: bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	
	listen(sockfd, 5);

	int len = sizeof(cliaddr);
	int newConnectionFD = accept(sockfd, (struct sockaddr *)&cliaddr, &len);	

	if(newConnectionFD < 0) {
		perror("Error: accept failed"); 
		exit(EXIT_FAILURE); 
	}

	int n;
	n = read(newConnectionFD, buffer, sizeof(buffer));
	if(n < 0) {
		perror("Error: reading"); 
		exit(EXIT_FAILURE); 
	}
	printf("Server: %s\n", buffer);

	n = write(newConnectionFD, hello, sizeof(hello));
	if(n < 0) {
		perror("Error: writing"); 
		exit(EXIT_FAILURE); 
	}

	return 0; 
} 

