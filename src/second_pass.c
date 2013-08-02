/* 
 * File:   second_pass.c
 * Author: evgeny
 *
 * Created on July 23, 2013, 9:18 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "second_pass.h"
#include "first_pass.h"

/*
 * 
 */
void second_pass() {
    enum Instr instr;
    char line[100], *linep;
    while (!feof(target_file)) {
        if (!get_line(line)) {
            continue;
        }

        remove_label(line);
        instr = get_instr(line);
        if (instr != NONE) {
            if (instr == entry || instr == extrn) {
                /* Handle this stuff here*/
            }
            continue;
        }
        
        apply_operation(line);


    }
}

int apply_operation(char *line) {
    bool dbl, type;
    int cmb;
    enum Operation oper = get_operation(line);
    
    set_params(&dbl, &type, &cmb, line);
    
}

void set_params(bool *dbl, bool *type, int *comb, char* line) {
    char *garbage_line = copy_line(line);
    char *linep = garbage_line;
    char *tok;
    Split *split;
    
    while(*linep != '/') {
        linep++;
    }
    linep++;
    
    split = split_string(linep, ',');
    *dbl = strtol(split -> tail, NULL, 0);
    
    if(strlen(split -> head) == 1 && strtol(split -> head, NULL, 0) == 0) {
        *type = false;
    } else if(strlen(split -> head) > 1) {
        if(strtol(strtok(split -> head, "/"), NULL, 0) == 1) {
            *type = true;
            tok = strtok(NULL, "/");
            *comb = strtol(tok, NULL, 0);
            *comb = *comb << 1;
            tok = strtok(NULL, "/");
            *comb = *comb | strtol(tok, NULL, 0);
        }
    }
    
}