#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

char * resize(char * buf, int * capacity)
{
    char * tmp = malloc(*capacity * 2);
    strncpy(tmp, buf, *capacity);
    *capacity = *capacity * 2;
    free(buf);
    return tmp;
}

void check(char * buf)
{
    int fl = 0;
    for(int i = 0; buf[i] != 0; i++)
        if(buf[i] == '/')
            fl = 1;
    if(fl)
        printf("%s\n", buf);
}

int main()
{
    int fd[2];
    pipe(fd);
    if(!fork()){
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        execlp("ls", "ls", "-F", NULL);
        exit(1);
    }
    if(!fork()){
        int capacity = 10;
        int size = 0;
        char * buf = malloc(capacity);
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        while(read(0, &buf[size], 1)){
            if(size == capacity - 1)
                buf = resize(buf, &capacity);
            if(buf[size] == ' ' || buf[size] == '\n'){
                buf[size] = 0;
                check(buf);
                size = 0;
                continue;
            }
            size++;
        }
        buf[size] = 0;
        check(buf);
        free(buf);
    }
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
    return 0;   
}