#include <string.h>
#include <ctype.h>

#include "helpers.h"

size_t trim(char *out, size_t len, const char *s)
{
    if (len == 0)
        return 0;

    const char *end;
    size_t out_size;

    // trim leading space
    while (isspace((unsigned char)*s))
        s++;

    if (*s == 0)
    {
        *out = 0;
        return 1;
    }

    // trim trailing space
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end))
        end--;
    end++;

    // set output size to minimum of trimmed string length and buffer size minus 1
    out_size = (size_t)(end - s) < (size_t)len - 1 ? (size_t)(end - s) : (size_t)len - 1;

    // copy trimmed string and add null terminator
    memcpy(out, s, out_size);
    out[out_size] = 0;

    return out_size;
}