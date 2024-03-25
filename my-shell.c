#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "queue.h"

struct alias_commands {
    char* command;
    char* name;
};

void type_prompt();
void read_command(char*, char**);
void internal_commands(char*, char**, Queue);
void external_commands(char*, char**);
void history_command(Queue, char*);
void broke_string(char*, char**);
void clean_buffer(char*, char**);
void assign(char**);
char* get_directory();
char* get_username();
char* get_hostname();
struct tm* get_time();

int main() {
    Queue history;

    init(&history);

    //system("clear");

    while (true) {
        char* command = malloc(sizeof(char) * COMMAND_SIZE);
        char* parameters[ARGS_SIZE];
        int status = 0;

        type_prompt();
        read_command(command, parameters);

        enqueue(history, command);

        internal_commands(command, parameters, history);
        external_commands(command, parameters);
    }

    return 0;
}

void type_prompt() {
    struct tm* time = get_time();
    printf("%s@%s[%02d:%02d:%02d] /%s $ ", get_username(), get_hostname(), time->tm_hour, time->tm_min, time->tm_sec, get_directory());
}

void read_command(char* command, char* parameters[]) {
    fgets(command, BUFFER_SIZE, stdin);
    broke_string(command, parameters);
}

void internal_commands(char* command, char* parameters[], Queue history) {
    if (!strcmp(command, "alias")) {
        assign(parameters);

    } else if (!strcmp(command, "history")) {
        clean_buffer(command, parameters);
        history_command(history, command);
        broke_string(command, parameters);
        internal_commands(command, parameters, history);

    } else if (!strcmp(command, "exit")) {
        _exit(0);

    } else if (!strcmp(command, "cd")) {
        if (parameters[1] == NULL) {
            chdir(getenv("HOME"));

        } else {
            chdir(parameters[1]);
        }
        clean_buffer(command, parameters);

    } else if (!strcmp(command, "cls") || !strcmp(command, "clear")) {
        system("clear");
        clean_buffer(command, parameters);
    }
}

void external_commands(char* command, char* parameters[]) {
    int status;

    if (fork() != 0) {
        waitpid(-1, &status, 0);
    } else {
        if (execvp(parameters[0], parameters) == -1) {
            clean_buffer(command, parameters);
            perror("execvp");
            _exit(1);
        }
    }
}

void history_command(Queue history, char* command) {
    int queue_size = size(history);

    for (int i = 0; i < queue_size; i++) {
        char* temp_string;
        temp_string = dequeue(history);
        printf("[%d]: %s\n", i + 1, temp_string);
        enqueue(history, temp_string);
    }

    fgets(command, BUFFER_SIZE, stdin);

    if (command[strlen(command) - 1] == '\n')
        command[strlen(command) - 1] = '\0';

    if (command[0] == '!'){
        
        command = strtok(command, "!");
        int index = atoi(command);
        
        for (int i = 0; i < queue_size; i++) {
            char* temp_string;
            temp_string = dequeue(history);
            enqueue(history, temp_string);

            if (i == index - 1) {
                snprintf(command, BUFFER_SIZE, "%s", temp_string);
            }
        }
        
        enqueue(history, command);
    } else {
        enqueue(history, command);
        printf("internal command \"%s\" not found\n", command);
    }
}

void broke_string(char* command, char* parameters[]) {
    char* token;

    if (command[strlen(command) - 1] == '\n')
        command[strlen(command) - 1] = '\0';

    token = strtok(command, " ");

    strcpy(command, token);

    int i = 1;
    while (token != NULL) {
        token = strtok(NULL, " ");
        if (token != NULL) {
            parameters[i] = malloc(sizeof(char) * BUFFER_SIZE);
            snprintf(parameters[i], BUFFER_SIZE, "%s", token);
        }

        i++;
    }

    parameters[0] = malloc(sizeof(char) * BUFFER_SIZE);
    snprintf(parameters[0], sizeof(command), "%s", command);

    parameters[i-1] = NULL;
}

void clean_buffer(char* command, char* parameters[]) {
    command = NULL;

    for (int i = 0; parameters[i] != NULL; i++) {
        parameters[i] = NULL;
    }
}

void assign(char* parameters[]){

}

char* get_directory() {
    char* buf = malloc(sizeof(char) * 1024);
    char* directory = malloc(sizeof(char) * 1024);
    char* cwd;
    char* token;

    cwd = getcwd(buf, 1024);
    token = strtok(cwd, "/");

    strcpy(cwd, token);

    int i = 0;
    while (token != NULL) {
        token = strtok(NULL, "/");

        if (token != NULL)
            snprintf(directory, BUFFER_SIZE, "%s", token);

        i++;
    }

    return directory;
}

char* get_username() {
    char* username = getenv("USER");
    return username;
}

char* get_hostname() {
    char* hostname = malloc(sizeof(char) * 100);
    gethostname(hostname, 100);
    return hostname;
}

struct tm* get_time() {
    time_t segundos;
    time(&segundos);

    return localtime(&segundos);
}