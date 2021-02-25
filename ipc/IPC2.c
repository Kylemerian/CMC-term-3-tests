#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
    int randIndex;
    struct sembuf sem[1];
    int key = ftok("file", 'a');
    int shmId = shmget(key, sizeof(int) * 10, 0666);
    int semId = semget(key, 1, 0666);
    int * arr = shmat(shmId, NULL, 0);
    sem[0].sem_num = 0;
    sem[0].sem_flg = 1;
    while(1){
        sem[0].sem_op = -1;
        semop(semId, sem, 1);
        randIndex = rand() % 10;
        arr[randIndex]++;
        randIndex = rand() % 10;
        arr[randIndex]--;
        sem[0].sem_op = 1;
        semop(semId, sem, 1);
    }
    return 0;
}