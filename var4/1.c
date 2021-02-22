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

void sort(struct list *l)
{
    int tmp_c;
    char *tmp_w;
    struct list *tmp1 = l, *tmp2 = l;
    while (tmp1 != NULL)
    {
        while (tmp2->next != NULL)
        {
            if (tmp1->count < tmp2->count)
            {
                tmp_c = tmp1->count;
                tmp_w = tmp1->word;
                tmp1->count = tmp2->next->count;
                tmp1->word = tmp2->next->word;
                tmp2->next->count = tmp_c;
                tmp2->next->word = tmp_w;
            }
            tmp2 = tmp2->next;
        }
        tmp1 = tmp1->next;
    }
}

void add(struct list **l, char *buffer, int size)
{
    int i;
    if (buffer[0] != '\0') {
        if (*l != NULL) {
            if (compare((*l)->word, buffer))
                (*l)->count++;
            else
                add(&((*l)->next), buffer, size);
        }
        else
        {
            *l = malloc(sizeof(struct list));
            (*l)->word = malloc(sizeof(char) * size);
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
    while ((c = getchar()) != '\n')
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
            buffer[i] = '\0';
            add(&l, buffer, i + 1);
            tmp_size = BUFF_SIZE;
            i = 0;
        }
    }
    buffer[i] = '\0';
    add(&l, buffer, i + 1);
    sort(l);
    i = 0;
    while(l != NULL && i < 10)
    {
        printf("%s - %d\n", l->word, l->count);
        l = l->next;
        i++;
    }
}