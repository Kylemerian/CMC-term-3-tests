#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list{
    char * str;
    int cnt;
    struct list * next;
}
list;

list * init(list * head)
{
    head = malloc(sizeof( * head));
    head -> cnt = 0;
    head -> str = NULL;
    head -> next = NULL;
    return head;
}


int commonS(int c)
{
    return(((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '1') && (c <= '9')));
}

int inList(list * head, char * buff)
{
    list * tmp = head;
    int res = 0;
    while(tmp != NULL){
        if(!strcmp(tmp -> str, buff)){
            res = 1;
            tmp -> cnt++;
        }
        tmp = tmp -> next;
    }
    return res;
}

char * extendbuff(char * buff, int * lenbuff)
{
    char * newBuff = malloc((*lenbuff) * 2);
    strncpy(newBuff, buff, (*lenbuff) - 1);
    *lenbuff *= 2;
    free(buff);
    return newBuff;
}
list * addToList(list * head, char * buff, int lenbuff){
        list * tmp = malloc(sizeof(*tmp));
        tmp -> cnt = lenbuff;
        tmp -> next = head;								#######dont need this staff######
        tmp -> str = malloc(lenbuff + 1);
        strncpy(tmp -> str, buff, lenbuff);
        tmp -> str[lenbuff] = 0;
        return tmp;
}

int findMax(list * head)
{
    int key = 0;
    list * tmp = head;
    while(tmp != NULL){
        if(tmp -> cnt > key) 
            key = tmp -> cnt;
        tmp = tmp -> next;
    }
    return key;
}

void printlist(list * head, int key)
{
    if (head != NULL) {
        printlist(head -> next, key);
        if (head -> cnt == key)
            printf("%s %d\n", head -> str, head -> cnt);
    }
}

void printmax(list * head)
{
    int key = findMax(head);
    printlist(head, key);
}

int main()
{
    int c, i = 0, lenbuff = 8;
    char * buff = malloc(lenbuff);
    list * head = NULL;
    while((c = getchar()) != EOF){
        if(c != '\n'){
            if(commonS(c)){
                if(i >= lenbuff - 1)
                    buff = extendbuff(buff, &lenbuff);
                buff[i++] = c;
            }
            else{
                if(i != 0)
                    head = addToList(head, buff, i);
                i = 0;
            }
        }
        else{
            buff[i] = 0;
            if(i != 0)
                head = addToList(head, buff, i);
            i = 0;
        }
    }
    printmax(head);
    return 0;
}
