#include "stdio.h"  /* NULL, strtod() */
#include "assert.h" /* assert() */
#include "errno.h"  /* errno, ERANGE */
#include "math.h"   /* HUGE_VAL */
#include "leptjson.h"

#define EXPECT(c, ch)             \
    do                            \
    {                             \
        assert(*c->json == (ch)); \
        c->json++;                \
    } while (0)
#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch) ((ch) >= '1' && (ch) <= '9')

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

static int lept_parse_literal(lept_context *c, lept_value *value, const char *literal, lept_type type)
{
    size_t i;
    EXPECT(c, literal[0]);
    for (i = 0; literal[i + 1]; i++)
        if (c->json[i] != literal[i + 1])
            return LEPT_PARSE_INVALID_VALUE;
    c->json += i;
    value->type = type;
    return LEPT_PARSE_OK;
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

static int lept_parse_number(lept_context *c, lept_value *value)
{
    const char *p = c->json;
    if (*p == '-')
        p++;
    if (*p == '0')
        p++;
    else
    {
        if (!ISDIGIT1TO9(*p))
            return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++)
            ;
    }
    if (*p == '.')
    {
        p++;
        if (!ISDIGIT(*p))
            return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++)
            ;
    }

    if (*p == 'e' || *p == 'E')
    {
        p++;
        if (*p == '+' || *p == '-')
            p++;
        if (!ISDIGIT(*p))
            return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++)
            ;
    }
    errno = 0;
    value->n = strnod(c->json, NULL);
    if (errno == ERANGE && (value->n == HUGE_VAL || value->n == -HUGE_VAL))
        return LEPT_PARSE_NUMBER_TOO_BIG;
    value->type = LEPT_NUMBER;
    c->json = p;
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context *c, lept_value *value)
{
    switch (*c->json)
    {
    case 't':
        return lept_parse_literal(c, value, "true", LEPT_TRUE);
    case 'f':
        return lept_parse_literal(c, value, "false", LEPT_FALSE);
    case 'n':
        return lept_parse_literal(c, value, "null", LEPT_NULL);
    case '\0':
        return LEPT_PARSE_EXPECT_VALUE;
    default:
        return lept_parse_number(c, value);
    }
}

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
            value->type = LEPT_NULL;
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

lept_type lept_get_type(const lept_value *value)
{
    assert(value != NULL);
    return value->type;
}

double lept_get_number(const lept_value *value)
{
    assert(value != NULL && value->type == LEPT_NUMBER);
    return value->n;
}