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
#include "../include/femto_shell.h"
#include "../include/commands_headers.h"

shell_t shell_type;
uint8_t prompt[PROMPT_SIZE];
uint8_t command[COMMAND_SIZE];
string_t *tokens = NULL;
file_descriptor_t *redirections_indices = NULL;

string_t jokes[JOKES_COUNT] = {
    "A3tiny command",
    "Elshell shelk yaba ektb",
    "lw 7slt moshkla msh bta3ty",
    "Fakar abl ma tktb abos eidk"

};
string_t redirections[REDIRECTIONS_COUNT] = {
    "<", ">", ">>", "2>"};

flag_t redirections_flags[REDIRECTIONS_COUNT] = {
    O_RDONLY,
    O_WRONLY | O_CREAT | O_TRUNC,
    O_WRONLY | O_APPEND,
    O_WRONLY | O_CREAT | O_TRUNC};
file_descriptor_t file_to_close[REDIRECTIONS_COUNT] = {
    STDIN_FILENO,
    STDOUT_FILENO,
    STDOUT_FILENO,
    STDERR_FILENO};

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

file_descriptor_t files_dup[] = {
    -1, -1, -1};

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

boolean_t handle_redirections(void)
{
    iterator_t tokens_iterator;
    for (tokens_iterator = 0; tokens_iterator < argc; ++tokens_iterator)
    {
        iterator_t redirections_iterator;
        for (redirections_iterator = 0; redirections_iterator < REDIRECTIONS_COUNT; ++redirections_iterator)
        {
            if (strcmp(tokens[tokens_iterator], redirections[redirections_iterator]) == IDENTICAL && tokens[tokens_iterator + 1] != NULL)
            {
                if ((files_dup[file_to_close[redirections_iterator]] = dup(file_to_close[redirections_iterator])) == DUP_ERROR)
                {
                    perror("dup error : ");
                    return false;
                }
                if (close(file_to_close[redirections_iterator]) != SUCCESSFUL_CLOSE)
                {
                    perror("close error : ");
                    return false;
                }
                if (open(tokens[tokens_iterator + 1], redirections_flags[redirections_iterator], 0644) == FILE_OPEN_ERROR)
                {
                    perror("Open File error: ");
                    return false;
                }

                iterator_t next_token = tokens_iterator + 2;
                iterator_t tmp_iterator = tokens_iterator;
                while (next_token < argc)
                {
                    strcpy(tokens[tmp_iterator], tokens[next_token]);
                    next_token++;
                    tmp_iterator++;
                }
                while (tmp_iterator < argc)
                {
                    free(tokens[tmp_iterator]);
                    tokens[tmp_iterator] = NULL;
                    tmp_iterator++;
                }
                argc -= 2;
                tokens_iterator--;
                break;
            }
        }
    }
    return true;
}

void get_joke_prompt(void)
{
    srand(time(0));
    strcpy(prompt, jokes[rand() % JOKES_COUNT]);
}

boolean_t parse_command(void)
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
                return false;
            }
            if ((tokens[tokens_iterator] = (string_t)malloc(sizeof(uint8_t) * COMMAND_SIZE)) == NULL)
            {
                perror("malloc error: ");
                return false;
            }
            strcpy(tokens[tokens_iterator], tmp);
            argc++;
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
            return false;
        }
        if ((tokens[tokens_iterator] = (string_t)malloc(sizeof(uint8_t) * COMMAND_SIZE)) == NULL)
        {
            perror("malloc error: ");
            return false;
        }
        strcpy(tokens[tokens_iterator], tmp);
        argc++;
        tokens_iterator++;
    }
    if ((tokens = (string_t *)realloc(tokens, (tokens_iterator + 1) * sizeof(string_t))) == NULL)
    {
        perror("Realloc error: ");
        return false;
    }

    tokens[tokens_iterator] = NULL;
    return true;
}
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
int main(void)
{
    set_commands_handlers();
    if (printf("Choose your shell type: (A for formal B for informal) : ") < 0)
    {
        perror("printf error :");
        return -1;
    }
    if ((shell_type = getchar()) == EOF)
    {
        perror("getchar error: ");
        return -1;
    }
    while (true)
    {
        if (shell_type == FORMAL_PROMPT)
        {
            if (getcwd(prompt, PROMPT_SIZE) == NULL)
            {
                perror("getcwd error: ");
                return -1;
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
                return -1;
            }
        }
    }
    if (getchar() == EOF)
    {
        perror("getchar error: ");
        return -1;
    }
    while (true)
    {
        if (reset_default_files())
        {

            reset_tokens();

            if (printf("%s", prompt) < 0)
            {
                perror("printf error: ");
                continue;
            }
            if (printf("%s", PROMPT_STRING) < 0)
            {
                perror("printf error: ");
                continue;
            }
            if (fflush(stdout) == EOF)
            {
                perror("fflush error :");
                continue;
                ;
            }
            if (fgets(command, COMMAND_SIZE, stdin) == NULL)
            {
                perror("fgets error : ");
                continue;
                ;
            }
            if (parse_command())
            {
                if (handle_redirections())
                {
                    execute_command();
                }
            }
        }
    }
}
