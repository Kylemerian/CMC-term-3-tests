#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int ended = 0;

void hdl(int signo)
{
    ended = 1;
};

int main(int argc, char ** argv)
{
    int res;
    int pid;
    char * wrds[12] = {"alpha", "beta", "gamma", "epsilon", "zeta", "eta", "teta", "yota",
        "kappa", "lambda", "mu", "\n"};
    signal(SIGCHLD, hdl);
    int fd[2];
    pipe(fd);
    pid = fork();
    if(!pid){
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execvp(argv[1], &argv[1]);
        exit(1);
    }
    if(!fork()){
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        for(int i = 0; i < 1000000; i++){
            if(!ended)
                for(int j = 0; j < 11; j++)
                    if(!ended)
                        printf("%s\n", wrds[j]);
                    else
                        break;
            else
                break;
        }
        close(fd[1]);
        exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    wait4(pid, &res, 0, 0);
    printf("%d exit code = %d\n", pid, res);
    return 0;
}