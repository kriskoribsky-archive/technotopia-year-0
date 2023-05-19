#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "stdiotest.h"
#include "filecmp.h"

bool test_main_stdio(main_void callback, char *input_file, char *expected_file, char *ouput_file)
{
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);

    FILE *fp_in = fopen(input_file, "r");
    FILE *fp_out = fopen(ouput_file, "w");

    fflush(stdout);
    dup2(fileno(fp_in), STDIN_FILENO);
    dup2(fileno(fp_out), STDOUT_FILENO);

    callback();

    fflush(stdout);
    fclose(fp_in);
    fclose(fp_out);

    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);

    return txt_cmp(expected_file, ouput_file);
}

bool test_main_stdio_params(main_params callback, int argc, char **argv, char *input_file, char *expected_file, char *ouput_file)
{
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);

    FILE *fp_in = fopen(input_file, "r");
    FILE *fp_out = fopen(ouput_file, "w");

    fflush(stdout);
    dup2(fileno(fp_in), STDIN_FILENO);
    dup2(fileno(fp_out), STDOUT_FILENO);

    callback(argc, argv);

    fflush(stdout);
    fclose(fp_in);
    fclose(fp_out);

    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);

    return txt_cmp(expected_file, ouput_file);
}