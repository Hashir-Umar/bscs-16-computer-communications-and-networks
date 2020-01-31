#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr

void sendLine(char line[], char *file_name) {
	
	FILE* fptr;
	fptr = fopen(file_name, "r");
	
	if (fptr != NULL) {
		fgets(line, 100, fptr);//receive data until end of file	    
		fclose(fptr);
	}
	else { 
		printf("Failed to open %s...\n", file_name); 
		exit(0); 
	} 
}

int main() 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 

    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    char line[100] = {'\0'};
    sendLine(line, "file.txt");
    if (write(sockfd, line, strlen(line)) < 0) { 
        printf("write failed...\n"); 
        exit(0); 
    } 

    // close the socket 
    close(sockfd); 
} 
