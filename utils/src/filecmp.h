#ifndef _FILECMP_H 
#define _FILECMP_H

#include <stdbool.h>

/**
 * Compare two text files byte by byte.
 * Similiar to cmp linux user command.
 * 
 * @param file1 path to the first file
 * @param file2 path to the second file
 * @return true if the two files are identical, false otherwise
 */
bool txt_cmp(const char *file1, const char *file2);

#endif