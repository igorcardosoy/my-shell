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

void read_command(char*, char**);
void internal_commands(char*, char**, Queue, Alias);
void external_commands(char*, char**);
void history_command(Queue, char*);
void broke_string(char*, char**);
void assign(char**, Alias);
void unassign(char**, Alias);

int main() {
    Queue history;
    Alias alias;

    init_queue(&history);
    init_list(&alias);

    system("clear");

    while (true) {
        char* command = malloc(sizeof(char) * BUFFER_SIZE);
        char* parameters[ARGS_SIZE];
        int status = 0;


        read_command(command, parameters);

        enqueue(history, command);

        internal_commands(command, parameters, history, alias);
        external_commands(command, parameters);
    }

    return 0;
}

void read_command(char* command, char* parameters[]) {
    
    do
    {
        type_prompt();
        fgets(command, BUFFER_SIZE, stdin);
    } while (strcmp(command, " \n") == 0 || strcmp(command, "\n") == 0);

    broke_string(command, parameters);
}

void internal_commands(char* command, char* parameters[], Queue history, Alias alias) {

    if (find_command(alias, parameters[0]) != NULL) {
        char* alias_command = find_command(alias, parameters[0]);
        snprintf(command, BUFFER_SIZE, "%s", alias_command);
        broke_string(command, parameters);
    }

    if (!strcmp(parameters[0], "alias")) {
        assign(parameters, alias);
        clean_buffer(command, parameters);
    } else if (!strcmp(parameters[0], "unalias")) {
        unassign(parameters, alias);
        clean_buffer(command, parameters);
    } else if (!strcmp(parameters[0], "history")) {
        clean_buffer(command, parameters);
        history_command(history, command);
        broke_string(command, parameters);
        internal_commands(command, parameters, history, alias);

    } else if (!strcmp(parameters[0], "exit")) {
        _exit(0);

    } else if (!strcmp(parameters[0], "cd")) {
        if (parameters[1] == NULL) {
            chdir(getenv("HOME"));

        } else {
            chdir(parameters[1]);
        }
        clean_buffer(command, parameters);

    } else if (!strcmp(parameters[0], "cls") || !strcmp(parameters[0], "clear")) {
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
    char* string = malloc(sizeof(char) * BUFFER_SIZE);

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
}

void broke_string(char* command, char* parameters[]) {
    char* token;
    char* copy;
    char* original;
    int i = 1;

    if (command[strlen(command) - 1] == '\n')
        command[strlen(command) - 1] = '\0';

    copy = malloc(sizeof(char) * BUFFER_SIZE);
    snprintf(copy, BUFFER_SIZE, "%s", command);

    original = malloc(sizeof(char) * BUFFER_SIZE);
    snprintf(original, BUFFER_SIZE, "%s", command);

    token = strtok(copy, " ");

    strcpy(copy, token);

    if (!strcmp(copy, "alias")) {

        parameters[0] = malloc(sizeof(char) * BUFFER_SIZE);
        snprintf(parameters[0], BUFFER_SIZE, "%s", copy);

        copy = strtok(NULL, "=");
        parameters[1] = malloc(sizeof(char) * BUFFER_SIZE);
        snprintf(parameters[1], BUFFER_SIZE, "%s", copy);

        original = strtok(NULL, "\"\"");
        parameters[2] = malloc(sizeof(char) * BUFFER_SIZE);
        snprintf(parameters[2], BUFFER_SIZE, "%s", original);

        parameters[3] = NULL;
    } else {
        while (token != NULL) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                parameters[i] = malloc(sizeof(char) * BUFFER_SIZE);
                snprintf(parameters[i], BUFFER_SIZE, "%s", token);
            }

            i++;
        }

        parameters[0] = malloc(sizeof(char) * BUFFER_SIZE);
        snprintf(parameters[0], sizeof(copy), "%s", copy);

        parameters[i - 1] = NULL;
    }
}

void assign(char* parameters[], Alias alias) {
    char* name = malloc(sizeof(char) * BUFFER_SIZE);
    snprintf(name, BUFFER_SIZE, "%s", parameters[1]);

    char* command = malloc(sizeof(char) * BUFFER_SIZE);
    snprintf(command, BUFFER_SIZE, "%s", parameters[2]);
    
    insert_command(alias, name, command);
}

void unassign(char* parameters[], Alias alias) {
    remove_command(alias, parameters[1]);
}