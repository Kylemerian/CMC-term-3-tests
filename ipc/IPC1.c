#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int arraySum(int * arr)
{
    int i;
    int res = 0;
    for(i = 0; i < 10; i++){
        printf("%d ", arr[i]);
        res += arr[i];
    }
    printf("\n");
    return res;
}

int main()
{
    int i;
    struct sembuf sem[1];
    /*unsigned short j = 1;*/
    int key = ftok("file", 'a');
    int shmId = shmget(key, sizeof(int) * 10, 0666 | IPC_CREAT);
    int semId = semget(key, 1, 0666 | IPC_CREAT);
    int * arr = shmat(shmId, NULL, 0);
    semctl(semId, 0, SETVAL, 1);
    sem[0].sem_num = 0;
    sem[0].sem_flg = 1;
    for(i = 0; i < 10; i++)
        arr[i] = 0;
    while(1){
        sem[0].sem_op = -1;
        semop(semId, sem, 1);
        printf("sum = %d\n", arraySum(arr));
        sem[0].sem_op = 1;
        semop(semId, sem, 1);
        sleep(1);
    }

    shmdt(arr);
    shmctl(shmId, IPC_RMID, NULL);
    semctl(semId, 0, IPC_RMID, 0);
    return 0;
}