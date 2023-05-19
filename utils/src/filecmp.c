#include <stdbool.h>
#include <stdio.h>

#include "filecmp.h"

bool txt_cmp(const char *file1, const char *file2)
{
    FILE *fp1 = fopen(file1, "r");
    FILE *fp2 = fopen(file2, "r");

    if (fp1 == NULL || fp2 == NULL)
    {
        return false;
    }

    int byte1, byte2;
    bool equal = true;
    do
    {
        byte1 = fgetc(fp1);
        byte2 = fgetc(fp2);

        if (byte1 != byte2)
        {
            equal = false;
            break;
        }

    } while (byte1 != EOF && byte2 != EOF);

    fclose(fp1);
    fclose(fp2);

    return equal && byte1 == EOF && byte2 == EOF;
}