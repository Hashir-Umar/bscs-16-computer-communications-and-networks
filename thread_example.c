#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct Student {
    int age;
    char *name;
};

void *threadFunction(void*);
void init(struct Student*, int, char*);

int main() {

    int age;
    char name[50];

    //      taking input from user     //
    /**********************************/
    printf("Enter student name: ");
    scanf("%[^\t\n]", name);

    printf("Enter student age: ");
    scanf("%d", &age);
    /**********************************/


    //declaring student object
    struct Student student;

    //initializing student object
    init(&student, age, name);

    pthread_t tid;
    //creating thread and passing student object as thread parameter
    pthread_create(&tid, NULL, threadFunction, (void*) &student);  

    //wating for the thread  
    pthread_join(tid, NULL);

    printf("\n");
    return 0;
}   

void *threadFunction(void *params) {

    int age = ((struct Student *) params)->age;
    char *name = ((struct Student *) params)->name;
    
    printf("\nName: %s", name);
    printf("\nAge: %d", age);

    free(name);
    return NULL;
}

void init(struct Student *obj, int age, char *name) {
    obj->age = age;
    obj->name = malloc(strlen(name) + 1);
    strcpy(obj->name, name);
}
