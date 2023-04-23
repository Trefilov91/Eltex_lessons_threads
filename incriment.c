#include <stdio.h>
#include <pthread.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <sys/wait.h>
// #include <stdlib.h>

#define MAX_ITERATION 1000000 
#define THREAD_ITERATION (MAX_ITERATION / 10) 
#define THREAD_NUMBER 10

int counter = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void *thread_counter(void *args)
{
    int i;
    for(i = 0; i < THREAD_ITERATION; i++){
        pthread_mutex_lock(&mutex1);
        counter++;
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}

int main()
{
    int i;
    int *thread_status;
    pthread_t thread[THREAD_NUMBER];

    for(i = 0; i < THREAD_NUMBER; i++){
        pthread_create(&thread[i], NULL, thread_counter, NULL);
    }

    for(i = 0; i < THREAD_NUMBER; i++){
        pthread_join(thread[i], (void **)&thread_status);
    }

    printf("%d\n", counter);
    return 0;
}