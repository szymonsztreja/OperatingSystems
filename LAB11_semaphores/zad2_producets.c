#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

sem_t commodity_buffor;
// sem_t buffor_full;
// sem_getvalue(&commodity_buffor, &v)
struct zad2
{
    int producersAmount;
    int consumersAmount;
};


void getProdConsAmountFromFile(struct zad2 *prod_con){
    FILE *pFile;
    char line[100];
    int producers, consumers;

    pFile = fopen("producers.txt", "r");
    if ( pFile == NULL){
        perror ("Error opening file");
        exit(1);
    } 

    while( fgets(line, sizeof(line), pFile)) {
        if (strstr(line, "Producers:") != NULL){
            sscanf(line, "Producers:%d", &producers);
        } else if(strstr(line, "Consumers:") != NULL){
            sscanf(line, "Consumers:%d", &consumers);
        }
    }
    fclose(pFile);

    prod_con->consumersAmount = consumers;
    prod_con->producersAmount = producers;
}


void *consumersThread(void *args){
    int sleepFor = rand() % 3 + 1;
    int id = *(int*) args;
    int v;   

    while(1){
        sem_getvalue(&commodity_buffor, &v);
        if( v < 1){
            printf("(C%d) Im waiting to consume! Buffor: %d\n",
        id, v);
        }
        sem_wait(&commodity_buffor);  
        printf("(C%d) Im consuming. Buffor: %d\n", id, v);
        sleep(sleepFor);
        printf(" (C%d) I have eaten. Bye!\n", id);
    }
    // free(args);
}

void *producersThread(void *args){
    int sleepFor = rand() % 3 + 1;  
    int id = *(int*) args;
    int value;
    while(1){
            while(1){
                sem_getvalue(&commodity_buffor, &value);
                printf("val inside prod: %d\n", value);
                if (value < 5) {
                break;
            } else {
                sleep(1);
                printf(" (P%d) Buffor full %d!\n",id, value);
            }
            }
        // sem_wait(&commodity_buffor);
        printf(" (P%d) Im producing.\n", id);
        int val1 = sem_getvalue(&commodity_buffor, &value);
        if(val1 < 5){
            sem_post(&commodity_buffor);
        }
        sleep(sleepFor);    
        printf(" (P%d) I have produced!\n", 
        id);
    }
    // free(args    );
}   


int main() {
    struct zad2 producers_consumers;    
    getProdConsAmountFromFile(&producers_consumers);
    int i, producers, consumers;
    producers = producers_consumers.producersAmount;
    consumers = producers_consumers.consumersAmount;

    sem_init(&commodity_buffor, 0, 5);
    // sem_init(&buffor_full, 0, 0);
    pthread_t consumers_thread[consumers];
    pthread_t producers_thread[producers];

    srand(time(0));


    for ( i = 0; i < consumers; i++){
        // Create a pointer of int size, then make i to point to the address of "a" pointer
        int* a = malloc(sizeof(int));
        *a = i;
        if( pthread_create( &consumers_thread[i], NULL, consumersThread, a)){
            perror("Failed to create thread!");
        }
    }

    for ( i = 0; i < producers; i++){
        // Create a pointer of int size, then make i to point to the address of "a" pointer
        int* b = malloc(sizeof(int));
        *b = i;
        if( pthread_create( &producers_thread[i], NULL, producersThread, b)){
            perror("Failed to create thread!");
        }
    }

    for ( i = 0; i < producers; i++){
        if (pthread_join(producers_thread[i], NULL) != 0){
            perror("Failed to join thread");
        }
    }

    for ( i = 0; i < consumers; i++){
        if (pthread_join(consumers_thread[i], NULL) != 0){
            perror("Failed to join thread");
        }
    }

    sem_destroy(&commodity_buffor);
    // sem_destr    oy(&buffor_full);
    return 0; 
} 