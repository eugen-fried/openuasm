/* 
 * File:   first_pass.c
 * Author: evgeny
 *
 * Created on July 4, 2013, 7:15 PM
 */

#include "first_pass.h"

int data_area[2000] = {};
int opr_area[2000] = {};
int ic = 0, dc = 0;
SymbolTable symbol_table;

/*
 * 
 */
void first_pass() {
    TAILQ_INIT(&(symbol_table.head));
    bool is_label;
    Symbol symbol;
    enum Instr instr;
    char line[100], *linep;
    while (!feof(target_file)) {
        fscanf(target_file, "%s", line);
        /*Trim whitespace*/
        trim_whitespace(line);

        /* Should we process the line? */
        if (is_meaningless_line(line)) {
            continue;
        }
        /* Does this line has a label? (maybe replace it with inner function call)*/
        if (has_label(line)) {
            is_label = true;
        }

        /* Get instruction from line (if exists)*/
        instr = get_instr(line, is_label);
        /* Handle data */
        if (instr == data || instr == string) {
            if (is_label) {
                add_symbol(get_label_name(line), dc++, false, false);
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
                add_symbol(get_symbol_name(line), 0, true, false);
            }
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

bool validate_line(char *line, int line_num) {
    char * first_tok;
    first_tok = strtok(line, " ");
    if (first_tok == NULL) {
        notify_error("There are no recognizable tokens on this line, consult your assembler syntax user guide.", line_num);
        return false;
    }
    return true;
}

void notify_error(char *msg, int line_num) {
    printf("You have an error on line %d:\n", line_num);
    printf("%s\n", msg);
}

enum Instr get_instr(char *line, bool symbol) {
    char *instr_tok, *garbage_line = copy_line(line);

    /*If has symbol, look in the second token*/
    if (symbol) {
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
    return none;

}

enum Action get_action(char *line) {
    char *tok;
    enum Action result;
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
        result =  NONE;
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
        data_area[++dc] = tmp;
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