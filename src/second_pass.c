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

Symbol *symbol_table[HASH_TAB_SIZE];

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
    int cmb, dest_reg, dest_adr, source_reg, source_adr;
    enum OpertType oper = get_opert_type(line);
    
    if(is_binary_operation(oper)) {
        line = remove_before_space(line);
        if(strchr(line, ',') != NULL) {
            
        }
    }
    
    set_params(&dbl, &type, &cmb, line);
    
}

int get_operation(Operation *opert, char *line) {
    Split *split;
    split = split_string(line, ' ');
    set_params(&(opert -> dbl), &(opert -> type), &(opert -> cmb), split -> head);
    get_opernds(opert, split -> tail);
    free(split);
}

int get_opernds(Operation *opert, char *line) {
    Split *split;
    split_string(line, ',');
    if(is_binary_operation(opert -> opertType)) {
        split = split_string(line, ",");
        get_oprnd_info(&(opert -> source_reg), 
                &(opert -> source_adr),
                &(opert -> source_oprnd),
                &(opert -> source_indx),
                split -> head);
    }
}

int get_oprnd_info(int *reg, int *adr, int *oprd, int *index, char *expr) {
    char *index_expr;
    char *tmp;
    int error;
    get_single_operand_info(expr, reg, adr);
    if(adr == 2) {
        oprd = get_symbol_adr(expr, &error);
        index_expr = get_index_expr(expr, error);
        if(reg == -10) {
            if(starts_with_char(index_expr, '#')) {
                tmp = index_expr + 1;
                *index = strtol(tmp, NULL, 0);
            } else {
                *index = calc_dist(tmp, &error);
            }
        }
    }
    
    if(adr == 1) {
        oprd = get_symbol_adr(expr, &error);
    }
    
    if(adr == 0) {
        oprd = strtol(expr, NULL, 0);
    }
    
}

/* Calculate distance between a symbol and the current operand*/
int calc_dist(char *symbol, int *error) {
    return ic - get_symbol_adr(symbol, error);
}

/* Get address of a symbol */
int get_symbol_adr(char *symbol, int *error) {
    Symbol *result = get_symbol_by_name(symbol, symbol_table);
    return result -> address;
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
    free(split);
    free(garbage_line);
}