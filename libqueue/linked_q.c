#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <pthreads.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdarg.h>
#include <unistd.h>

#include "queue.h"



linkedlist_queue_t *queue;

struct Node *create_node(int data){
    struct Node *node=(struct Node *)malloc(sizeof(struct Node));
    node->data=data;
    node->next=NULL;
    return node;
}

void init_queue(){
    queue->head=NULL;
    queue->tail=NULL;
    queue->size=0;
}

int enqueue(int data){
    struct Node *node=create_node(data);
    if (queue->head==NULL){
        queue->head=node;
        queue->tail=node;
    }
    queue->tail->next=node;
    queue->size++;
    return 0;
}

int dequeue(){
    if (queue->head==NULL){
        return -1;
    }
    struct Node *tmp=queue->head;
    queue->head=tmp->next;
    tmp->next=NULL;
    queue->size--;
    if (queue->size==0){
        queue->tail=NULL;
    }
    free(tmp);
    return 0;
}

void destroy_queue(){
    struct Node *curr=queue->head;
    while (curr!=NULL){
        struct Node *tmp=curr;
        curr=curr->next;
        free(tmp);
    }
}