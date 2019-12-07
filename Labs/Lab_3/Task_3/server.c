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
	
	
	while(1) {
		if(listen(sockfd, 5) < 0){
			perror("Error: listening failed"); 
			exit(EXIT_FAILURE); 
		}
		else {
			printf("\nListening\n");
		}


		int len = sizeof(cliaddr);
		int newConnectionFD = accept(sockfd, (struct sockaddr *)&cliaddr, &len);	


		int portNumber;
		printf("\nServer: Port number = %d", ntohs(cliaddr.sin_port));

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
		
		printf("\nServer: filename %s\n", buffer); 

		char files[2][20] = {"file.txt", "file1.txt"};

		int isFileFound = -1;
		for (int i = 0; i < 2; i++) {
			if(strcmp(buffer, files[i]) == 0) {
				isFileFound = i;	
				break;			
			}	
		}

		if(isFileFound == -1) {
			printf("Server: %s not found \n", buffer);
			char fileNotFound[20] = "File not found";
			n = write(newConnectionFD, fileNotFound, sizeof(fileNotFound));
			if(n < 0) {
				perror("Server Error: writing"); 
				exit(EXIT_FAILURE); 
			}
		}
		else {
			FILE *ptr;
			ptr = fopen(files[isFileFound], "r");
			if(ptr == NULL) {
				perror("file not found\n"); 
				exit(EXIT_FAILURE); 
			}
			else {
				printf("Server: %s found and sending back to client\n", buffer);
				char fileContent[MAXLINE]; 
				while(fgets(fileContent, MAXLINE, ptr) != NULL);
				fclose(ptr);
				n = write(newConnectionFD, fileContent, sizeof(fileContent));
				if(n < 0) {
					perror("Server Error: writing"); 
					exit(EXIT_FAILURE); 
				}
			}
		}
	}
	return 0; 
} 

