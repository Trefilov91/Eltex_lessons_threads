#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

pthread_once_t once = PTHREAD_ONCE_INIT;
pthread_key_t key;

void destructor(void *buff)
{
    free(buff);
}

void key_create(void)
{
    pthread_key_create(&key, destructor);
}

int get_buyer_progress(int product)
{
    int *progress;

    pthread_once(&once, key_create);

    progress = pthread_getspecific(key);
    if(!progress) {
        progress = malloc(sizeof(int));
        *progress = 0;
        pthread_setspecific(key, progress);
    }

    *progress += product;
    return *progress;
}