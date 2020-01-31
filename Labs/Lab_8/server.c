#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
#define MAXLINE 1024

int isFile(char *dir) {

	#define EXT_ARR_LEN 7
	char *fileExt[EXT_ARR_LEN] = {".txt", ".pdf", ".exe", ".doc", ".docx", ".xls", ".xlsx"};

	for(int i = 0; i < EXT_ARR_LEN; i++) {
		if(strstr(dir, fileExt[i]))
			return 1;
	}

	return 0;
}

// Driver function 
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
  	char line[MAXLINE] = {'\0'};
    int n;
   	n = read(connfd, line, MAXLINE); 
   	if (n < 0) {
        printf("read failed...\n"); 
        exit(0); 
    } 

    line[n] = '\0';
    printf("\nReceived %s", line);
  	
  	char delimeter = ' ';
  	char delimeter2 = ',';
    char* folder = strtok(line, &delimeter);
    
    mkdir(folder, 0777);
    char tempPath[100];
	    
    while(1) {

	    char* subDir = strtok(NULL, &delimeter2);

	    if(subDir == NULL)
	    	break;

	    memset(tempPath, 0, sizeof(tempPath));
		sprintf(tempPath, "%s/%s", folder, subDir);

	    if(!isFile(subDir)) {
		    mkdir(tempPath, 0777);
	    }
	    else {
	    	FILE* fptr;
			fptr = fopen(tempPath, "w");
			fclose(fptr);
	    }
	    
    }

    // After chatting close the socket 
    close(sockfd); 
} 
