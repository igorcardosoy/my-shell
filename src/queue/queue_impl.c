#include "queue.h"

struct queue{
    Element items[QUEUE_MAX_SIZE];
    int last, first, size;
};

void init_queue(Queue* queue){
    Queue q;
    q = malloc(sizeof(struct queue));
    q->last = -1;
    q->first = -1;
    q->size = 0;
    *queue = q;
}

bool is_full(Queue queue){
    return queue->size == QUEUE_MAX_SIZE;
}

bool is_empty(Queue queue){
    return queue->size == 0;
}

int size(Queue queue){
    return queue->size;
}

void enqueue(Queue queue, Element new_element){
    char* aux = malloc(sizeof(char) * BUFFER_SIZE);
    snprintf(aux, BUFFER_SIZE, "%s", new_element);

    if(queue->last == QUEUE_MAX_SIZE - 1)
            queue->last = 0;
    else
        queue->last += 1;
    
    if(is_full(queue)){
        queue->size--;
        queue->first = queue->last;
    }

    queue->items[queue->last] = aux;
    queue->size += 1;
}

Element dequeue(Queue queue){
    Element answer = "The history is empty.";
    if(!is_empty(queue)){
        if(queue->first == QUEUE_MAX_SIZE - 1)
            queue->first = 0;
        else
            queue->first += 1;
        
        queue->size -= 1;
        answer = queue->items[queue->first];
    }
    return answer;
}