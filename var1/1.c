#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>


typedef struct list{
    int i;
    int pid;
    struct list * next;
}
list;

list * addToList(list * head, int pid, int i){
    list * tmp = malloc(sizeof(*tmp));
    tmp -> pid = pid;
    tmp -> i = i;
    tmp -> next = head;
    return tmp;
}

int findNextCmd(char ** arr, int i)
{
    int k;
    for(k = i; arr[k] != NULL; k++)
        ;
    return k + 1;
}

int findName(list * head, int pid)
{
    int key = 0;
    list * tmp = head;
    while(tmp != NULL){
        if(tmp -> pid == pid)
            key = tmp -> i;
        tmp = tmp -> next;
    }
    return key;
}

void func(int argc, char ** arr)
{
    list * head = NULL;
    int res, status;
    int tres, tstatus;
    int name;
    int i = 1;
    int pid;
    while(i < argc){
        pid = fork();
        if(pid == 0){
            execvp(arr[i], &arr[i]);
            perror(arr[i]);
            fflush(stderr);
            _exit(1);
        }
        else
            head = addToList(head, pid, i);
        i = findNextCmd(arr, i);
    }
    res = wait4(-1, &status, WNOHANG, NULL);
    do{
        tres = wait4(-1, &tstatus, WNOHANG, NULL);
        if(tres > 0){
            res = tres;
            status = tstatus;
        }
    }
    while(tres > -1);
    name = findName(head, res);
    printf("last cmd = %s signo = %d\n", arr[name], status); 
}

int main(int argc, char ** argv)
{
    int i;
    char ** arr = malloc((argc + 1) * sizeof(*arr));
    for(i = 0; i < argc; i++){
        if(!strcmp("---", argv[i]))
            arr[i] = NULL;
        else{
            arr[i] = argv[i];
        }
    }
    arr[argc] = NULL;
    func(argc, arr);
    return 0;
}
