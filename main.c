#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include "utils.h"
#include "structures.h"
#include "modules.h"
#include "schemes.h"


char *ROOT_DIR_NAME;
ssize_t ROOT_DIR_NAME_LEN;
char *TEMP_BUF;
size_t BUFSIZE = 1000;


char *MODULES = "modules.txt";
char *SCHEMES = "schemes.txt";
char *TIMES   = "times.txt";
char *ROOMS   = "rooms.txt";


// lists of existing modules and schemes, will be initialised with dummy node.
module_node* MOD_LIST = NULL;
scheme_node* SCH_LIST = NULL;


// keeps information about all days and hours whether classess are given then
week WEEK = {0};

week_scheme WEEK_SCHEME = {0};


/**
 * Creates string ROOT_DIRECTORY/'path_end'
 * in TEMP_BUF location, assuming there is already ROOT_DIRECTORY string here.
 */
void join_paths(char *path_end) {
    if (strcmp(ROOT_DIR_NAME + ROOT_DIR_NAME_LEN - 1, "/") == 0) {
        strcpy(TEMP_BUF + ROOT_DIR_NAME_LEN, path_end);
    }
    else {
        strcpy(TEMP_BUF + ROOT_DIR_NAME_LEN, "/");
        strcpy(TEMP_BUF + ROOT_DIR_NAME_LEN + 1, path_end);
    }
}


void open_modules() {
    FILE *fptr;
    join_paths(MODULES);
    fptr = fopen(TEMP_BUF, "r");
    if(fptr == NULL) {
        file_open_error(TEMP_BUF);
    }
    printf("modules properly opened.\n");

    // reading modules
    module new_mod;
    while (fscanf(fptr, "%s %d %dL%d %dP%d",
                  new_mod.name, &new_mod.semester, &new_mod.lect_in_week, &new_mod.lect_length,
                  &new_mod.pract_in_week, &new_mod.pract_length) != EOF) {
        mod_append(new_mod, MOD_LIST);
    }
}


/**
 * Opens schemes file and puts its content to associated structures.
 * It creates lists of modules hooked to any scheme, BUT for optimisation,
 * they keeps only modules names, without details.
 */
void open_schemes() {
    FILE *fptr;
    join_paths(SCHEMES);
    fptr = fopen(TEMP_BUF, "r");
    if(fptr == NULL) {
        file_open_error(TEMP_BUF);
    }
    printf("schemes properly opened.\n");

    // reading schemes
    scheme new_sch;
    module tmp_mod;
    memset(&tmp_mod, '\0', sizeof(tmp_mod));
    int modules;
    while (fscanf(fptr, "%s %d %d", new_sch.name, &new_sch.year, &new_sch.students) != EOF) {
        fscanf(fptr, "%d", &modules); // modules number
        module_node *mods;
        init_mod_list(&mods);
        for (int i = 0; i < modules; i++) {
            fscanf(fptr, "%s", tmp_mod.name);
            mod_append(tmp_mod, mods);
        }
        new_sch.modules = mods;
        scheme_append(new_sch, SCH_LIST);
    }
}


void open_times() {
    FILE *fptr;
    join_paths(TIMES);
    fptr = fopen(TEMP_BUF, "r");
    if(fptr == NULL) {
        file_open_error(TEMP_BUF);
    }
    printf("times properly opened.\n");

    for (int day = Monday; day <= Sunday; day++) {
        char tmp_str[10]; // for day name
        int teach_hours, hour;
        fscanf(fptr, "%9s %2d", tmp_str, &teach_hours);
        while (teach_hours--) {
            fscanf(fptr, "%d", &hour);
            WEEK.teaching[day][hour] = true;
        }
    }
}



/**
 * All the stuff to initialise working environment.
 */
void prepare_env() {
    TEMP_BUF = malloc(BUFSIZE + 10);
    strcpy(TEMP_BUF, ROOT_DIR_NAME);

    // creates dummy list nodes (just for easy implementation).
    init_mod_list(&MOD_LIST);
    init_sch_list(&SCH_LIST);
}


/**
 * Clean things up, frees memory allocated during program execution.
 */
void free_memory() {
    free(ROOT_DIR_NAME);
    free(TEMP_BUF);

    free_modules(MOD_LIST);
    free_schemes(SCH_LIST);
}


void init_timetable() {
    for (int i = Monday; i <= Sunday; ++i) {
        for (int j = FIRST_HOUR; j <= LAST_HOUR; ++j) {
            init_mod_list(&WEEK_SCHEME.classess[i][j]);
        }
    }
}


void clean_timetable() {
    for (int i = Monday; i <= Sunday; ++i) {
        for (int j = FIRST_HOUR; j <= LAST_HOUR; ++j) {
            assert(is_empty_mod(WEEK_SCHEME.classess[i][j]));
            free_modules(WEEK_SCHEME.classess[i][j]);
            WEEK_SCHEME.classess[i][j] = NULL;
        }
    }
}


/**
 * Removes modules taught in given semester.
 */
void remove_from_sem(module_node* list, int sem) {
    bool finish = false;
    while (!finish) {
        module_node *tmp = list->next; // omit dummy
        finish = true;
        while (tmp != NULL) {
            module_node *tmp_real_node = mod_search(tmp->module.name, MOD_LIST);
            assert(tmp_real_node != NULL);
            int tmp_sem = tmp_real_node->module.semester;
            if (tmp_sem == sem) {
                finish = false; // there exists more to delete
                remove_module(list, tmp->module.name);
                break;
            }
            tmp = tmp->next;
        }
    }
}


/**
 * Given exact module name it looks up for modules it clash with,
 * i.e. modules, that have common scheme and are given in same semester
 * and prints them.
 */
void query_module(char *mod_name) {
    module_node *mod = mod_search(mod_name, MOD_LIST);
    if (mod == NULL) {
        printf("Wrong module name!\n");
        return;
    }

    // 'clashing_mods' returns also modules given in another semester,
    // need to filter it.
    module_node* clash_mods;
    clash_mods = clashing_mods(mod_name, SCH_LIST);

    if (mod->module.semester == 1)
        remove_from_sem(clash_mods, 2);
    else if (mod->module.semester == 2)
        remove_from_sem(clash_mods, 1);
    else
        assert(false);

    // and remove 'mod_name' occurences (they exists, it clashes with itself too)
    remove_module(clash_mods, mod_name);

    print_module(mod->module);
    printf("Modules clashing with '%s':\n", mod_name);
    if (is_empty_mod(clash_mods))
        printf("None.\n");
    else
        print_module_list_trimmed(clash_mods);

    free_modules(clash_mods);
}


void print_header() {
    printf("%-15s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s\n",
            "Day",   "9","10","11","12","13","14","15","16","17");
}

void print_parting() {
    for (int i = 0; i < 15 + (9 * 10); ++i) {
        putchar('=');
    }
    putchar('\n');
}


/**
 * Tells whether there is something left to be printed
 * in exact day.
 */
bool sth_to_print_left(enum Days day) {
    for (int hour = FIRST_HOUR; hour <= LAST_HOUR; hour++) {
        if (!is_empty_mod(WEEK_SCHEME.classess[day][hour])) {
            return true;
        }
    }
    return false;
}


/**
 * Print row and (!) pops printed elements from lists.
 * Each element once printed is removed.
 */
void print_row(enum Days day) {
    printf("%-15s", DAY_NAMES[day]);
    for (int hour = FIRST_HOUR; hour <= LAST_HOUR; hour++) {
        module_node *act_list = WEEK_SCHEME.classess[day][hour];
        if (!is_empty_mod(act_list)) {
            print_module_trimmed(act_list->next->module);
            pop_front(act_list);
        } else {
            printf("%-10s", ""); // empty cell
        }
    }
    printf("\n");
}

/**
 * Prints whole timetable. To adjust to expected table format,
 * it do it row by row, deleting already printed elements.
 */
void print_timetable() {
    print_parting();
    print_header();
    print_parting();
    for (int day = Monday; day <= Sunday; day++) {
        bool sth_printed = false;
        while (sth_to_print_left(day)) {
            sth_printed = true;
            print_row(day);
        }
        if (!sth_printed) {
            // put row only with dayname, with empty cells
            print_row(day);
        }
        print_parting();
    }
}


/**
 * Finds proper place in timetable for given module
 * and inserts it here. Returns false if cannot find
 * such place.
 * For optimisation, clashing modules are passed as argument
 * instead of counting them all the time (any module occurs
 * <number of its lectures + practices> times).
 */
bool find_proper_date(char *mod_name, enum Type mod_type, module_node *clashing_mods) {
    for (int i = Monday; i <= Sunday; ++i) {
        for (int j = FIRST_HOUR; j <= LAST_HOUR; ++j) {
            if (WEEK.teaching[i][j]) {
                // there must be teaching allowed that time

                if (mod_any_in(clashing_mods, WEEK_SCHEME.classess[i][j])) {
                    // if any clashing module already is in place,
                    // we cant place it here
                }
                else {
                    module new;
                    strcpy(new.name, mod_name);
                    new.class_type = mod_type;

                    // to print properly we need only name and type
                    mod_append(new, WEEK_SCHEME.classess[i][j]);

                    return true;
                }
            }
        }
    }

    return false;
}

bool belongs_to_any_scheme(char *mod_name) {
    scheme_node *tmp_sch = SCH_LIST->next;
    while (tmp_sch != NULL) {
        if (mod_search(mod_name, tmp_sch->scheme.modules) != NULL)
            return true;
        tmp_sch = tmp_sch->next;
    }

    return false;
}


/**
 * Fills table of proposed week scheme, holding on rule
 * that there can't be two clashing modules in any cell.
 */
void create_week_scheme(int sem) {
    module_node *tmp = MOD_LIST->next;

    while (tmp != NULL) {
        // in table put only modules from given semester, and hooked
        // to at least one scheme
        if (!belongs_to_any_scheme(tmp->module.name) || tmp->module.semester != sem) {
            tmp = tmp->next;
            continue;
        }

        int lects = tmp->module.lect_in_week;
        int practs = tmp->module.pract_in_week;

        module_node *clashings = clashing_mods(tmp->module.name, SCH_LIST);

        // look for place for lectures
        while (lects--) {
            if (!find_proper_date(tmp->module.name, Lecture, clashings)) {
                // cannot find place to any module
                printf("WARNING! -----  Cannot fit all the modules! ------\n");
                return;
            }
        }

        // look for place for practices
        while (practs--) {
            if (!find_proper_date(tmp->module.name, Practical, clashings)) {
                // cannot find place to any module
                printf("WARNING! -----  Cannot fit all the modules! ------\n");
                return;
            }
        }

        free_modules(clashings);

        tmp = tmp->next;
    }
}

void create_table() {
    int sem;
    char tmp_sem[3];
    printf("What semestr are You interested in?\n");
    scanf("%s", &tmp_sem);
    sem = atoi(tmp_sem);
    if (sem != 1 && sem != 2) {
        printf("Only 1 or 2 matches.\n");
        return;
    }

    init_timetable();
    create_week_scheme(sem);
    print_timetable();
    clean_timetable();
}


int main() {
    printf("Week Timetable\n");
    printf("Enter directory with data files name:\n");
    ROOT_DIR_NAME = malloc(BUFSIZE);
    scanf("%s", ROOT_DIR_NAME);
    ROOT_DIR_NAME_LEN = strlen(ROOT_DIR_NAME) + 1;
    //getline allocates some memory dynamically, need to free it
    if (ROOT_DIR_NAME_LEN < 0) {
        unspecified_error();
    }

    // we must get rid of last character, which is new line, and thus decrease line length
    ROOT_DIR_NAME[--ROOT_DIR_NAME_LEN] = '\0';

    prepare_env();

    open_modules();
    open_schemes();
    open_times();

    // main user interface action
    bool exit = false;
    char tmp_action[3];

    enum Action {
        QUERY_MODULE = 1,
        CREATE_TABLE = 2,
        EXIT = 3,
    };

    while (!exit) {
        int action;
        print_parting();
        printf("Choose action:\n");
        printf("1 = query module, 2 = create timetable, 3 = exit:\n");

        scanf("%2s", tmp_action);
        action = atoi(tmp_action);

        char name[MOD_NAME_SIZE];
        switch (action) {
            case QUERY_MODULE:
                printf("Enter module name:\n");
                scanf("%s", name);
                query_module(name);
                break;
            case CREATE_TABLE:
                create_table();
                break;
            case EXIT:
                exit = true;
                break;
            default:
                printf("Unknown command, try again!\n");
                break;
        }
    }

    free_memory();
    return 0;
}
