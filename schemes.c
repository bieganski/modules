#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "schemes.h"
#include "modules.h"


void init_sch_list(scheme_node **ptr) {
    scheme_node *sch_dummy = malloc(sizeof(scheme_node));
    memset(sch_dummy, '\0', sizeof(scheme_node));
    sch_dummy->next = NULL; // just for sure
    strcpy(sch_dummy->scheme.name, "DUMMY_SCH");
    init_mod_list(&sch_dummy->scheme.modules);
    *ptr = sch_dummy;
}


bool is_empty_sch(scheme_node* list) {
    return list->next == NULL; // dummy exists
}


void print_scheme(scheme sch) {
    printf("scheme '%s', year: '%d', students: '%d' modules: \n", sch.name, sch.year, sch.students);
    print_module_list(sch.modules);
    printf("end of modules list of scheme %s\n", sch.name);
}


void print_scheme_list(scheme_node* node) {
    while (node != NULL) {
        print_scheme(node->scheme);
        node = node->next;
    }
}


/**
 * Returns list of modules that clash with given 'module_name',
 * Warning: Returned list contains of modules tauhght in both semesters,
 * modules from another semester are omitted in smarter way.
 * Occurences of 'module_name' ARE NOT removed (it clashes with itself).
 * Returned list must be freed.
 */
module_node* clashing_mods(char *mod_name, scheme_node* list) {
    module_node *res, *found;
    init_mod_list(&res);
    while (list != NULL) {
        found = mod_search(mod_name, list->scheme.modules);
        if (found != NULL) {
            // that scheme contains seeked module,
            // lets add all clashing modules from that scheme
            // to our 'res' list
            module_node *mods =  list->scheme.modules; // 'mod_name' belongs to 'mods'
            mods = mods->next; // omit dummy
            while (mods != NULL) {
                mod_append(mods->module, res);
                mods = mods->next;
            }
        }
        list = list->next;
    }
    return res;
}


/**
 * Appends given element to given normalised list as a last element, thus
 * doesnt change list pointer (to first elem).
 * Allocates memory dynamically.
 */
void scheme_append(scheme to_add, scheme_node* list) {
    scheme_node *tmp_ptr = list;
    while (tmp_ptr->next != NULL) {
        tmp_ptr = tmp_ptr->next;
    }
    scheme_node *new_node = malloc(sizeof(scheme_node));
    tmp_ptr->next = new_node;
    new_node->scheme = to_add;
    new_node->next = NULL;
}


void free_schemes(scheme_node *ptr) {
    if (ptr->next != NULL)
        free_schemes(ptr->next);
    free_modules(ptr->scheme.modules);
    free(ptr);
}
