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

int str_len(const char * s)
{
    int len = 0;
    for (len = 0; s[len] != 0; len++)
        ;
    return len;
}

int main(int argc, char ** argv)
{
    int res;
    char * wrds[12] = {"alpha", "beta", "gamma", "epsilon", "zeta", "eta", "teta", "yota",
        "kappa", "lambda", "mu", "\n"};
    signal(SIGCHLD, hdl);
    signal(SIGPIPE, SIG_IGN);   //prevent exit of main proc
    int fd[2];
    pipe(fd);
    if(fork() == 0){
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execvp(argv[1], &argv[1]);
        exit(1);
    }
    close(fd[0]);
    for(int i = 0; i < 1000000; i++){
        if(!ended)
            for(int j = 0; j < 11; j++)
                if(!ended){
                    write(fd[1], wrds[j], str_len(wrds[j]));
                    write(fd[1], wrds[11], 1);
                }
                    //dprintf(fd[1], "%s\n", wrds[j]);
                else
                    break;
        else
            break;
    }
    close(fd[1]);
    wait(&res);
    printf("exit code = %d\n", res);
    return 0;
}