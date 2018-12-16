#ifndef TIMETABLE_SCHEMES_H
#define TIMETABLE_SCHEMES_H

#include <stdbool.h>

#include "modules.h"

typedef struct scheme {
    char name[10];
    int year; // year of study
    int students; // number of students
    module_node *modules; // list of modules
} scheme;

typedef struct scheme_node {
    scheme scheme;
    struct scheme_node *next;
} scheme_node;



void init_sch_list(scheme_node **ptr);


bool is_empty_sch(scheme_node* list);


void print_scheme(scheme sch);


void print_scheme_list(scheme_node* node);


/**
 * Appends given element to given normalised list as a last element, thus
 * doesnt change list pointer (to first elem).
 * Use it carefully, it doesnt make copy of modules list!
 * Allocates memory dynamically.
 */
void scheme_append(scheme to_add, scheme_node* list);


/**
 * Returns list of modules that clash with given 'module_name',
 * taught, only at same semester as 'module_name'.
 * Occurences of 'module_name' removed (theoretically it clashes with itself).
 * Returned list must be freed.
 */
module_node* clashing_mods(char *mod_name, scheme_node* list);


void free_schemes(scheme_node *ptr);

#endif //TIMETABLE_SCHEMES_H
