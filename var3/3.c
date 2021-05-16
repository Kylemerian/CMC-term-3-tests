#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
    int ind = 0;
    char ** cmd = malloc((argc + 1) * sizeof(*cmd));
    int fd1[2];
    int fd2[2];

    for (int i = 0; i < argc; i++){
        if(strcmp("---", argv[i]))
            cmd[i] = argv[i];
        else{
            ind = i;
            cmd[i] = NULL;
        }      
    }
    cmd[argc] = NULL;
    pipe(fd1);
    pipe(fd2);
    if(!fork()){    //1
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        dup2(fd1[1], 1);
        close(fd1[1]);
        execvp(cmd[1], &cmd[1]);
        perror(NULL);
        fflush(stderr);
        _exit(1);
    }
    if(!fork()){    //filter
        char c;
        int res;
        int fl = 0;         // to print
        int endline = 1;    // \n
        close(fd2[0]);
        close(fd1[1]);
        dup2(fd2[1], 1);
        dup2(fd1[0], 0);
        close(fd2[1]);
        close(fd1[0]);
        while(res){
            res = read(0, &c, 1);
            if(endline){
                endline = 0;
                if(c == ' ' || c == '\t')
                    fl = 1;
            }
            if(fl)
                write(1, &c, 1);
            if(c == '\n'){
                fl = 0;
                endline = 1;
            }
        }
    }
    if(!fork()){    //2
        close(fd2[1]);
        close(fd1[0]);
        close(fd1[1]);
        dup2(fd2[0], 0);
        close(fd2[0]);
        execvp(cmd[ind + 1], &cmd[ind + 1]);
        perror(NULL);
        fflush(stderr);
        _exit(1);
    }
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}