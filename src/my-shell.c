/*
    Para compilar o programa, execute o comando:
    $ make

    Se preferir compilar manualmente, execute o comando:
    $ gcc -o my-shell ./src/my-shell.c ./src/utils/utils.c ./src/queue/queue_impl.c ./src/lse/lse_impl.c

    Para executar o programa, execute o comando:
    $ ./my-shell
*/

#include "./utils/utils.h"
#include "./queue/queue.h"
#include "./lse/lse.h"

void read_command(char*, char**, Queue);
void internal_commands(char*, char**, Queue, Alias);
void external_commands(char*, char**);
void history_command(char*, char**, Queue, Alias);
void process_string(char*, char**);
void assign(char*, char**, Alias);
void unassign(char*, char**, Alias);
void exit_shell(char*, char**);
void cd_command(char*, char**);
void clear_screen(char*, char**);

int main() {
    Queue history;
    Alias alias;

    init_queue(&history);
    init_list(&alias);

    system("clear");

    while (true) {
        char* command = malloc(sizeof(char) * BUFFER_SIZE);
        char* parameters[ARGS_SIZE];

        read_command(command, parameters, history);

        internal_commands(command, parameters, history, alias);
        external_commands(command, parameters);
    }

    return 0;
}

void read_command(char* command, char* parameters[], Queue history) {

    type_prompt();
    fgets(command, BUFFER_SIZE, stdin);

    process_string(command, parameters);

    enqueue(history, command);
}

void internal_commands(char* command, char* parameters[], Queue history, Alias alias) {

    if (find_command(alias, parameters[0]) != NULL) {
        strcpy(command, find_command(alias, parameters[0]));
        process_string(command, parameters);
    }
    
    if (!strcmp(parameters[0], "alias")) {
        assign(command, parameters, alias);
    } else if (!strcmp(parameters[0], "unalias")) {
        unassign(command, parameters, alias);
    } else if (!strcmp(parameters[0], "history")) {
        history_command(command, parameters, history, alias);
    } else if (!strcmp(parameters[0], "exit")) {
        exit_shell(command, parameters);
    } else if (!strcmp(parameters[0], "cd")) {
        cd_command(command, parameters);
    } else if (!strcmp(parameters[0], "cls") || !strcmp(parameters[0], "clear")) {
        clear_screen(command, parameters);
    }
}

void external_commands(char* command, char* parameters[]) {
    int status;

    if (fork() != 0) {
        waitpid(-1, &status, 0);
    } else {
        if (execvp(parameters[0], parameters) == -1) {
            clean_buffer(command, parameters);
            perror("Error");
            _exit(1);
        }
    }
}

void history_command(char* command, char* parameters[], Queue history, Alias alias) {
    int queue_size = size(history);
    char* string = malloc(sizeof(char) * BUFFER_SIZE);

    clean_buffer(command, parameters);

    for (int i = 0; i < queue_size; i++) {
        char* temp_string;
        temp_string = dequeue(history);
        printf("[%d]: %s\n", i + 1, temp_string);
        enqueue(history, temp_string);
    }

    fgets(string, BUFFER_SIZE, stdin);
    if (string[strlen(string) - 1] == '\n')
        string[strlen(string) - 1] = '\0';

    if (string[0] == '!' && string[1] != ' ' && string[1] != '\0') {

        string = strtok(string, "!");
        int index = atoi(string);

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
        enqueue(history, string);
        snprintf(command, BUFFER_SIZE, "%s", string);
        printf("internal command \"%s\" not found\n", string);
    }

    process_string(command, parameters);
    internal_commands(command, parameters, history, alias);
}

void process_string(char* command, char* parameters[]) {
    remove_trailing_newline(command);

    if (is_alias(command)) {
        separate_alias(command, parameters);
    } else {
        separate_parameters(command, parameters);
    }
}

void assign(char* command, char* parameters[], Alias alias) {
    char* name = malloc(sizeof(char) * BUFFER_SIZE);

    strcpy(name, parameters[1]);
    strcpy(command, parameters[2]);

    insert_command(alias, name, command);

    clean_buffer(command, parameters);
}

void unassign(char* command, char* parameters[], Alias alias) {
    remove_command(alias, parameters[1]);

    clean_buffer(command, parameters);
}

void cd_command(char* command, char* parameters[]) {
    if (parameters[1] != NULL) {
        if (!strcmp(parameters[1], "/")){
            chdir(getenv("HOME"));
        } else {
            chdir(parameters[1]);
        }
    } else {
        chdir(getenv("HOME"));
    }

    clean_buffer(command, parameters);
}

void exit_shell(char* command, char* parameters[]) {
    clean_buffer(command, parameters);
    exit(0);
}

void clear_screen(char* command, char* parameters[]) {
    system("clear");
    clean_buffer(command, parameters);
}