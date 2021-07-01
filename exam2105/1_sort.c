#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list
{
    int frq;
    int len;
    char *word;
    struct list *next;
};

void add_word(struct list **l, char *buff, int size)
{
    if (*l == NULL)
    {
        *l = malloc(sizeof(struct list));
        (*l)->len = size;
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

void swap(struct list *tmp1, struct list *tmp2)
{
    int tmp_c;
    int tmp_len;
    char *tmp_w;
    tmp_c = tmp1->frq;
    tmp_w = tmp1->word;
    tmp_len = tmp1->len;
    tmp1->frq = tmp2->frq;
    tmp1->word = tmp2->word;
    tmp1->len = tmp2->len;
    tmp2->frq = tmp_c;
    tmp2->word = tmp_w;
    tmp2->len = tmp_len;
}

void sort(struct list *l)
{
    struct list *tmp1 = l, *tmp2 = l;
    while (tmp1 != NULL)
    {
        while (tmp2 != NULL)
        {
            if (((tmp1->len < tmp2->len) || 
                ((tmp1->len == tmp2->len) && tmp1->frq > tmp2->frq)) && 
                (tmp1 != tmp2))
                swap(tmp1, tmp2);
            tmp2 = tmp2->next;
        }
        tmp1 = tmp1->next;
        tmp2 = l;
    }
}

int main(int argc, char const *argv[])
{
    int c;
    int i = 0;
    int old_len = 0;
    int buff_size = 32;
    char *tmp = NULL;
    char *buff = malloc(buff_size * sizeof(buff));
    struct list *l = NULL;
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
                if (i != 0)
                    add_word(&l, buff, i);
                buff[0] = '\0';
                i = 0;
            }
    }
    add_word(&l, buff, i);
    sort(l);
    while(l != NULL)
    {
        if (old_len != l->len)
        {
            printf("len: %d, word: %s\n",l->len, l->word);
            old_len = l->len;
        }
        l = l->next;
    }

    return 0;
}