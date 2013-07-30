#include <stdio.h>
#include <stdbool.h>

extern FILE* target_file;

bool load_target_file(char*);
int save_ob_file(int*, int ic, int dc);