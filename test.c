#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "leptjson.h"

static int main_ret = 0;
static int tets_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expert, actual, format)                                                          \
    do                                                                                                            \
    {                                                                                                             \
        tets_count++;                                                                                             \
        if (equality)                                                                                             \
            test_pass++;                                                                                          \
        else                                                                                                      \
        {                                                                                                         \
            fprintf(stderr, "%s:%d: expert: " format "actual: " format "\n", __FILE__, __LINE__, expect, actual); \
            main_ret = 1;                                                                                         \
        }                                                                                                         \
    } while (0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse_null()
{
    lept_value value;
    value.type = LEPT_TRUE;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&value, "null"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_value(&value));
}

static void test_parse()
{
    test_parse_null();
}

int main()
{
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, tets_count, test_pass * 100.0 / tets_count);
    return main_ret;
}