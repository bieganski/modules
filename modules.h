#ifndef TIMETABLE_MODULES_H
#define TIMETABLE_MODULES_H

#define MOD_NAME_SIZE 9

#include <stdlib.h>
#include <stdbool.h>

enum Type {
    Lecture = 1,
    Practical = 2,
};

typedef struct module {
    char name[MOD_NAME_SIZE + 1]; // +1 because of null character
    int semester; // 1 or 2
    int lect_in_week; // number of lecture in week
    int lect_length; // lecture length in hours
    int pract_in_week; // number of practical in week
    int pract_length; // practical length in hours
    enum Type class_type; // used only by timetable, to distinguish class type
} module;


/**
 * For implementation reasons, lists should be represented with
 * first element being dummy, for instance empty list contains exactly one,
 * dummy element.
 */

typedef struct module_node {
    struct module module;
    struct module_node *next;
} module_node;



void init_mod_list(module_node **ptr);


/**
 * Appends given element to given normalised list as a last element, thus
 * doesnt change list pointer (to first elem).
 * Allocates memory dynamically.
 */
void mod_append(module to_add, module_node* list);


bool is_empty_mod(module_node* list);

/**
 * Searches for given module name in given list.
 * Returns pointer to found or NULL pointer if none exists.
 */
module_node* mod_search(char *name, module_node *list);


/**
 * Pops first element from list, safely frees memory.
 * If list was empty (ahd dummy node only), function has no action.
 */
void pop_front(module_node* list);


/**
 * Returns copy of first module on given list.
 * List cannot be empty.
 */
module mod_front(module_node* list);


void print_module(module mod);


void print_module_trimmed(module mod);


void print_module_list(module_node *node);


void print_module_list_trimmed(module_node *node);


void remove_module(module_node* list, char *name);


/**
 * Checks whether any of 'l1' elements belongs to 'l2'.
 */
bool mod_any_in(module_node *l1, module_node *l2);


void free_modules(module_node* ptr);


#endif //TIMETABLE_MODULES_H
