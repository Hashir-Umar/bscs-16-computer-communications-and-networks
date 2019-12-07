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

struct ServerInformation {
	char *ip;
	int port;
	int file_descriptor;
};

void* serverThread(void* parameter) {
	char *ip = ((struct ServerInformation *) parameter)->ip;
	int port = ((struct ServerInformation *) parameter)->port;
	int file_descriptor = ((struct ServerInformation *) parameter)->file_descriptor;

	char port_str[30];
	sprintf(port_str, "%i", port);
	printf("\nClient IP Address: %s\n", ip);
	printf("Client Port: %s\n", port_str);
	printf("Client File Descriptor: %d\n", file_descriptor);

	if(write(file_descriptor, ip, MAXLINE) < 0) {
		perror("Write Error\n");
		exit(0);
	}

	if(write(file_descriptor, port_str, MAXLINE) < 0) {
		perror("Write Error\n");
		exit(0);
	}
	
	while(1) {

		char* listOfFiles = "\n\t1) tempfile1.txt \n\t2) tempfile2.txt \n\t3) tempfile3.txt";
		
		if(write(file_descriptor, listOfFiles, MAXLINE) < 0) {
			perror("Write Error\n");
			exit(0);
		}
		
		char buffer[MAXLINE];
		if(read(file_descriptor, buffer, MAXLINE) < 0) {
			perror("Read Error\n");
			exit(0);
		}
		
		printf("Client: File Name: %s\n", buffer);
		FILE *filePtr = fopen(buffer, "r");
		if(filePtr == NULL) {
			if(write(file_descriptor, "File not found.", MAXLINE) < 0) {
				perror("Write Error\n");
				exit(0);
			}
		}
		else {
			char fileContent[MAXLINE]; 
			while(fgets(fileContent, MAXLINE, filePtr) != NULL);
			fclose(filePtr);
			
			if(write(file_descriptor, fileContent, MAXLINE) < 0) {
				perror("Write Error\n");
				exit(0);
			}
			
			if(write(file_descriptor, "Do you want to download more files(Yes/No): ", MAXLINE) < 0) {
				perror("Write Error\n");
				exit(0);
			}

			//reading user choice Yes\No
			if(read(file_descriptor, buffer, MAXLINE) < 0) {
				perror("Read Error\n");
				exit(0);
			}

			if(strcmp(buffer, "Yes") == 0)
				printf("\nClient wants to download more files..\n");
			else 
				break;
		}
	}
	
	printf("Client left\n\n");
	close(file_descriptor);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

	signal(SIGTSTP, signal_handler);
	signal(SIGINT, signal_handler);

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

	bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(sockfd, 5);

	while(1){
		unsigned int len = sizeof(cliaddr);
		int newfd = accept(sockfd, (struct sockaddr *) &cliaddr, &len);

		//extracting client information from clientaddr struct 
		struct ServerInformation s_info;
		char *ip = inet_ntoa(cliaddr.sin_addr);
		s_info.ip = (char*) malloc((strlen(ip) + 1) * sizeof(char));
		strcpy(s_info.ip , ip);
		s_info.port = ntohs(cliaddr.sin_port);
		s_info.file_descriptor = newfd;

		//executing a thread for client-server communication
		pthread_t tid;
		if(pthread_create(&tid, NULL, serverThread, (void*) &s_info) != 0)  {
			perror("Error while creating thread\n");
			exit(0);
		}
	}
	
	return 0;
}
