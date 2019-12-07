// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

int PORT = 1234;
#define MAXLINE 1024

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from client"; 
	struct sockaddr_in	 servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 


	int len = sizeof(servaddr);
	char *name = "Hashir\n";
	if(sendto(sockfd, (const char *)name, strlen(name), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		perror("SendTo failed"); 
		exit(EXIT_FAILURE); 
	}


	int n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &servaddr, &len);
	buffer[n] = '\0';
	printf("\nClient: %s\n", buffer);

	close(sockfd); 
	return 0; 
} 

