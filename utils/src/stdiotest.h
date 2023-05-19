#ifndef _STDIOTEST_H
#define _STDIOTEST_H

#include <stdbool.h>

typedef int (*main_void)(void);
typedef int (*main_params)(int arc, char **argv);

/**
 * Compare stdout of main function with expected values.
 *
 * @param target_main_void pointer to the target program's main function to be executed
 * @param input_file path to the input file to be used for the program execution
 * @param ouput_file path to the output file to be generated by the program execution
 * @param expected_file path to the expected output file to be compared against the generated output file
 * @return true if the generated output file matches the expected output file, false otherwise
 */
bool test_main_stdio(main_void callback, char *input_file, char *expected_file, char *ouput_file);

/**
 * Compare stdout of main function with expected values.
 *
 * @param callback pointer to the target program's main function to be executed
 * @param argc number of command line arguments
 * @param argv command line arguments array
 * @param input_file path to the input file to be used for the program execution
 * @param ouput_file path to the output file to be generated by the program execution
 * @param expected_file path to the expected output file to be compared against the generated output file
 * @return true if the generated output file matches the expected output file, false otherwise
 */
bool test_main_stdio_params(main_params callback, int argc, char **argv, char *input_file, char *expected_file, char *ouput_file);

#endif