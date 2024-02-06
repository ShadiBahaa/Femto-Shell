#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include "../include/utilities.h"
#include "../include/femto_shell.h"
#include "../include/commands_headers.h"

shell_t shell_type;
uint8_t prompt[PROMPT_SIZE];
uint8_t command[COMMAND_SIZE];
string_t *tokens = NULL;

string_t jokes[JOKES_COUNT] = {
    "A3tiny command",
    "Elshell shelk yaba ektb",
    "lw 7slt moshkla msh bta3ty",
    "Fakar abl ma tktb abos eidk"

};

#if TRY_EXTERNAL_COMMANDS_WITHOUT_EXECVP == 1
#define COMMANDS_COUNT 7
#else
#define COMMANDS_COUNT 4
#endif

string_t commands[COMMANDS_COUNT] = {
    "exit", "cd", "echo", "pwd"
#if TRY_EXTERNAL_COMMANDS_WITHOUT_EXECVP == 1
    ,
    "mv", "cp", "cat"
#endif
};

command_handler_t command_handlers[COMMANDS_COUNT];

size_t argc;

void set_commands_handlers(void)
{
    command_handlers[0] = execute_exit;
    command_handlers[1] = execute_cd;
    command_handlers[2] = execute_echo;
    command_handlers[3] = execute_pwd;
#if TRY_EXTERNAL_COMMANDS_WITHOUT_EXECVP == 1
    command_handlers[4] = execute_move;
    command_handlers[5] = execute_copy;
    command_handlers[6] = execute_cat;
#endif
}

void get_joke_prompt(void)
{
    srand(time(0));
    strcpy(prompt, jokes[rand() % JOKES_COUNT]);
}

void parse_command(void)
{
    iterator_t command_iterator = 0;
    iterator_t tmp_iterator = 0;
    iterator_t tokens_iterator = 0;
    uint8_t tmp[COMMAND_SIZE];
    while (command[command_iterator] == SPACE_CHAR)
        command_iterator++;
    while (command[command_iterator] != NULL_TERMINATOR && command[command_iterator] != NEW_LINE && command[command_iterator] != CARRIAGE_RETURN)
    {
        boolean_t is_space = false;
        while (command[command_iterator] == SPACE_CHAR)
        {
            is_space = true;
            command_iterator++;
        }
        if (is_space)
        {
            tmp[tmp_iterator] = NULL_TERMINATOR;
            if ((tokens = (string_t *)realloc(tokens, (tokens_iterator + 1) * sizeof(string_t))) == NULL)
            {
                perror("Realloc error: ");
                exit(-1);
            }
            if ((tokens[tokens_iterator] = (string_t)malloc(sizeof(uint8_t) * COMMAND_SIZE)) == NULL)
            {
                perror("malloc error: ");
                exit(-1);
            }
            strcpy(tokens[tokens_iterator], tmp);
            argc++;
            if (strcmp(tmp, "echo") == IDENTICAL)
            {
                ++tokens_iterator;
                tmp_iterator = 0;
                tmp[0] = NULL_TERMINATOR;
                while (command[command_iterator] != NULL_TERMINATOR && command[command_iterator] != NEW_LINE && command[command_iterator] != CARRIAGE_RETURN)
                {
                    tmp[tmp_iterator] = command[command_iterator];
                    ++command_iterator;
                    ++tmp_iterator;
                }
                tmp[tmp_iterator] = NULL_TERMINATOR;
                if ((tokens = (string_t *)realloc(tokens, (tokens_iterator + 1) * sizeof(string_t))) == NULL)
                {
                    perror("Realloc error: ");
                    exit(-1);
                }
                if ((tokens[tokens_iterator] = (string_t)malloc(sizeof(uint8_t) * COMMAND_SIZE)) == NULL)
                {
                    perror("malloc error: ");
                    exit(-1);
                }
                strcpy(tokens[tokens_iterator], tmp);
                argc++;
                ++tokens_iterator;
                if ((tokens = (string_t *)realloc(tokens, (tokens_iterator + 1) * sizeof(string_t))) == NULL)
                {
                    perror("Realloc error: ");
                    exit(-1);
                }
                if ((tokens[tokens_iterator] = (string_t)malloc(sizeof(uint8_t) * COMMAND_SIZE)) == NULL)
                {
                    perror("malloc error: ");
                    exit(-1);
                }
                tokens[tokens_iterator] = NULL;
                return;
            }
            tmp_iterator = 0;
            tmp[0] = NULL_TERMINATOR;
            tokens_iterator++;
            command_iterator--;
        }
        else
        {
            tmp[tmp_iterator] = command[command_iterator];
            tmp_iterator++;
        }
        command_iterator++;
    }
    if (tmp[0] != NULL_TERMINATOR)
    {
        tmp[tmp_iterator] = NULL_TERMINATOR;
        if ((tokens = (string_t *)realloc(tokens, (tokens_iterator + 1) * sizeof(string_t))) == NULL)
        {
            perror("Realloc error: ");
            exit(-1);
        }
        if ((tokens[tokens_iterator] = (string_t)malloc(sizeof(uint8_t) * COMMAND_SIZE)) == NULL)
        {
            perror("malloc error: ");
            exit(-1);
        }
        strcpy(tokens[tokens_iterator], tmp);
        argc++;
        tokens_iterator++;
    }
    if ((tokens = (string_t *)realloc(tokens, (tokens_iterator + 1) * sizeof(string_t))) == NULL)
    {
        perror("Realloc error: ");
        exit(-1);
    }

    tokens[tokens_iterator] = NULL;
}
void execute_command(void)
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
            exit(-1);
        }
        else if (fork_return == FORK_CHILD)
        {
            execvp(tokens[0], tokens);
            perror("Execvp error: ");
            exit(-1);
        }
        else
        {
            wait(NULL);
        }
    }
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
    tokens = NULL;
}
int main(void)
{
    set_commands_handlers();
    if (printf("Choose your shell type: (A for formal B for informal) : ") < 0)
    {
        perror("printf error :");
        exit(-1);
    }
    if ((shell_type = getchar()) == EOF)
    {
        perror("getchar error: ");
        exit(-1);
    }
    while (true)
    {
        if (shell_type == FORMAL_PROMPT)
        {
            if (getcwd(prompt, PROMPT_SIZE) == NULL)
            {
                perror("getcwd error: ");
                exit(-1);
            }
            break;
        }
        else if (shell_type == INFORMAL_PROMPT)
        {
            get_joke_prompt();
            break;
        }
        else
        {
            printf("Wrong! Try again : ");
            if ((shell_type = getchar()) == EOF)
            {
                perror("getchar error: ");
                exit(-1);
            }
        }
    }
    if (getchar() == EOF)
    {
        perror("getchar error: ");
        exit(-1);
    }
    while (true)
    {
        argc = 0;
        if (printf("%s", prompt) < 0)
        {
            perror("printf error: ");
            exit(-1);
        }
        if (printf("%s", PROMPT_STRING) < 0)
        {
            perror("printf error: ");
            exit(-1);
        }
        if (fflush(stdout) == EOF)
        {
            perror("fflush error :");
            exit(-1);
        }
        if (fgets(command, COMMAND_SIZE, stdin) == NULL)
        {
            perror("fgets error : ");
            exit(-1);
        }
        parse_command();
        execute_command();
    }
}
