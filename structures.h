#ifndef TIMETABLE_STRUCTURES_H
#define TIMETABLE_STRUCTURES_H

#include <stdbool.h>

#define NR_DAYS 7
#define FIRST_HOUR 9
#define LAST_HOUR 17


const char *DAY_NAMES[] = { "Monday", "Tuesday", "Wednesday",
                            "Thursday", "Friday", "Saturday", "Sunday" };


enum Days {
    Monday = 0,
    Tuesday = 1,
    Wednesday = 2,
    Thursday = 3,
    Friday = 4,
    Saturday = 5,
    Sunday = 6
};


typedef struct week {
    // tells whether we can use any date
    // we use only [.][9-17] indexes
    bool teaching[NR_DAYS][LAST_HOUR + 1];
} week;


typedef struct week_scheme {
    // here we got built timetable, each hour at each day
    // got own list of hooked modules
    // we use only [.][9-17] indexes
    struct module_node* classess[NR_DAYS][LAST_HOUR + 1];
} week_scheme;


#endif //TIMETABLE_STRUCTURES_H
