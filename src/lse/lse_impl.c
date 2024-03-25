#include "lse.h"

struct alias_commands {
    Node head;
};

struct node {
    char* name;
    char* command;
    Node next;
};

void init_list(Alias* list) {
    *list = malloc(sizeof(struct alias_commands));
    (*list)->head = NULL;
}

void insert_command(Alias list, Typeinfo name, Typeinfo command) {

    if (list != NULL) {
        Node new_node = malloc(sizeof(struct node));

        new_node->next = list->head;
        new_node->name = name;
        new_node->command = command;
        list->head = new_node;
    }
}

Typeinfo find_command(Alias list, Typeinfo name) {
    Typeinfo nodeInfo = NULL;

    if (list != NULL) {
        if (list->head != NULL) {
            for (Node i = list->head; i != NULL && nodeInfo == NULL; i = i->next) {
                if (!strcmp(i->name, name)) {
                    nodeInfo = i->command;
                }
            }
        }
    }



    return nodeInfo;
}

bool remove_command(Alias list, Typeinfo name) {
    bool removed = false;

    if (list != NULL) {
        if (list->head != NULL) {
            Node rip = list->head;
            Node jhonny_walker = NULL;

            if (!strcmp(rip->name, name)) {
                list->head = rip->next;
                free(rip);
                removed = true;
            } else {
                while (rip->next != NULL && !removed) {
                    jhonny_walker = rip;
                    rip = rip->next;

                    if (!strcmp(rip->name, name)) {
                        jhonny_walker->next = rip->next;
                        free(rip);
                        removed = true;
                    }
                }
            }
        }
    }

    return removed;

}

void destroy_list(Alias* list) {
    if (*list != NULL) {
        if ((*list)->head != NULL) {
            Node rip;
            Node jhonny_walker;

            rip = (*list)->head;

            while (rip->next != NULL) {
                jhonny_walker = rip->next;
                free(rip);
                rip = jhonny_walker;
            }
            free(rip);
        }

        free(*list);
        *list = NULL;
    }
}