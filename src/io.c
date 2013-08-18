#include "io.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

FILE* target_file = NULL;

/* Load the as code file */
bool load_target_file(char* file_name) {
    char* full_file_name = strcat(file_name, ".as");
    target_file = fopen(full_file_name, "r");
    if (target_file == NULL) {
        printf("Could not find file %s, skipping to the next one.\n", full_file_name);
        return false;
    }
    return true;
}

int save_ob_file(char* file_name) {
    char* out_file_name = malloc(sizeof(char) * (strlen(file_name) + 3));
    strcat(out_file_name, ".ob");
    FILE *out = fopen(out_file_name, "w");
    fclose(out);
    return 0;
}