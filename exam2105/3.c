#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    signal(SIGPIPE, SIG_IGN);
    int st;
    int pid;
    int fd[2];
    char buff[9];
    pipe(fd);
    if ((pid = fork()) == 0)
    {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execvp(argv[1], &argv[1]);
        exit(1);
    }
    else
    {
        close(fd[0]);
        for (int i = 0; i < 10000000; ++i)
        {
            snprintf(buff, sizeof(buff), "%d\n", i);
            write(fd[1], buff, sizeof(buff));
        }
        close(fd[1]);
    }
    wait4(pid, &st, 0, 0);
    printf("program with %d pid has exited with code %d\n", pid, st);
    return 0;
}