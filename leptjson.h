#ifndef LEPTJSON_H__
#define LEPTJSON_H__

typedef enum
{
    LEPT_NULL,
    LEPT_FALSE,
    LEPT_TRUE,
    LEPT_NUMBER,
    LEPT_STRING,
    LEPT_ARRAY,
    LEPT_OBJECT
} lept_type;

typedef struct
{
    lept_type type;
} lept_value;

int lept_parse(lept_value *value, const char *json);

lept_type lept_get_value(const lept_value *value);

#endif // LEPTJSON_H__