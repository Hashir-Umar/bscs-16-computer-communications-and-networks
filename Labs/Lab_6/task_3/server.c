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

void signal_handler(int sig) {
	printf("\nTerminating server!!\n");
	exit(-1);
}

struct ClientInformation {
	char *ip;
	int port;
	int file_descriptor;

	int in_the_list;	//for exit condition
};

#define MAX_CLIENTS  20
struct ClientInformation c_info[MAX_CLIENTS];
int client_index = 0;

void sendAll(int port) {

	char buffer[MAXLINE];

	char port_str[50];

	memset(&buffer, 0, sizeof(buffer)); 
	sprintf(port_str, "New Connection with port %i has been arrived", port);
	strcpy(buffer, port_str);

	for(int i = 0; i < client_index; i++) {
		if(write(c_info[i].file_descriptor, buffer, MAXLINE) < 0) {
			perror("Write Error\n");
			exit(0);
		}	
	}

}

void store(char *ip, int port, int newfd) {
	
	c_info[client_index].ip = ip;
	c_info[client_index].port = port;
	c_info[client_index].file_descriptor = newfd;

	client_index++;
}

void* serverThread(void* parameter) {
	char *ip = ((struct ClientInformation *) parameter)->ip;
	int port = ((struct ClientInformation *) parameter)->port;
	int file_descriptor = ((struct ClientInformation *) parameter)->file_descriptor;

	char port_str[30];
	sprintf(port_str, "%i", port);
	printf("Client with port %s connected\n", port_str);

	char connectionMsg[50];
	sprintf(connectionMsg, "Connected with server port:  %i", PORT);
	if(write(file_descriptor, connectionMsg, MAXLINE) < 0) {
		perror("Write Error\n");
		exit(0);
	}	

	char buffer[MAXLINE];	
	while(1) {


		memset(&buffer, 0, sizeof(buffer)); 
		if(read(file_descriptor, buffer, MAXLINE) <= 0) {
			printf("Client with IP Address: %s and Port: %s left\n\n", ip, port_str);
			return NULL;
		}

		//telling other clients that this connection is going to close
		if(strcmp(buffer, "exit") == 0) {
		
			memset(&buffer, 0, sizeof(buffer)); 
			strcat(buffer, "Client with port ");
			strcat(buffer, port_str);
			strcat(buffer, " number has left");

			for(int i = 0; i < client_index; i++) {
				if(c_info[i].file_descriptor != file_descriptor && c_info[i].in_the_list == 1) {
					if(write(c_info[i].file_descriptor, buffer, MAXLINE) < 0) {
						perror("Write Error\n");
						exit(0);
					}	
				}

				if(c_info[i].file_descriptor == file_descriptor) {
					c_info[i].in_the_list = 0;
				}
			}

			break;
		}
		///////////////////////////////////////////////////////////////


		char tempBuffer[MAXLINE];
		sprintf(tempBuffer, "[from port %i]: ", port);
		strcat(tempBuffer, buffer);
		strcpy(buffer, tempBuffer);

		//uncomment this line to see all the messages send by the clients connected to this server
		// printf("\n%s\n", buffer);

		for(int i = 0; i < client_index; i++) {
			if(c_info[i].file_descriptor != file_descriptor && c_info[i].in_the_list == 1) {
				if(write(c_info[i].file_descriptor, buffer, MAXLINE) < 0) {
					perror("Write Error\n");
					exit(0);
				}	
			}
		}
	}
	
	printf("Client with IP Address: %s and Port: %s left\n\n", ip, port_str);
	close(file_descriptor);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

	signal(SIGTSTP, signal_handler);
	signal(SIGINT, signal_handler);

	for(int i = 0; i < MAX_CLIENTS; i++) {
		c_info[i].in_the_list = 1;
	}

	int sockfd; 
	struct sockaddr_in	 servaddr, cliaddr; 

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

	if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		perror("Error while binding\n");
		exit(0);
	}

	if(listen(sockfd, 5) < 0){
		perror("Error while liistening\n");
		exit(0);
	}

	while(1){
		unsigned int len = sizeof(cliaddr);
		int newfd = accept(sockfd, (struct sockaddr *) &cliaddr, &len);

		sendAll(ntohs(cliaddr.sin_port));
		store(inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), newfd);
		//extracting client information from clientaddr struct 
		

		//executing a thread for client-server communication
		pthread_t tid;
		if(pthread_create(&tid, NULL, serverThread, (void*) &c_info[client_index - 1]) != 0)  {
			perror("Error while creating thread\n");
			exit(0);
		}
	}
	
	return 0;
}
