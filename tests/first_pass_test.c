#include "../src/mystring.c"
#include "../src/symbols.c"
#include "../src/first_pass.c"
#include <gtest/gtest.h>
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

    EXPECT_EQ(get_instr(a), string);
    EXPECT_EQ(get_instr(a), string);

    EXPECT_EQ(get_instr(b), data);
    EXPECT_EQ(get_instr(b), data);

    EXPECT_EQ(get_instr(c), data);
    EXPECT_EQ(get_instr(c), data);

    EXPECT_EQ(get_instr(d), entry);

    EXPECT_EQ(get_instr(e), extrn);

    EXPECT_EQ(get_instr(f), NONE);
    EXPECT_EQ(get_instr(f), NONE);

    EXPECT_EQ(get_instr(g), NONE);
    EXPECT_EQ(get_instr(g), NONE);
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
    sglib_hashed_Symbol_init(symbol_table);
    char s1[] = "START";
    char s2[] = "END";
    Symbol *result;
    add_symbol(s1, 0x144, false, true);
    add_symbol(s2, 0x164, true, false);
    result = get_symbol_by_name(symbol_table, "END");
    EXPECT_EQ(result -> address, 0x164);
    
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

TEST(FirstPass, get_opert_type) {
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

    EXPECT_EQ(get_opert_type(s1), 0);
    EXPECT_EQ(get_opert_type(s2), 1);
    EXPECT_EQ(get_opert_type(s3), 2);
    EXPECT_EQ(get_opert_type(s4), 3);
    EXPECT_EQ(get_opert_type(s5), 6);
    EXPECT_EQ(get_opert_type(s6), 4);
    EXPECT_EQ(get_opert_type(s7), 5);
    EXPECT_EQ(get_opert_type(s8), 7);
    EXPECT_EQ(get_opert_type(s9), 8);
    EXPECT_EQ(get_opert_type(s10), 9);
    EXPECT_EQ(get_opert_type(s11), 10);
    EXPECT_EQ(get_opert_type(s12), 11);
    EXPECT_EQ(get_opert_type(s13), 12);
    EXPECT_EQ(get_opert_type(s14), 13);
}

TEST(FirstPass, get_index_expr) {
    int error = 0;
    char *result;
    char *s1 = {"STRING{*LENGTH}"};
    char *s2 = {"y{5}"};
    char *s3 = {"ab{{c}}"};
    char *s4 = {"r2"};

    
    result = get_index_expr(s1, &error);
    EXPECT_EQ(error, 0);
    EXPECT_STREQ(result, "*LENGTH");
    
    error = 0;
    result = get_index_expr(s2, &error);
    EXPECT_EQ(error, 0);
    EXPECT_STREQ(result, "5");
    
    error = 0;
    result = get_index_expr(s3, &error);
    EXPECT_EQ(error, INVALID);
    
    error = 0;
    result = get_index_expr(s4, &error);
    EXPECT_EQ(error, NONE);
}

TEST(FirstPass, get_register_code) {
    int error = 0;
    char *result;
    char *s1 = {"r1"};
    char *s2 = {"r7"};
    char *s3 = {"a1"};
    char *s4 = {"r10"};
    
    EXPECT_EQ(get_register_code(s1), 1);
    EXPECT_EQ(get_register_code(s2), 7);
    EXPECT_EQ(get_register_code(s3), INVALID);
    EXPECT_EQ(get_register_code(s4), INVALID);
}

TEST(FirstPass, get_single_operand_length) {
    char s1[] = {"r1"};
    char s2[] = {"r7"};
    char s3[] = {"a"};
    char s4[] = {"y{10}"};
    char s5[] = {"STR{*x}"};
    char s6[] = {"WORD{r3}"};
    char s7[] = {"#545"};
    
    int reg = -10, adr;
    
    EXPECT_EQ(get_single_operand_info(s1, NULL, NULL), 0);
    EXPECT_EQ(get_single_operand_info(s2, &reg, &adr), 0);
    EXPECT_EQ(reg, 7);
    EXPECT_EQ(adr, 3);
    reg = -10; adr = 0;
    
    
    EXPECT_EQ(get_single_operand_info(s3, &reg, &adr), 1);
    EXPECT_EQ(reg, -10);
    EXPECT_EQ(adr, 1);
    reg = -10;
    adr = 0;
    
    EXPECT_EQ(get_single_operand_info(s4, NULL, NULL), 2);
    EXPECT_EQ(get_single_operand_info(s5, NULL, NULL), 2);
    EXPECT_EQ(get_single_operand_info(s6, NULL, NULL), 1);
    
    EXPECT_EQ(get_single_operand_info(s7, &reg, &adr), 1);
    EXPECT_EQ(reg, -10);
    EXPECT_EQ(adr, 0);
    reg = -1;
    adr = 0;
}

TEST(FirstPass, get_binary_length) {
    char s1[] = {"mov #67, r2"};
    char s2[] = {"add y, r0"};
    char s3[] = {"cmp y{3}, #14"};
    char s4[] = {"mov STRING{XYZ}, x"};
    
    EXPECT_EQ(get_binary_length(s1), 2);
    EXPECT_EQ(get_binary_length(s2), 2);
    EXPECT_EQ(get_binary_length(s3), 4);
    EXPECT_EQ(get_binary_length(s4), 4);
}

TEST(FirstPass, calc_code_length) {
    char s1[] = {"mov A, r1"};
    char s2[] = {"cmp y{z}, #22"};
    char s3[] = {"add y, r2"};
    char s4[] = {"sub z, r7"};
    char s5[] = {"lea z, #91"};

    EXPECT_EQ(calc_code_length(s1), 2);
    EXPECT_EQ(calc_code_length(s2), 4);
    EXPECT_EQ(calc_code_length(s3), 2);
    EXPECT_EQ(calc_code_length(s4), 2);
    EXPECT_EQ(calc_code_length(s5), 3);
}
    
