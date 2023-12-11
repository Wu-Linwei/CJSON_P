#include "stdio.h"
#include "leptjson.h"
typedef struct
{
    const char *json;
} lept_context;

// 提示：这里应该是 JSON-text = ws value ws
// 以下实现未处理最后的ws和LEPT_PARSE_ROOT_NOT_SINGULAR
int lept_parse(lept_value *value, const char *json)
{
    lept_context c;
    assert(v != NULL);
    c.json = json;
    v->tyoe = LEPT_NULL;
    lept_parse_whitespace(&c);
    return lept_parse_value(&c, v);
}
