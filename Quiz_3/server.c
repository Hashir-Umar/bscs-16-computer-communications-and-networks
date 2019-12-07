#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <semaphore.h>

int PORT = 1234;
#define MAXLINE 1024
#define MAXCLIENTS 100

struct ClientInfo {
	int port;
	char *ip;
};

void freeMemory(struct ClientInfo *clientInfo, int size) {
	for(int i = 0; i < size; i++)
		free(clientInfo[i].ip);
}

int main(int argc, char *argv[]){

    int size = 0;
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from server"; 
	struct sockaddr_in	 servaddr, cliaddr; 


	struct ClientInfo ci[MAXCLIENTS];
	int clientIndex = 0;

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

	if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("socket bind failed"); 
		exit(EXIT_FAILURE); 
	}

	memset(&buffer, 0, sizeof(buffer)); 
	unsigned int len;
	char tempPtr[MAXLINE];
	while(1) {
	
		int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
		buffer[n] = '\0';
		write(1, buffer, strlen(buffer));

		//maintaing the list of the clients
		ci[clientIndex].ip = (char *)malloc(100 * sizeof(char));
		strcpy(ci[clientIndex].ip, inet_ntoa(cliaddr.sin_addr));
		ci[clientIndex].port = ntohs(cliaddr.sin_port);
		clientIndex++;
		
		memset(&buffer, 0, sizeof(buffer)); 
		memset(&tempPtr, 0, sizeof(tempPtr)); 
		

		if(clientIndex == 1) {
			strcpy(buffer, "You are the first client.\n");
		}
		else {
			sprintf(buffer, "Server has communicated with the follwing ips and ports.\n");
		}

		for (int i = 0; i < clientIndex - 1; i++) {
			sprintf(tempPtr, "Ip: %s, Port: %d\n", ci[i].ip, ci[i].port);
			strcat(buffer, tempPtr);
		}

		if(sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len) == -1) {
			perror("SendTo failed");
			freeMemory(ci, clientIndex);
			exit(EXIT_FAILURE);
		}
	}

	freeMemory(ci, clientIndex);

	return 0;
}
