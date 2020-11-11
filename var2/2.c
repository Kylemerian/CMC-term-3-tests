#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

/*in progress*/

char * extendbuff(char * buff, int * len)
{
    char * newB = malloc(*len * 2);
    strncpy(newB, buff, *len);
    *len *= 2;
    free(buff);
    return newB;
}

int findCmd(char ** arr)
{
    int i = 1;
    while(arr[i] != NULL)
        i++;
    return i + 1;
}

void func(int argc, char ** arr)
{
    int i = 0;
    int lenbuff = 8;
    char * buff = malloc(8);
    int c;
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    if(fork() == 0){
        printf("%s", arr[1]);
        close(fd2[0]);
        close(fd2[1]);
        close(fd1[0]);
        dup2(fd1[1], 1);
        execvp(arr[1], arr + 1);
        perror(arr[1]);
        fflush(stderr);
        _exit(1);
    }
    else if(fork() == 0){
        close(fd1[1]);
        close(fd2[0]);
        dup2(fd1[0], 0);
        dup2(fd2[1], 1);
        while((c = read(0, &c, 1)) != 0){
            if(c != '\n'){
                if(i >= lenbuff)
                    buff = extendbuff(buff, &lenbuff);
                buff[i++] = c;
            }
            else{
                buff[i] = 0;
                printf("%s\n", buff);
                i = 0;
            }
        }
        buff[i] = 0;
        printf("%s\n", buff);
        write(1, &c, 1);
    }
    else if(fork() == 0){
        printf("%s", arr[findCmd(arr)]);
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[1]);
        dup2(fd2[0], 0);
        execvp(arr[findCmd(arr)], &arr[findCmd(arr)]);
        perror(arr[findCmd(arr)]);
        fflush(stderr);
        _exit(1);
    }
    else{
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[1]);
        close(fd2[0]);
        while(wait(NULL) != -1)
            ;
    }
}

int main(int argc, char ** argv)
{
    char ** arr = malloc((argc + 1) * sizeof(*arr));
    int i;
    for(i = 0; i < argc; i++){
        if(!strcmp("---", argv[i]))
            arr[i] = NULL;
        else
            arr[i] = argv[i];
    }
    arr[argc] = NULL;
    func(argc, arr);
    return 0;
}
