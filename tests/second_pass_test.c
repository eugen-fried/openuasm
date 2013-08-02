#include "../src/mystring.c"
#include "../src/first_pass.c"
#include "../src/second_pass.c"
#include <gtest/gtest.h>
#include "../lib/queue.h"
#include <string.h>

FILE *target_file;

TEST(SecondPass, set_params) {
    char a[] = "mov/0,1";
    char b[] = "mov/1/0/1,0";
    char c[] = "DATA: .data +7,-57,  17 , 9";
    char d[] = ".entry HELLO";
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

