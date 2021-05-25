#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int sec5 = 0;
int sec1 = 0;
int pid;

int str_len(const char * s)
{
    int len = 0;
    for (len = 0; s[len] != 0; len++)
        ;
    return len;
}

void alarm1(int signo)
{
    char s[] = "ended > 6 sec";
    sec1 = 1;
    write(1, &s, str_len(s));
    kill(pid, SIGKILL);
    exit(0);
}

void alarm5(int signo)
{
    sec5 = 1;
}

void child1(int signo)
{
    char s[] = "ended < 5 sec";
    write(1, &s, str_len(s));
    exit(0);
}

void child2(int signo)
{
    char s[] = "ended > 5 sec < 6 sec";
    write(1, &s, str_len(s));
    exit(0);
}

int main(int argc, char ** argv)
{
    char * wrds[12] = {"alpha", "beta", "gamma", "epsilon", "zeta", "eta", "teta", "yota",
        "kappa", "lambda", "mu", "\n"};
    int fd[2];
    pipe(fd);
    pid = fork();
    if(!pid){
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execvp(argv[1], &argv[1]);
        perror(argv[1]);
        exit(1);
    }
    close(fd[0]);    
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, alarm5);
    signal(SIGCHLD, child1);
    alarm(5);
    while(1){
        for(int i = 0; i < 11; i++){
            if(sec5)
                break;
            write(fd[1], wrds[i], str_len(wrds[i]));
            write(fd[1], wrds[11], 1);
        }
        if(sec5)
            break;
    }
    close(fd[1]);
    signal(SIGALRM, alarm1);
    signal(SIGCHLD, child2);
    alarm(1);
    while(1)
        ;
    return 0;
}


