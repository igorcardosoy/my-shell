#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define QUEUE_MAX_SIZE 10
#define BUFFER_SIZE 100
#define COMMAND_SIZE 200
#define ARGS_SIZE 20

typedef char* Element;
typedef struct queue *Queue;

void init_queue(Queue *queue);

bool is_full(Queue queue);

bool is_empty(Queue queue);

int size(Queue queue);

void enqueue(Queue queue, Element new_element);

Element dequeue(Queue queue);