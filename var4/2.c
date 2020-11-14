#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

int sum = 0;
int i = 0;

void alrm(int signo)
{
    signal(SIGALRM, alrm);
    i = 0;
}

void handler(int signo)
{
    signal(SIGINT, handler);
    sum = 0;
    i++;
    if(i == 1)
        alarm(2);
    if(i == 2)
        exit(0);
}

int main()
{
    signal(SIGALRM, alrm);
    signal(SIGINT, handler);
    int c;
    int cur = 0;
    int minus = 0;
    while((c = getchar()) != EOF){
        if(c != '\n'){
            if(isdigit(c)){
                cur = cur * 10 + (c - '0');
            }
            else if(c == '-'){
                minus = 1;
            }
            else{
                if(minus)
                    sum -= cur;
                else 
                    sum += cur;
                cur = 0;
                minus = 0;
            }
        }
        else{
            if(minus)
                sum -= cur;
            else
                sum += cur;
            cur = 0;
            minus = 0;
            printf("%d\n", sum);
        }    
    }
    return 0;
}
