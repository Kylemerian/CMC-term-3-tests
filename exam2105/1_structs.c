#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list
{
    int frq;
    char *word;
    struct list *next;
};

struct container
{
    int id;
    struct list *words;
    struct container *next;
};

void add_word(struct list **l, char *buff, int size)
{
    if (*l == NULL)
    {
        *l = malloc(sizeof(struct list));
        (*l)->frq = 0;
        (*l)->word = malloc(size + 1);
        (*l)->next = NULL;
        strncpy((*l)->word, buff, size + 1);
    }
    else
    {
        if (!strcmp((*l)->word, buff))
            (*l)->frq++;
        else
            add_word(&((*l)->next), buff, size);
    }
}

void container_add(struct container **c, char *buff, int size)
{
    if (*c == NULL)
    {
        *c = malloc(sizeof(struct container));
        (*c)->id = size;
        (*c)->words = NULL;
        (*c)->next = NULL;
        add_word(&((*c)->words), buff, size);
    }
    else
    {
        if ((*c)->id == size)
            add_word(&((*c)->words), buff, size);
        else
            container_add(&((*c)->next), buff, size);
    }
}

char *find_max(struct list *l)
{
    char *max_str = NULL;
    int max_frq = -1;
    while (l != NULL)
    {
        if (l->frq > max_frq)
        {
            max_frq = l->frq;
            max_str = l->word;
        }
        l = l->next;
    }
    return max_str;
}


int main(int argc, char const *argv[])
{
    int c;
    int i = 0;
    int buff_size = 32;
    char *tmp = NULL;
    char *buff = malloc(buff_size * sizeof(buff));
    struct container *con = NULL;
    while ((c = getchar()) != -1)
    {
        if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))
            || ((c >= '0') && (c <= '9')))
        {
            if (i < buff_size - 1)
            {
                buff[i] = c;
                buff[i + 1] = '\0';
                i++;
            }
            else
            {
                tmp = malloc(buff_size * 2);
                strncpy(tmp, buff, buff_size);
                free(buff);
                buff_size *= 2;
                buff = tmp;
            }
        }
        else
        {
            if (i > 0)
                container_add(&con, buff, i);
            buff[0] = '\0';
            i = 0;
        }
    }
    if (i > 0)
        container_add(&con, buff, i);
    while (con != NULL)
    {
        printf("for len %d - %s\n", con->id, find_max(con->words));
        con = con->next;
    }
    return 0;
}