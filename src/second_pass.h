#include <stdbool.h>
#include "first_pass.h"

void second_pass();
void set_params(bool*, bool*, int*, char*);
int apply_operation(char*);
int add_data_area();
int write_operand(int, int, int);
int get_operation_word(Operation*);
int get_operation(Operation*, char*);
int get_opernds(Operation*, char*);
int get_oprnd_info(int *, int *, int *, int *, char *);
int calc_dist(char *, int *);
int get_symbol_adr(char *, int *);

