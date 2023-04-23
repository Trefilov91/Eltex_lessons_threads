#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#define PRODUCT_TARGET    10000
#define PRODUCT_INCREMENT 1000
#define SHOP_INIT_VAL     1000
#define BUYER_NUMBER      3
#define SHOPS_NUMBER      5

typedef struct{
    int id;
    int progress;    
} buyer_t;

int shops[SHOPS_NUMBER];
pthread_mutex_t mutex[SHOPS_NUMBER];

void *thread_buyer(void *args)
{
    buyer_t *buyer = (buyer_t *)args;
    int i;

    while(buyer->progress < PRODUCT_TARGET) {
        for(i = 0; i < SHOPS_NUMBER; i++) {
            if( pthread_mutex_trylock(&mutex[i]) )
                continue;

            buyer->progress += shops[i];
            if(buyer->progress < PRODUCT_TARGET){
                shops[i] = 0;
            }
            else {
                shops[i] = buyer->progress - PRODUCT_TARGET;
                pthread_mutex_unlock(&mutex[i]);
                buyer->progress = PRODUCT_TARGET;                
                printf("buyer %d has %d products, thets enough\n", buyer->id, buyer->progress);
                return NULL;
            }
            pthread_mutex_unlock(&mutex[i]);
            printf("buyer %d has %d products\n", buyer->id, buyer->progress);
            sleep(2);
        }
    }

    return NULL;
}

void *thread_loader(void *args)
{
    int product_increment = *(int *)args;
    int i;

    while(1) {
        for(i = 0; i < SHOPS_NUMBER; i++) {
            if( pthread_mutex_trylock(&mutex[i]) )
                continue;

            shops[i] += product_increment;
            pthread_mutex_unlock(&mutex[i]);
            printf("shop %d has %d products\n", i+1, shops[i]);
            sleep(1);            
        }
    }
    
    return NULL;
}

int main()
{
    int i, product_increment = PRODUCT_INCREMENT; 
    int *thread_status;
    pthread_t thread_buyer_id[BUYER_NUMBER], thread_loader_id;
    buyer_t buyer[BUYER_NUMBER];

    for(i = 0; i < SHOPS_NUMBER; i++){
        shops[i] = SHOP_INIT_VAL;
        pthread_mutex_init(&mutex[i], NULL);
        printf("shop %d has %d products\n", i+1, shops[i]);
    }

    for(i = 0; i < BUYER_NUMBER; i++){
        buyer[i].id = i+1;
        buyer[i].progress = 0;
        pthread_create(&thread_buyer_id[i], NULL, thread_buyer, (void *)&buyer[i]);
    }
    pthread_create(&thread_loader_id, NULL, thread_loader, (void *)&product_increment);

    for(i = 0; i < BUYER_NUMBER; i++){
        pthread_join(thread_buyer_id[i], (void **)&thread_status);
        printf("buyer %d end work\n", buyer[i].id);
    }

    pthread_cancel(thread_loader_id); 
    printf("loader was stoped\n");

    return 0;
}