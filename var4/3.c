#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

struct list
{
    char *dir;
    struct list *next;
};

void add(struct list **l, char *buffer, int size)
{
    int i;
    if (*l != NULL)
        add(&((*l)->next), buffer, size);
    else
    {
        (*l) = malloc(sizeof(struct list));
        (*l)->dir = malloc(size + 1);
        for (i = 0; i < size; ++i)
            (*l)->dir[i] = buffer[i];
        (*l)->dir[size] = '\0';
        (*l)->next = NULL;
    }
}

int main()
{
    int j = 0;
    int i;
    int rc;
    int fd[2];
    char buffer[20];
    char word[71];
    struct list *subdirs = NULL;
    pipe(fd);
    if (fork() == 0)
    {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execlp("ls", "ls", "-F", NULL);
    }
    close(fd[1]);
    while ((rc = read(fd[0], &buffer, sizeof(buffer))))
    {
        i = 0;
        while (i < rc)
        {
            if ((buffer[i] == '/') && (j < 70) && (j > 0))
                add(&subdirs, word, j);
            else if (buffer[i] == '\n')
                j = 0;
            else if (j < 70)
            {
                word[j] = buffer[i];
                j++;
            }
            i++;
        }
    }
    printf("All subdirs:\n");
    while (subdirs != NULL)
    {
        printf("%s\n", subdirs->dir);
        subdirs = subdirs->next;
    }
    close(fd[0]);
    wait(NULL);
    return 0;
}
