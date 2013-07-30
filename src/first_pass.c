/* 
 * File:   first_pass.c
 * Author: evgeny
 *
 * Created on July 4, 2013, 7:15 PM
 */

#include "first_pass.h"

int data_area[2000] = {};
int opr_area[2000] = {};
int ic = 100, dc = 0;
SymbolTable symbol_table;

/*
 * 
 */
void first_pass() {
    TAILQ_INIT(&(symbol_table.head));
    int error = 0, line_num = 0;
    
    char line[100], *linep;
    while (!feof(target_file)) {
        
        fscanf(target_file, "%s", line);
        /*Trim whitespace*/
        trim_whitespace(line);

        /* Should we process the line? */
        if (is_meaningless_line(line)) {
            continue;
        }
        
        error = handle_instr(line);
        if(error == 0) {
            continue;
        } else if (error > 0) {
            handle_error(error, line);
        }

        error = handle_operation(line);
        if (error == 0) {
            continue;
        } else if (error > 0) {
            handle_error(error, line);
        }
        
        line_num++;
    }
}

int handle_operation(char *line) {
    if (get_operation(line) != NONE) {
        add_symbol(get_label_name(line), ic, false, false);
        ic += calc_code_length(line);
    }
}

int handle_instr(char *line) {
    enum Instr instr;
    /* Get instruction from line (if exists)*/
    instr = get_instr(line);
    /* Handle data */
    if (instr == data || instr == string) {
        if (has_label(line)) {
            add_symbol(get_label_name(line), dc, false, true);
        }
        if (instr == string) {
            handle_string_instr(line);
        } else if (instr == data) {
            handle_data_instr(line);
        }
    } else if (instr == entry || instr == extrn) {
        /*Handle extern and entry instructions*/
        if (extrn) {
            add_symbol(get_symbol_name(line), 0, true, false);
        } else {
            add_symbol(get_symbol_name(line), 0, true, true);
        }
    }
}

bool is_meaningless_line(char *line) {
    /*Check for empty string*/
    if (is_empty_string(line)) {
        return true;
    }

    /*Is it a comment?*/
    if (starts_with_char(line, ';')) {
        return true;
    }
    return false;
}

bool has_label(char *line) {
    if (get_label_name(line) == NULL) {
        return false;
    }
    return true;
}

void notify_error(char *msg, int line_num) {
    printf("You have an error on line %d:\n", line_num);
    printf("%s\n", msg);
}

int get_instr(char *line) {
    char *instr_tok, *garbage_line = copy_line(line);

    /*If has symbol, look in the second token*/
    if (has_label(line)) {
        strtok(garbage_line, " ");
        instr_tok = strtok(NULL, " ");
    } else {
        instr_tok = strtok(garbage_line, " ");
    }

    /* Return appropriate pseudo instruction. Not the prettiest memory operation, but we write C stuff :-/ */
    if (strcmp(instr_tok, ".data") == 0) {
        free(garbage_line);
        return data;
    } else if (strcmp(instr_tok, ".string") == 0) {
        free(garbage_line);
        return string;
    } else if (strcmp(instr_tok, ".entry") == 0) {
        free(garbage_line);
        return entry;
    } else if (strcmp(instr_tok, ".extern") == 0) {
        free(garbage_line);
        return extrn;
    }

    /* Seems like there is no instruction*/
    free(garbage_line);
    return NONE;

}

int calc_code_length(char *line) {
    enum Operation opert = get_operation(line);

    if (opert == RTS || opert == STOP) {
        return 1;
    }

    bool binary = (opert == MOV || opert == CMP ||
            opert == ADD || opert == SUB ||
            opert == LEA);

    if (binary) {
        return get_binary_length(line);
    }

    line = remove_before_space(line);
    return 1 + get_single_operand_length(line);


}

int get_binary_length(char *line) {
    line = remove_before_space(line);
    int result = 1;
    Split *split = split_string(line, ',');

    result += get_single_operand_length(split -> head);
    result += get_single_operand_length(split -> tail);

    free(split);
    return result;
}

/*How many steps we should advance the IC?*/
int get_single_operand_length(char *oper) {
    oper = trim_whitespace(oper);
    /* Is it a register? */
    if (get_register_code(oper) != INVALID) {
        return 0;
    }

    /* ...or maybe immediate number? */
    if (starts_with_char(oper, '#')) {
        return 1;
    }
    /* so it should be an index call! */
    int index_type = get_index_type(oper);
    if (index_type == REGISTER) {
        return 1;
    }
    if (index_type == REFERENCE || index_type == IMMEDIATE) {
        return 2;
    }

    return 1;
}

int get_index_type(char *oper) {
    int error = 0;
    char *index_expr = get_index_expr(oper, &error);

    if (error == NONE || error == INVALID) {
        return error;
    }

    if (get_register_code(index_expr) != INVALID) {
        return REGISTER;
    }

    if (isdigit(index_expr)) {
        return IMMEDIATE;
    }

    return REFERENCE;

}

/* Parse register code from the operand. Returns INVALID if it's not a register. */
int get_register_code(char *oper) {
    char *copy, *copyp;
    int result = INVALID;

    /* We want to be space tolerant, so copy the string and trim spaces*/
    copy = (char *) malloc(strlen(oper) * sizeof (char));
    strcpy(copy, oper);
    copyp = copy;
    copy = trim_whitespace(copy);

    int reg;
    if (strlen(copy) != 2 || !starts_with_char(copy, 'r') || !isdigit(*(copy + 1))) {
        free(copyp);
        return INVALID;
    }

    reg = strtol((copy + 1), NULL, 0);
    if (reg >= 0 && reg <= 7) {
        result = reg;
    }

    free(copyp);
    return result;
}

/* We want to analyze the index expression (inside { })*/
char *get_index_expr(char *oper, int *error) {
    char *tmp = oper, *result, *index_expr;
    bool started = false;
    bool correct = false;
    int size = 0;

    while (*tmp != '\0') {
        if (!started) {
            if (*tmp == '{') {
                started = true;
                index_expr = tmp + 1;
            }
        } else {
            /* We don't want to pass smth like {{ax}*/
            if (*tmp == '{') {
                *error = INVALID;
                break;
            }

            size++;
            if (*tmp == '}') {
                correct = true;
                started = false;
                size--;
            }
        }

        /* another pair? It's too much for us. */
        if (correct && started) {
            *error = INVALID;
            break;
        }
        tmp++;
    }

    if (!started && !correct) {
        *error = NONE;
        return index_expr;
    }

    result = (char *) malloc((size + 1) * sizeof (char));
    strlcpy(result, index_expr, size + 1);
    return result;
}

int get_operation(char *line) {
    char *tok;
    int result;
    Split *split;
    line = remove_label(line);

    split = split_string(line, ' ');
    tok = split -> head;


    /* Could be replaced with some preprocessor magic, I do it if I'll have time */
    if (strcmp(tok, "mov") == 0) {
        result = MOV;
    } else if (strcmp(tok, "cmp") == 0) {
        result = CMP;
    } else if (strcmp(tok, "add") == 0) {
        result = ADD;
    } else if (strcmp(tok, "sub") == 0) {
        result = SUB;
    } else if (strcmp(tok, "not") == 0) {
        result = NOT;
    } else if (strcmp(tok, "clr") == 0) {
        result = CLR;
    } else if (strcmp(tok, "lea") == 0) {
        result = LEA;
    } else if (strcmp(tok, "inc") == 0) {
        result = INC;
    } else if (strcmp(tok, "dec") == 0) {
        result = DEC;
    } else if (strcmp(tok, "jmp") == 0) {
        result = JMP;
    } else if (strcmp(tok, "bne") == 0) {
        result = BNE;
    } else if (strcmp(tok, "red") == 0) {
        result = RED;
    } else if (strcmp(tok, "prn") == 0) {
        result = PRN;
    } else if (strcmp(tok, "jsr") == 0) {
        result = JSR;
    } else if (strcmp(tok, "rts") == 0) {
        result = RTS;
    } else if (strcmp(tok, "stop") == 0) {
        result = STOP;
    } else {
        result = NONE;
    }
    free(split);
    return result;
}

/*Damned strtok string modification, maybe I should've written my own tokenizer?*/
char *copy_line(char *line) {
    char *result;
    result = (char *) malloc(MAX_LINE_SIZE * sizeof (char));
    strcpy(result, line);
    return result;
}

/* Returns the label name, see tests for examples */
char *get_label_name(char* line) {
    char * first_tok, *garbage_line, *result;
    garbage_line = copy_line(line);

    /* Split by spaces */
    first_tok = strtok(garbage_line, " ");
    /* Do we have a colon, that indicates a label? */
    if (first_tok[strlen(first_tok) - 1] == ':') {
        /* Remove the colon */
        first_tok[strlen(first_tok) - 1] = '\0';
        result = (char *) malloc(strlen(first_tok) + 1);
        strcpy(result, first_tok);
        /* Don't want memory leaks in my house */
        free(garbage_line);
        return result;
    }
    free(garbage_line);
    return NULL;
}

/* Returns the symbol name for .extern and .entry instructions.
 * Do not use it in other cases. */
char *get_symbol_name(char *line) {
    line = remove_label(line);
    char *sec_tok, *garbage_line, *result;
    garbage_line = copy_line(line);
    strtok(garbage_line, " ");
    sec_tok = strtok(NULL, " ");
    if (sec_tok != NULL) {
        result = (char *) malloc(strlen(sec_tok) + 1);
        strcpy(result, sec_tok);
        free(garbage_line);
        return result;
    }
    free(garbage_line);
    return NULL;
}

char *remove_label(char *line) {
    if (has_label(line)) {
        line = strchr(line, ':');
        line += 2;
    }
    return line;
}

bool add_symbol(char *name, int adress, bool is_extrn, bool has_inst) {
    Symbol *symbol = (Symbol *) malloc(sizeof (Symbol));
    symbol->name = name;
    symbol->address = adress;
    symbol->is_extern = is_extrn;
    symbol->has_inst = has_inst;
    TAILQ_INSERT_TAIL(&(symbol_table.head), symbol, pointers);
    return true;
}

void handle_string_instr(char *line) {
    char* data = get_string_data(line);
    if (data == NULL) {
        return;
    }

    char curr_char = 0;
    do {
        curr_char = *data;
        data_area[dc++] = curr_char;
    } while (curr_char != 0);
    free(data);
}

char *get_string_data(char *line) {
    line = remove_label(line);
    char *sec_tok, *result;
    Split *split;
    split = split_string(line, ' ');

    sec_tok = split -> tail;

    if (starts_with_char(sec_tok, '\"') && ends_with_char(sec_tok, '\"')) {
        result = (char *) malloc(strlen(sec_tok) + 1);
        strcpy(result, sec_tok);
        free(split);
        result++;
        *(result + (strlen(result) - 1)) = '\0';
        return result;
    }
    free(split);
    return NULL;
}

void handle_data_instr(char *line) {
    int org_dc = dc, tmp;
    Split *split;
    line = remove_label(line);
    split = split_string(line, ' ');
    char *tok = strtok(split -> tail, ",");

    while (tok != NULL) {
        tmp = strtol(tok, NULL, 0);
        data_area[dc++] = tmp;
        strtok(NULL, ",");
    }
}

/* If something went wrong (invalid data) we need the ability to rollback to
 * previous state. */
void rollback_data(int original_dc) {
    for (; dc > original_dc; dc--) {
        data_area[dc] = 0;
    }
}

void handle_error(int error, char *line) {
    /*Do something with the error...*/
}