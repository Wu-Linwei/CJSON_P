#include "stdio.h"
#include "assert.h"
#include "leptjson.h"

#define EXPECT(c, ch)             \
    do                            \
    {                             \
        assert(*c->json == (ch)); \
        c->json++;                \
    } while (0)

typedef struct
{
    const char *json;
} lept_context;

static void lept_parse_whitespace(lept_context *c)
{
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

static int lept_parse_true(lept_context *c, lept_value *value)
{
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    value->type = LEPT_TRUE;
    return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context *c, lept_value *value)
{
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
    {
        return LEPT_PARSE_INVALID_VALUE;
    }
    c->json += 3;
    value->type = LEPT_FALSE;
    return LEPT_PARSE_OK;
}

static int lept_parse_null(lept_context *c, lept_value *value)
{
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
    {
        return LEPT_PARSE_INVALID_VALUE;
    }
    c->json += 3;
    value->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context *c, lept_value *value)
{
    switch (*c->json)
    {
    case 't':
        return lept_parse_true(c, value);
    case 'f':
        return lept_parse_false(c, value);
    case 'n':
        return lept_parse_null(c, value);
    case '\0':
        return LEPT_PARSE_EXPECT_VALUE;
    default:
        return LEPT_PARSE_INVALID_VALUE;
    }
}
// 提示：这里应该是 JSON-text = ws value ws
// 以下实现未处理最后的ws和LEPT_PARSE_ROOT_NOT_SINGULAR
int lept_parse(lept_value *value, const char *json)
{
    lept_context c;
    int ret;
    assert(value != NULL);
    c.json = json;
    value->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    if ((ret = lept_parse_value(&c, value)) == LEPT_PARSE_OK)
    {
        lept_parse_whitespace(&c);
        if (*c.json != '\0')
        {
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
}

lept_type lept_get_type(const lept_value *value)
{
    assert(value != NULL);
    return value->type;
}