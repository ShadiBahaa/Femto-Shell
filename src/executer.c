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
#include "../include/executer.h"

extern string_t commands[COMMANDS_COUNT];

extern command_handler_t command_handlers[COMMANDS_COUNT];

extern string_t *tokens;

boolean_t execute_command(void)
{
    boolean_t chosen = false;
    iterator_t command_iterator;
    for (command_iterator = 0; command_iterator < COMMANDS_COUNT; ++command_iterator)
    {
        if (strcmp(commands[command_iterator], tokens[0]) == IDENTICAL)
        {
            command_handlers[command_iterator]();
            chosen = true;
            break;
        }
    }
    if (!chosen)
    {
        pid_t fork_return = fork();
        if (fork_return == FAILED_FORK)
        {
            perror("Fork error: ");
            return false;
        }
        else if (fork_return == FORK_CHILD)
        {
            execvp(tokens[0], tokens);
            perror("Execvp error: ");
            return false;
        }
        else
        {
            wait(NULL);
        }
    }
    return true;
}