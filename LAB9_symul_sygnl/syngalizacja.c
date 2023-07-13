#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHARED_MEMORY_KEY 1234

struct Lane
{
    pid_t laneProcessId;
    int car[50];
    int green;
};

struct Lane* lanes;

// Flaga informujaca o otrzymaniu sygnalu
volatile int signalReceived = 0;

void sigusr1_handler(int signal){
    int pid = getpid();
    printf("Received SIGUSR1 signal in process %d\n", pid);

    // Ustawienie flagi po otrzymaniu sygnalu
    signalReceived = 1; 
    for (int i = 0; i < 4; i++) {
    if (lanes[i].laneProcessId == pid) {
        for (int j = 0; j < 50; j++) {
            if(lanes[i].car[j] == 50){
                printf("BOOOOOOOM!!!! (incident)\n");
                exit(0);
            } else if( lanes[i].car[j] == 0){
                printf("New car approaches! in lane of id: %d\n", pid);
                lanes[i].car[j] = 50;
                break;
            }
        }
        break;
    }
}
}

void* simulateSignalization(void* pid){
    int procId = getpid();
    printf("my process id: %d\n", procId);

    // Oczekiwanie na ustawienie flagi
    while (!signalReceived);
    printf("I will be a signalization!\n");
    for (int i = 0; i < 4; i++) {
    if (lanes[i].laneProcessId == procId) {
        int oneOrZero = 0;
        while(1){
            lanes[i].green = oneOrZero % 2;
            oneOrZero++;
            sleep(3);
        }
        // Green = 1 swiatlo jest wlaczone

        break;
    }
}
}

void* simulateTraffic(void* pid){
    int procId = getpid();
    printf("my process id: %d\n", procId);

    // Oczekiwanie na ustawienie flagi
    while (!signalReceived); 
    // printf("I will be a traffic flow!\n");
    for (int i = 0; i < 4; i++) {
    if (lanes[i].laneProcessId == procId) {
        for (int j = 0; j < 50; j++) {
            if( lanes[i].car[j] != 0 ) {
                    if( lanes[i].car[j] > 20 && lanes[i].green == 1){
                        for (int k = 0; k < 50; k++){
                            int counter = 0;
                            if( lanes[i].car[k] != 0){
                                lanes[i].car[k]--;
                                counter++;  
                            }
                            printf("Distance of %d cars decreased\n", counter);
                        }                   
                    } else{
                        lanes[i].car[j]--;
                        printf("Single car distnace decresead!\n");
                    }
            }
            }
        }
        break;
    }
}

void createSharedMemory() {
    int shmid = shmget(SHARED_MEMORY_KEY, sizeof(struct Lane), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }
    lanes = (struct Lane*) shmat(shmid, NULL, 0);
    if (lanes == (struct Lane*) -1) {
        perror("shmat failed");
        exit(1);
    }
}

void removeSharedMemory() {
    int shmid = shmget(SHARED_MEMORY_KEY, sizeof(struct Lane), 0666);
    if (shmid != -1) {
        shmctl(shmid, IPC_RMID, NULL);
    }
}

void createFourProcesses(){
    int i;
    pid_t child_pid, parent_id;

    for( i = 0; i < 4; i++){
        memset(lanes[i].car, 0, sizeof(lanes[i].car));
    }

    parent_id = getpid();
    printf("%d\n", parent_id);

    for( i = 0; i < 4; i++){
        child_pid = fork();

        if(child_pid < 0){
            printf("Fork failed.\n");
        } else if (child_pid == 0){
            // If it's a child process break out of the loop so that
            // it won't fork()
            lanes[i].laneProcessId = getpid();
            break;
        }
    }

    if( parent_id == getpid()) {
        int randomLaneIndex;
        srand(time(0));
        sleep(2);
        while(1){
            randomLaneIndex = rand() % 4;
            sleep(2);
            kill(lanes[randomLaneIndex].laneProcessId, SIGUSR1);
        }
    }
    
    if ( parent_id != getpid()){
        signal(SIGUSR1, sigusr1_handler);
        printf("my id: %d, my parent: %d\n", getpid(), getppid());

        pthread_t signalization_t, traffic_t;

        pid_t processId = getpid();
        if( pthread_create(&signalization_t, NULL, &simulateSignalization, NULL) != 0){
            printf("pthread_create failed.\n");
            exit(1);
        }
        pthread_join(signalization_t, NULL);

        if( pthread_create(&traffic_t, NULL, &simulateTraffic, NULL) != 0){
            printf("pthread_create failed.\n");
            exit(1);
        }
        pthread_join(traffic_t, NULL);
    }

    while(wait(NULL) != -1);
}

int main() {
    createSharedMemory();
    createFourProcesses();
    removeSharedMemory();
    return 0;
}
