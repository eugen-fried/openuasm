#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "io.h"
#include "first_pass.h"


void parse_arguments(int, char**);


int main(int argc, char** argv){
    parse_arguments(argc, argv);
    return 0;
}

void parse_arguments(int argc, char** argv){
    if(argc == 1){
        /* Seems like we don't have arguments */
        printf("%s\n", "There are no asm files to process, exiting.");
        exit(1);
    }
    int c = 0;
    
    for(c = 1; c < argc; c++) {
        if(load_target_file(argv[c])){
            first_pass();
        }
        
    }
    
}
