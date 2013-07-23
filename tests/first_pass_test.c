#include "../src/mystring.c"
#include "../src/first_pass.c"
#include <gtest/gtest.h>
#include "../lib/queue.h"
#include <string.h>


FILE *target_file;


TEST(FirstPass, get_label_name) {
    char a[] = "NAME: .string \"Linus\"";
    char b[] = ".data +7,-57,  17 , 9";
    char c[] = "DATA: .data +7,-57,  17 , 9";
    char d[] = ".entry HELLO";

    EXPECT_STREQ(get_label_name(a), "NAME");
    EXPECT_STREQ(get_label_name(b), NULL);
    EXPECT_STREQ(get_label_name(c), "DATA");
    EXPECT_STREQ(get_label_name(d), NULL);
}

TEST(FirstPass, has_label) {
    char a[] = "Y: .data 202";
    char b[] = "HABRAHABR .data 205";
    char c[] = "HABRAHABR.:data 205";
    EXPECT_TRUE(has_label(a));
    EXPECT_FALSE(has_label(b));
    EXPECT_FALSE(has_label(c));
}

TEST(FirstPass, is_meaningless_line) {
    char a[] = ";Here is a comment";
    char b[] = "";
    char c[] = "\t\t\t  ";
    char d[] = " ";
    char e[] = "MOV ax, 125";
    EXPECT_TRUE(is_meaningless_line(a));
    EXPECT_TRUE(is_meaningless_line(b));
    EXPECT_TRUE(is_meaningless_line(c));
    EXPECT_TRUE(is_meaningless_line(d));
    EXPECT_FALSE(is_meaningless_line(e));
}

TEST(FirstPass, get_instr) {
    char a[] = "NAME: .string \"Linus\"";
    char b[] = ".data +7,-57,  17 , 9";
    char c[] = "DATA: .data +7,-57,  17 , 9";
    char d[] = ".entry HELLO";
    char e[] = ".extern HELLO";
    char f[] = "some nonsense and white bears ";
    char g[] = "MOV ax, 125";

    EXPECT_EQ(get_instr(a, true), string);
    EXPECT_NE(get_instr(a, false), string);
    
    EXPECT_EQ(get_instr(b, false), data);
    EXPECT_NE(get_instr(b, true), data);
    
    EXPECT_EQ(get_instr(c, true), data);
    EXPECT_NE(get_instr(c, false), data);
    
    EXPECT_EQ(get_instr(d, false), entry);
    
    EXPECT_EQ(get_instr(e, false), extrn);
    
    EXPECT_EQ(get_instr(f, false), none);
    EXPECT_EQ(get_instr(f, true), none);
    
    EXPECT_EQ(get_instr(g, false), none);
    EXPECT_EQ(get_instr(g, true), none);
}

TEST(FirstPass, remove_label) {
    char a[] = "NAME: .string \"Linus\"";
    char b[] = ".data +7,-57,  17 , 9";
    char c[] = "DATA: .data +7,-57,  17 , 9";
    char d[] = ".entry HELLO";
    
    EXPECT_STREQ(remove_label(a), ".string \"Linus\"");
    EXPECT_STREQ(remove_label(b), ".data +7,-57,  17 , 9");
    EXPECT_STREQ(remove_label(c), ".data +7,-57,  17 , 9");
    EXPECT_STREQ(remove_label(d), ".entry HELLO");
}

TEST(FirstPass, get_symbol_name) {
    char a[] = "NAME: .entry XYZ";
    char b[] = ".entry XYZ";
    char c[] = "DATA: .extern A";
    char d[] = ".extern HELLO";
    char e[] = ".extern";

    EXPECT_STREQ(get_symbol_name(a), "XYZ");
    EXPECT_STREQ(get_symbol_name(b), "XYZ");
    EXPECT_STREQ(get_symbol_name(c), "A");
    EXPECT_STREQ(get_symbol_name(d), "HELLO");
    EXPECT_STREQ(get_symbol_name(e), NULL);
}

TEST(FirstPass, add_symbol) {
    TAILQ_INIT(&(symbol_table.head));
    char s1[] = "START";
    char s2[] = "END";
    add_symbol(s1, 0x144, false, true);
    add_symbol(s2, 0x164, true, false);
    
}

TEST(FirstPass, get_string_data) {
    char s1[] = {".string \"I'm an englishman in New York\""};
    char s2[] = {".string \"home\""};
    char s3[] = {".string \"jakarta"};
    char s4[] = {".string home\""};
    
    EXPECT_STREQ(get_string_data(s1), "I'm an englishman in New York");
    EXPECT_STREQ(get_string_data(s2), "home");
    EXPECT_STREQ(get_string_data(s3), NULL);
    EXPECT_STREQ(get_string_data(s4), NULL);
}

TEST(FirstPass, get_action) {
    char *s1 = {"mov ax, 27"};
    char *s2 = {"cmp x, -5"};
    char *s3 = {"FUNC: add dx, 275"};
    char *s4 = {"sub 15, 6"};
    char *s5 = {"lea a, 13"};
    char *s6 = {"not x"};
    char *s7 = {"CLEAR: clr x"};
    char *s8 = {"inc z"};
    char *s9 = {"dec z"};
    char *s10 = {"jmp FUNC"};
    char *s11 = {"bne FUNC"};
    char *s12 = {"red dx"};
    char *s13 = {"prn dx"};
    char *s14 = {"jsr FUNC"};

    EXPECT_EQ(get_action(s1), 0);
    EXPECT_EQ(get_action(s2), 1);
    EXPECT_EQ(get_action(s3), 2);
    EXPECT_EQ(get_action(s4), 3);
    EXPECT_EQ(get_action(s5), 6);
    EXPECT_EQ(get_action(s6), 4);
    EXPECT_EQ(get_action(s7), 5);
    EXPECT_EQ(get_action(s8), 7);
    EXPECT_EQ(get_action(s9), 8);
    EXPECT_EQ(get_action(s10), 9);
    EXPECT_EQ(get_action(s11), 10);
    EXPECT_EQ(get_action(s12), 11);
    EXPECT_EQ(get_action(s13), 12);
    EXPECT_EQ(get_action(s14), 13);
}
