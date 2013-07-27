#include "../src/mystring.c"
#include <gtest/gtest.h>

TEST(Mystring, is_empty_string) {
    EXPECT_TRUE(is_empty_string("     "));
    EXPECT_TRUE(is_empty_string("  \t       "));
    EXPECT_FALSE(is_empty_string("hello world!"));
    EXPECT_FALSE(is_empty_string("          ;"));
}

TEST(Mystring, starts_with_char) {
    EXPECT_TRUE(starts_with_char("Flower", 'F'));
    EXPECT_TRUE(starts_with_char(";  Flower", ';'));
    EXPECT_FALSE(starts_with_char("  Flower", ';'));
    EXPECT_FALSE(starts_with_char("Flower", ';'));
    EXPECT_FALSE(starts_with_char("\tFlower", 'j'));
    
}

TEST(Mystring, trim_whitespace) {
    // C stores string literals in unnamed read only buffers, so it's immutable
    char a[] = " sababa baba   ";
    char b[] = "\t sababa baba \t  ";
    char c[] = "sababa\tbaba  ";
    EXPECT_STREQ(trim_whitespace(a), "sababa baba");
    EXPECT_STREQ(trim_whitespace(b), "sababa baba");
    EXPECT_STRNE(trim_whitespace(c), "sababa baba");
}

TEST(Mystring, ends_with_char) {
    EXPECT_TRUE(ends_with_char("Flower", 'r'));
    EXPECT_TRUE(ends_with_char("  Flower;", ';'));
    EXPECT_FALSE(ends_with_char("  Flower", '\"'));
    EXPECT_FALSE(ends_with_char("Flower", ';'));
    EXPECT_FALSE(ends_with_char("\tFlower", 'j'));

}

TEST(Mystring, split_string) {
    Split *result;
    result = split_string("Come together, right now", ' ');
    EXPECT_STREQ((result -> tail), "together, right now");
}

TEST(Mystring, remove_before_space) {
    char s1[] = {"sudo make sandwich"};
    
    EXPECT_STREQ(remove_before_space(s1), "make sandwich");
}