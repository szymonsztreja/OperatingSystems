#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

sem_t printer;


int getWorkersAmountFromFile(){
   FILE * pFile;
   char buffer [100];
   int amountOfWorkers;

   pFile = fopen ("workersFile.txt" , "r");
   if (pFile == NULL) perror ("Error opening file");
   else
   {
        fscanf(pFile, "%d", &amountOfWorkers);
        fclose (pFile);
   }

   return amountOfWorkers;
}


void *workersThread(void *args){
    int sleepFor = rand() % 3 + 1;

    sem_wait(&printer);
    sleep(sleepFor);    
    printf("I am worker: %d! Im using a printer. I've waited: %ds\n", *(int*)args, sleepFor);
    sem_post(&printer);
    free(args);
}


int main() {
    int workers_amount = getWorkersAmountFromFile();
    int i;
    printf("Workers: %d\n", workers_amount);

    sem_init(&printer, 0, 1);
    pthread_t workers_thread[workers_amount];

    srand(time(0));


    for ( i = 0; i < workers_amount; i++){
        // Create a pointer of int size, then make i to point to the address of "a" pointer
        int* a = malloc(sizeof(int));
        *a = i;
        if( pthread_create( &workers_thread[i], NULL, workersThread, a)){
            perror("Failed to create thread!");
        }
    }

    for ( i = 0; i < workers_amount; i++){
        if (pthread_join(workers_thread[i], NULL) != 0){
            perror("Failed to join thread");
        }
    }

    sem_destroy(&printer);
    return 0; 
} 