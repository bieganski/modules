#include <stdio.h>
#include <stdlib.h>

void unspecified_error() {
    printf("Unspecified error occured. Aborting.");
    exit(1);
}

void file_format_error(char *path, int line_nr) {
    printf("Wrong file '%s' format in line %d. Aborting", path, line_nr);
    exit(1);
}

void file_open_error(char *path) {
    printf("Cannot open file '%s'. Aborting.", path);
    exit(1);
}
