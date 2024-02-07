#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/utilities.h"
#include "../include/resetter.h"

extern file_descriptor_t files_dup[];

extern size_t argc;

extern string_t *tokens;

boolean_t reset_default_files(void)
{
    file_descriptor_t file_descriptor;
    for (file_descriptor = 0; file_descriptor < 3; ++file_descriptor)
    {
        if (files_dup[file_descriptor] != -1)
            if (dup2(files_dup[file_descriptor], file_descriptor) == DUP_ERROR)
            {
                perror("dup error : ");
                return false;
            }
        files_dup[file_descriptor] = -1;
    }
    return true;
}
void reset_tokens(void)
{
    iterator_t argv_iterator;
    for (argv_iterator = 0; argv_iterator < argc; ++argv_iterator)
    {
        if (tokens[argv_iterator] != NULL)
        {
            free(tokens[argv_iterator]);
            tokens[argv_iterator] = NULL;
        }
    }
    free(tokens);
    argc = 0;
    tokens = NULL;
}