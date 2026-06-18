struct Node{
    int data;
    struct Node *next;
}

// runtime > memory
// head, tail, next ptr can make all O(1)
// enqueue at tail, dequeue at head
// if opposite, need prev pointer. o.w. O(n) for dequeue
// without tail ptr, enqueue is O(n)
typedef struct{
    struct Node *head;
    struct Node *tail;
    int size;
}linkedlist_queue_t;

typedef struct{
    struct Node *buf;
    int head;
    int tail;
    int cap;
    int size;
}ring_queue_t;



struct Node *create_node(int data);

