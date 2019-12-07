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

int isLowerCase(char ch) {
	if(ch >= 'a' && ch <= 'z')
		return 1;
	return 0;
}

int isUpperCase(char ch) {
	if(ch >= 'A' && ch <= 'Z')
		return 1;
	return 0;
}

int isNumber(char ch) {
	if(ch >= '0' && ch <= '9')
		return 1;
	return 0;
}

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
	
	
	if(listen(sockfd, 5) < 0){
		perror("Error: listening failed"); 
		exit(EXIT_FAILURE); 
	}
	else {
		printf("Listening\n");
	}

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
	
	printf("\nServer: Rececived encrypted file content");

	for(int i = 0; i < strlen(buffer); i++) {
		if(isLowerCase(buffer[i])) {
			buffer[i] -= 3;
		}
		else if(isUpperCase(buffer[i])) {
			buffer[i] -= 2;
		}
		else if(isNumber(buffer[i])) {
			buffer[i] -= 1;
		}
	}
	
	printf("\nServer: Performed decryption on file content");

	n = write(newConnectionFD, buffer, sizeof(buffer));
	if(n < 0) {
		perror("Error: writing"); 
		exit(EXIT_FAILURE); 
	}

	printf("\nServer: sending back decrypted file content to client\n");

	return 0; 
} 

