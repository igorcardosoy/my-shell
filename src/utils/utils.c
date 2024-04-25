#include "utils.h"

void type_prompt() {
    struct tm* time = get_time();
    printf("%s@%s[%02d:%02d:%02d] /%s $ ", get_username(), get_hostname(), time->tm_hour, time->tm_min, time->tm_sec, get_directory());
}

void clean_buffer(char* command, char* parameters[]) {
    command = NULL;

    for (int i = 0; parameters[i] != NULL; i++) {
        parameters[i] = NULL;
    }
}

void remove_trailing_newline(char* command) {
    if (command[strlen(command) - 1] == '\n')
        command[strlen(command) - 1] = '\0';
}

void separate_parameters(char* command, char** parameters) {
    int i = 1;
    char* token;

    token = strtok(command, " ");

    while (token != NULL) {
        token = strtok(NULL, " ");
        if (token != NULL) {
            parameters[i] = malloc(sizeof(char) * BUFFER_SIZE);
            strcpy(parameters[i], token);
        }

        i++;
    }

    parameters[0] = malloc(sizeof(char) * BUFFER_SIZE);
    strcpy(parameters[0], command);

    if (i > 1) {
        parameters[i - 1] = NULL;
    }
}

bool is_alias(char* command) {
    char* token;
    char copy[BUFFER_SIZE];

    strcpy(copy, command);
    token = strtok(copy, " ");

    if (token != NULL) {
        if (strcmp(token, "alias") == 0) return true;
    }

    return false;
}

void separate_alias(char* command, char** parameters) {
    char* token;
    char* copy = malloc(sizeof(char) * BUFFER_SIZE);
    char* original = malloc(sizeof(char) * BUFFER_SIZE);

    for (int i = 0; i < 3; i++)
        parameters[i] = malloc(sizeof(char) * BUFFER_SIZE);
    
    strcpy(copy, command);
    strcpy(original, command);

    token = strtok(copy, " ");
    strcpy(copy, token);
    strcpy(parameters[0], copy);

    copy = strtok(NULL, "=");

    while (copy[0] == ' ') {
        copy++;
    }
    strcpy(parameters[1], copy);

    original = strtok(original, "\"\"");
    original = strtok(NULL, "\"\"");
    strcpy(parameters[2], original);

    parameters[3] = NULL;
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