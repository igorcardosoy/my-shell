#include "../utils/utils.h"

#define QUEUE_MAX_SIZE 10

typedef char* Element;
typedef struct queue *Queue;

void init_queue(Queue *queue);

bool is_full(Queue queue);

bool is_empty(Queue queue);

int size(Queue queue);

void enqueue(Queue queue, Element new_element);

Element dequeue(Queue queue);