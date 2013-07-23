#include "io.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

FILE* target_file = NULL;


bool load_target_file(char* file_name) {
    char* full_file_name = strcat(file_name, ".as");
    target_file = fopen(full_file_name, "r");
    if(target_file == NULL) {
        printf("Could not find file %s, skipping to the next one.\n", full_file_name);
        return false;
    }
    return true;
}
