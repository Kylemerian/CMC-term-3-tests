#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

int pid;
int stop = 1;

void alrm()
{
    signal(SIGALRM, alrm);
    if (stop)
    {
        kill(pid, SIGSTOP);
        stop = 0;
    }
    else
    {
        kill(pid, SIGCONT);
        stop = 1;
    }
    alarm(1);
}

int main(int argc, char **argv)
{
    int fd1;
    int fd2;
    signal(SIGALRM, alrm);
    alarm(1);
    if ((pid = fork()) == 0)
    {
        fd1 = open(argv[1], O_RDONLY);
        fd2 = open(argv[2], O_WRONLY);
        dup2(fd1, 0);
        dup2(fd2, 1);
        close(fd1);
        close(fd2);
        execvp(argv[3], argv + 3);
        perror(argv[3]);
        exit(1);
    }
    wait(NULL);
    return 0;
}