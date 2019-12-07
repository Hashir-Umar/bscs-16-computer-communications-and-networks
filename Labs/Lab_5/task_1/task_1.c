#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void* printInfo(void* var)
{
	printf("\nName: Hashir Umar");
	printf("\nRegistration Number: L1F16BSCS0263\n");

	pthread_exit(NULL);
}

int main()
{
	pthread_t pthreadId;
	if(pthread_create(&pthreadId, NULL, printInfo, NULL) != 0) {
		perror("Error while creating thread\n");
		exit(0);
	}

	if(pthread_join(pthreadId, NULL) != 0) {
		perror("Error while joining thread\n");
		exit(0);
	}
	
	return 0;
}
