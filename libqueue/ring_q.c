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

ring_queue_t *queue;

struct Node *create_node(int data){
    struct Node *node=(struct Node *)malloc(sizeof(struct Node));
    node->data=data;
    node->next=NULL;
    return node;
}

void init_queue(){

}

int enqueue(int data){

}

int dequeue(){

}

void destroy_queue(){

}