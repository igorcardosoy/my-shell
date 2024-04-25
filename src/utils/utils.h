#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define ARGS_SIZE 20

void type_prompt();

void clean_buffer(char*, char**);

void remove_trailing_newline(char*);

void separate_parameters(char*, char**);

bool is_alias(char*);

void separate_alias(char*, char**);

char* get_directory();

char* get_username();

char* get_hostname();

struct tm* get_time();