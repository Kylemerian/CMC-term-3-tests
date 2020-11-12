#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

int sflag = 0;
int pid;
int cnt = 0;

void alrm(int signo)
{
    signal(SIGALRM, alrm);
    cnt = 0;
}

void handlerINT(int signo)
{
    signal(SIGINT, handlerINT);
    cnt++;
    if(cnt == 1)
        alarm(1);
    if(cnt == 2){
        kill(pid, SIGKILL);
    }
    else if(sflag == 0){
        sflag = !sflag;
        kill(pid, SIGSTOP);
    }
    else{
        sflag = !sflag;
        kill(pid, SIGCONT);
    } 
}

int main(int argc, char ** argv)
{
    int fd0 ,fd1;
    signal(SIGALRM, alrm);
    signal(SIGINT, handlerINT);
    pid = fork();
    if(pid == 0){
        signal(SIGINT, SIG_IGN);
        fd0 = open(argv[1], O_RDONLY);
        fd1 = open(argv[2], O_WRONLY);
        dup2(fd0, 0);
        dup2(fd1, 1);
        execvp(argv[3], &argv[3]);
        perror(argv[3]);
        fflush(stderr);
        _exit(1);
    }
    wait(NULL);
    return 0;
}
