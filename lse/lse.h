#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct alias_commands* Alias;

typedef struct node* Node;

typedef char* Typeinfo;

void init_list(Alias*);

void insert_command(Alias, Typeinfo, Typeinfo);

Typeinfo find_command(Alias, Typeinfo);

bool remove_command(Alias, Typeinfo);

void destroy_list(Alias*);