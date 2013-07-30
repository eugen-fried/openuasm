#include "io.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

FILE* target_file = NULL;

bool load_target_file(char* file_name) {
    char* full_file_name = strcat(file_name, ".as");
    target_file = fopen(full_file_name, "r");
    if (target_file == NULL) {
        printf("Could not find file %s, skipping to the next one.\n", full_file_name);
        return false;
    }
    return true;
}

int save_ob_file(int *result, int ic, int dc){
    int length = ic + dc, i;
    /* Open file for writing*/
    for(i = 0x144; i <= length; i++, result++) {
        /* Write in format i         *result*/
    }
    /* Don't forget to close the stream */
}
