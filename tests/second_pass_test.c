#include "../src/mystring.c"
#include "../src/first_pass.c"
#include "../src/second_pass.c"
#include "../src/symbols.c"
#include <gtest/gtest.h>
#include <string.h>

FILE *target_file;

TEST(SecondPass, set_params) {
    char a[] = "mov/0,1";
    char b[] = "mov/1/0/1,0";
    
    bool type = false, dbl = false;
    int comb = 0;
    
    set_params(&dbl, &type, &comb, a);
    EXPECT_EQ(dbl, true);
    EXPECT_EQ(type, false);
    type = false;
    dbl = false;
    comb = 0;
    
    set_params(&dbl, &type, &comb, b);
    EXPECT_EQ(dbl, false);
    EXPECT_EQ(type, true);
    EXPECT_EQ(comb, 1);
}

TEST(SecondPass, get_operation_word) {
    /*mov/0,0 #21,r2;*/
    Operation oper1;
    oper1.dbl = false;
    oper1.type = false;
    oper1.cmb = 0;
    oper1.opertType = MOV;
    oper1.source_oprnd = 21;
    oper1.source_reg = 0;
    oper1.source_adr = 0;
    oper1.dest_adr = 3;
    oper1.dest_reg = 2;
    
    EXPECT_EQ(get_operation_word(&oper1), 56);
}