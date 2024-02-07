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
#include "../include/parser.h"
#include "../include/executer.h"
#include "../include/resetter.h"

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

void get_joke_prompt(void)
{
    srand(time(0));
    strcpy(prompt, jokes[rand() % JOKES_COUNT]);
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