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

	FILE *ptr;
	ptr = fopen("fileData.txt", "r");
	if(ptr == NULL) {
		perror("file cannot open\n"); 
		exit(EXIT_FAILURE); 
	}

	char fileContent[MAXLINE]; 
	while(fgets(fileContent, MAXLINE, ptr) != NULL);
	fclose(ptr);

	for(int i = 0; i < strlen(fileContent); i++) {
		if(isLowerCase(fileContent[i])) {
			fileContent[i] += 3;
		}
		else if(isUpperCase(fileContent[i])) {
			fileContent[i] += 2;
		}
		else if(isNumber(fileContent[i])) {
			fileContent[i] += 1;
		}
	}
	printf("\nClient: Performed Encryption");

	int n;	
	n = write(sockfd, fileContent, sizeof(fileContent));
	if(n < 0) {
		printf("Error: writing to socket");
		exit(EXIT_FAILURE); 	
	}	

	printf("\nClient: Sending file content after encryption: %s\n", fileContent);	

	bzero(buffer, MAXLINE);	
	n = read(sockfd, buffer, sizeof(buffer));
	if(n < 0) {
		printf("Error: writing to socket");	
		exit(EXIT_FAILURE); 
	}

	printf("Client: Rececived decrypted file content: %s\n", buffer);

	close(sockfd); 
	return 0; 
} 

