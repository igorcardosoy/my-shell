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