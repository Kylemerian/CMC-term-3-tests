#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int count = 0;

void reset()
{
    count = 0;
}

void hdl()
{
    signal(SIGINT, hdl);
    signal(SIGALRM, reset);
    count++;
    if (count == 1)
        alarm(1);
    if (count == 3)
        exit(0);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT ,hdl);
    int c;
    while ((c = getchar()) != -1) 
    {
        if (c == '\n')
            printf("Ok\n");
    }
    printf("Goodbye\n");
    return 0;
}