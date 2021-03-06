#include <stdio.h>
#include <stdlib.h>

const int BUFF_SIZE = 16;

struct list
{
    char *word;
    int count;
    struct list *next;
};

int compare(char *w1, char *w2)
{
    int i = 0;
    while (w1[i] != '\0' && w2[i] != '\0')
    {
        if (w1[i] != w2[i])
            return 0;
        i++;
    }
    if (w1[i] == w2[i])
        return 1;
    return 0;
}

void swap(struct list *tmp1, struct list *tmp2)
{
    int tmp_c;
    char *tmp_w;
    tmp_c = tmp1->count;
    tmp_w = tmp1->word;
    tmp1->count = tmp2->count;
    tmp1->word = tmp2->word;
    tmp2->count = tmp_c;
    tmp2->word = tmp_w;
}

void sort(struct list *l)
{
    struct list *tmp1 = l, *tmp2 = l;
    while (tmp1 != NULL)
    {
        while (tmp2 != NULL)
        {
            if ((tmp1->count > tmp2->count) && (tmp1 != tmp2))
                swap(tmp1, tmp2);
            tmp2 = tmp2->next;
        }
        tmp1 = tmp1->next;
        tmp2 = l;
    }
}

void add(struct list **l, char *buffer, int size)
{
    int i;
    if (buffer[0] != '\0') {
        buffer[size] = '\0';
        if (*l != NULL) 
        {
            if (compare((*l)->word, buffer))
                (*l)->count++;
            else
                add(&((*l)->next), buffer, size);
        }
        else
        {
            *l = malloc(sizeof(struct list));
            (*l)->word = malloc(sizeof(char) * (size + 1));
            (*l)->count = 1;
            (*l)->next = NULL;
            for (i = 0; i < size; ++i)
                (*l)->word[i] = buffer[i];
        }
    }
}

int main()
{
    int c;
    int i = 0;
    int j;
    int tmp_size = BUFF_SIZE;
    char *buffer = malloc(sizeof(char) * tmp_size);
    char *tmp = NULL;
    struct list *l = NULL;
    while ((c = getchar()) != EOF)
    {
        if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))
            || ((c >= '0') && (c <= '9')))
        {
            if (i + 1 == tmp_size)
            {
                tmp = malloc(sizeof(char) * tmp_size * 2);
                for (j = 0; j < tmp_size; ++j)
                    tmp[j] = buffer[j];
                free(buffer);
                tmp_size *= 2;
                buffer = tmp;
            }
            buffer[i] = c;
            i++;
        }
        else
        {
            add(&l, buffer, i);
            tmp_size = BUFF_SIZE;
            i = 0;
        }
    }
    add(&l, buffer, i);
    sort(l);
    i = 0;
    while(l != NULL && i < 10)
    {
        printf("%s - %d\n", l->word, l->count);
        l = l->next;
        i++;
    }
}