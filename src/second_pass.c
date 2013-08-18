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

/* Second pass around the code*/
void second_pass() {
    ic = 0;
    enum Instr instr;
    char line[100], *linep;
    while (!feof(target_file)) {
        if (!get_line(line)) {
            continue;
        }

        linep = remove_label(line);
        instr = get_instr(linep);
        if (instr != NONE) {
            if (instr == entry || instr == extrn) {
                /* Handle this stuff here*/
            }
            continue;
        }

        apply_operation(linep);

    }
    add_data_area();
    fclose(target_file);
}

/* Concatenate the data area to operations */
int add_data_area() {
    int i;
    for (i = 0; i < dc; i++) {
        opr_area[ic++] = data_area[i];
    }
    return 0;
}

/* Write an operation to binary code */
int apply_operation(char *line) {
    int oper_word;
    Operation opert;
    opert.opertType = get_opert_type(line);
    get_operation(&opert, line);

    oper_word = get_operation_word(&opert);
    opr_area[ic++] = oper_word;
    if (opert.source_adr != 3) {
        write_operand(opert.source_oprnd, opert.source_indx, opert.source_adr);
    }
    if (opert.dest_adr != 3) {
        write_operand(opert.dest_oprnd, opert.dest_indx, opert.dest_adr);
    }
    return 0;
}

/* Write operand to binary code */
int write_operand(int operand, int index, int adr) {
    opr_area[ic++] = operand;
    if (adr == 2) {
        opr_area[ic++] = index;
    }
    return 0;
}

/* Construct the operation byte */
int get_operation_word(Operation *opert) {
    int result = 0;
    result |= opert -> dbl;
    result = result << 1;
    result |= opert -> type;
    result = result << 3;
    result |= opert -> opertType;
    result = result << 2;
    result |= opert -> source_adr;
    result = result << 2;
    result |= opert -> source_reg;
    result = result << 2;
    result |= opert -> dest_adr;
    result = result << 2;
    result |= opert -> dest_reg;
    result = result << 2;
    result |= opert -> cmb;
    return result;

}

/* Gen operation data */
int get_operation(Operation *opert, char *line) {
    if (opert->opertType == RTS || opert->opertType == STOP) {
        set_params(&(opert -> dbl), &(opert -> type), &(opert -> cmb), line);
        return 0;
    }
    
    Split *split;
    split = split_string(line, ' ');
    set_params(&(opert -> dbl), &(opert -> type), &(opert -> cmb), split -> head);
    get_opernds(opert, split -> tail);
    free(split);
}

/* Get operands details */
int get_opernds(Operation *opert, char *line) {
    Split *split;
    if (is_binary_operation(opert -> opertType)) {
        split = split_string(line, ',');
        get_oprnd_info(&(opert -> source_reg),
                &(opert -> source_adr),
                &(opert -> source_oprnd),
                &(opert -> source_indx),
                split -> head);

        get_oprnd_info(&(opert -> dest_reg),
                &(opert -> dest_adr),
                &(opert -> dest_oprnd),
                &(opert -> dest_indx),
                split -> tail);
    }
}

/* Get details for a single operand */
int get_oprnd_info(int *reg, int *adr, int *oprd, int *index, char *expr) {
    char *index_expr;
    char *tmp;
    int error;
    get_single_operand_info(expr, reg, adr);
    if (*adr == 2) {
        *oprd = get_symbol_adr(expr, &error);
        index_expr = get_index_expr(expr, &error);
        if (*reg == -10) {
            if (starts_with_char(index_expr, '#')) {
                tmp = index_expr + 1;
                *index = strtol(tmp, NULL, 0);
            } else {
                *index = calc_dist(index_expr, &error);
            }
        }
    }

    if (*adr == 1) {
        *oprd = get_symbol_adr(expr, &error);
    }

    if (*adr == 0) {
        *oprd = strtol(expr, NULL, 0);
    }

}

/* Calculate distance between a symbol and the current operand*/
int calc_dist(char *symbol, int *error) {
    return ic - get_symbol_adr(symbol, error);
}

/* Get address of a symbol */
int get_symbol_adr(char *symbol, int *error) {
    Symbol *result = get_symbol_by_name(symbol_table, symbol);
    return result -> address;
}

/* Set operation parameters, double, type, comb */
void set_params(bool *dbl, bool *type, int *comb, char* line) {
    char *garbage_line = copy_line(line);
    char *linep = garbage_line;
    char *tok;
    Split *split;

    while (*linep != '/') {
        linep++;
    }
    linep++;

    split = split_string(linep, ',');
    *dbl = strtol(split -> tail, NULL, 0);

    if (strlen(split -> head) == 1 && strtol(split -> head, NULL, 0) == 0) {
        *type = false;
    } else if (strlen(split -> head) > 1) {
        if (strtol(strtok(split -> head, "/"), NULL, 0) == 1) {
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