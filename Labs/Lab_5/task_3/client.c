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


	if(read(sockfd, buffer, MAXLINE) < 0) {
		perror("Read Error\n");
		exit(0);
	}

	printf("Server: Client IP: %s\n", buffer);

	if(read(sockfd, buffer, MAXLINE) < 0) {
		perror("Read Error\n");
		exit(0);
	}
	
	printf("Server: Client Port: %s\n", buffer);

	while(1) {	

		if(read(sockfd, buffer, MAXLINE) < 0) {
			perror("Read Error\n");
			exit(0);
		}


		printf("\nServer: Enter the name of the file (with .txt) you want to reterieve: %s\n", buffer);
		
		char filename[30];
		printf("\n\tFile Name: ");
		scanf("%s", filename);
		
		if(write(sockfd, filename, MAXLINE) < 0) {
			perror("Write Error\n");
			exit(0);
		}
		
		if(read(sockfd, buffer, MAXLINE) < 0) {
			perror("Read Error\n");
			exit(0);
		}
		
		if(strcmp(buffer, "File not found.\n") == 0) {
			printf("\n%s", buffer);
			continue;
		}
		else {
			printf("\nFile content recevied (copied to newContent.txt): %s", buffer);
			FILE *filePtrD = fopen("newContent.txt", "a");
			fprintf (filePtrD, "%s", buffer);
			fclose(filePtrD);
		}
		
		if(read(sockfd, buffer, MAXLINE) < 0) {
			perror("Read Error\n");
			exit(0);
		}
		
		printf("\n%s", buffer);

		char choice[5];
		scanf("%s", choice);

		if(strcmp(choice, "No") == 0) {
			if(write(sockfd, "No", MAXLINE) < 0) {
				perror("Write Error\n");
				exit(0);
			}
			close(sockfd); 
			return 0; 
		} else if(strcmp(choice, "Yes") == 0)  {
			if(write(sockfd, "Yes", MAXLINE) < 0) {
				perror("Write Error\n");
				exit(0);
			}
		}
	}
	
	close(sockfd); 
	return 0; 
} 

