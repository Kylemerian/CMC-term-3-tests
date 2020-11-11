#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int aflag = 0;
int iter = 0;

void alrm(int signo)
{
    signal(SIGALRM, alrm);
    iter = 0;
}

void handler(int signo)
{
    signal(SIGINT, handler);
    iter++;
    if(iter == 1)
        alarm(1);
    if(iter == 3)
        _exit(0);
}

int main()
{
    int c;
    signal(SIGALRM, alrm);
    signal(SIGINT, handler);
    while((c = getchar()) != EOF){
        if(c == '\n')
            printf("OK\n");
    }
    printf("Goodbye\n");
    return 0;
}
