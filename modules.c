#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>

#include "modules.h"


void init_mod_list(module_node **ptr) {
    module_node *mod_dummy = malloc(sizeof(module_node));
    memset(mod_dummy, '\0', sizeof(module_node));
    mod_dummy->next = NULL; // just for sure
    strcpy(mod_dummy->module.name, "DUMMY_MOD");
    *ptr = mod_dummy;
}


bool is_empty_mod(module_node* list) {
    return list->next == NULL; // dummy exists
}


/**
 * Appends given element to given normalised list as a last element, thus
 * doesnt change list pointer (to first elem).
 * Allocates memory dynamically.
 */
void mod_append(module to_add, module_node* list) {
    module_node *tmp_ptr = list;
    while (tmp_ptr->next != NULL) {
        tmp_ptr = tmp_ptr->next;
    }
    module_node *new_node = malloc(sizeof(module_node));
    tmp_ptr->next = new_node;
    new_node->module = to_add;
    new_node->next = NULL;
}


/**
 * Searches for given module name in given list.
 * Returns pointer to found or NULL pointer if none exists.
 */
module_node* mod_search(char *name, module_node *list) {
    module_node *res = NULL;
    while (list != NULL) {
        if (strcmp(list->module.name, name) == 0) {
            res = list;
            break;
        }
        list = list->next;
    }
    return res;
}


void print_module(module mod) {
    printf("Module %s, semester: %d\nLectures: %d, Practicals: %d\n",
           mod.name, mod.semester,
           mod.lect_in_week,
           mod.pract_in_week);
}


void print_module_list(module_node *node) {
    while (node != NULL) {
        print_module(node->module);
        node = node->next;
    }
}


/**
 * Pops first element from list, safely frees memory.
 * If list was empty (ahd dummy node only), function has no action.
 */
void pop_front(module_node* list) {
    if(is_empty_mod(list))
        return;
    module_node* new_first = list->next->next; // may be null
    free(list->next);
    list->next = new_first;
}


void print_module_trimmed(module mod) {
    char type[2];
    char res[2 + MOD_NAME_SIZE];

    if (strcmp(mod.name, "DUMMY_MOD") == 0) {
        printf("%-10s", "");
        return ;
    }

    switch (mod.class_type) {
        case Practical:
            strcpy(type, "P\0");
            break;
        case Lecture:
            strcpy(type, "L\0");
            break;
        default:
            strcpy(type, "\0");
            break;
    }
    strcpy(res, mod.name);
    strcat(res, type);
    printf("%-10s", res);
}


void print_module_list_trimmed(module_node *node) {
    while (node != NULL) {
        if (strcmp(node->module.name, "DUMMY_MOD") != 0) {
            printf("%s", node->module.name);
            if (strcmp(node->module.name, "DUMMY_MOD") != 0)
                printf("\n");
        }
        node = node->next;
    }
}

/**
 * Returns copy of first module on given list.
 * List cannot be empty.
 */
module mod_front(module_node* list) {
    assert(!is_empty_mod(list));
    return list->next->module;
}

/**
 * Removes element with given name from list,
 * no action if there is none here.
 */
void remove_module(module_node* list, char *name) {
    if (mod_search(name, list) == NULL) {
        return;
    }
    while (strcmp(list->next->module.name, name) != 0)
        list = list->next;
    module_node *next = list->next->next; // may be null
    free(list->next);
    list->next = next;
}


/**
 * Checks whether any of 'l1' elements belongs to 'l2'.
 */
bool mod_any_in(module_node *l1, module_node *l2) {
    l1 = l1->next; // omit dummy
    while (l1 != NULL) {
        if (mod_search(l1->module.name, l2) != NULL)
            return true;
        l1 = l1->next;
    }
    return false;
}


void free_modules(module_node* ptr) {
    if (ptr->next != NULL)
        free_modules(ptr->next);
    free(ptr);
}
