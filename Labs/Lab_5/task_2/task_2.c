#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>

struct Info {
	char *name;
	char *regNumber;
};

void init(struct Info *info, const char* _name, const char* _regNumber) {
	info->name = (char*) malloc((strlen(_name) + 1) * sizeof(char));
	info->regNumber = (char*) malloc((strlen(_regNumber) + 1) * sizeof(char));
	
	strcpy(info->name, _name);
	strcpy(info->regNumber, _regNumber);
}

void* printInfo(void* parameter) {
	printf("\nName: %s", ((struct Info *)parameter)->name);
	printf("\nRegistration Number: %s\n", ((struct Info *)parameter)->regNumber);

	pthread_exit(NULL);
}

int main()
{
	struct Info info;
	init(&info, "Hashir Umar", "L1F16BSCS0263");

	pthread_t pthreadId;
	if(pthread_create(&pthreadId, NULL, printInfo, (void *)&info) != 0) {
		perror("Error while creating thread\n");
		exit(0);
	}

	if(pthread_join(pthreadId, NULL) != 0) {
		perror("Error while joining thread\n");
		exit(0);
	}
	
	return 0;
}

