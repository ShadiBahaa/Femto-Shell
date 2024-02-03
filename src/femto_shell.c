#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "../include/utilities.h"
#include "../include/femto_shell.h"
#include "../include/commands_headers.h"

shell_t shell_type;
uint8_t prompt[PROMPT_SIZE];
uint8_t command[COMMAND_SIZE];
uint8_t tokens[TOCKENS_COUNT][COMMAND_SIZE];

string_t jokes[JOKES_COUNT] = {
    "A3tiny command",
    "Elshell shelk yaba ektb",
    "lw 7slt moshkla msh bta3ty",
    "Fakar abl ma tktb abos eidk"

};
string_t commands[COMMANDS_COUNT] = {
    "exit", "cd", "echo", "pwd", "mv", "cp", "cat"};

command_handler_t command_handlers[COMMANDS_COUNT];

size_t argc;

void set_commands_handlers(void)
{
    command_handlers[0] = execute_exit;
    command_handlers[1] = execute_cd;
    command_handlers[2] = execute_echo;
    command_handlers[3] = execute_pwd;
    command_handlers[4] = execute_move;
    command_handlers[5] = execute_copy;
    command_handlers[6] = execute_cat;
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
                strcpy(tokens[tokens_iterator], tmp);
                argc++;
                ++tokens_iterator;
                memset(tokens[tokens_iterator], NULL_TERMINATOR, COMMAND_SIZE);
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
        strcpy(tokens[tokens_iterator], tmp);
        argc++;
        tokens_iterator++;
    }
    memset(tokens[tokens_iterator], NULL_TERMINATOR, COMMAND_SIZE);
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
        printf("No such command\n");
    }
}
int main(void)
{
    set_commands_handlers();
    printf("Choose your shell type: (A for formal B for informal) : ");
    shell_type = getchar();
    while (true)
    {
        if (shell_type == FORMAL_PROMPT)
        {
            getcwd(prompt, PROMPT_SIZE);
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
            shell_type = getchar();
        }
    }
    getchar();
    while (true)
    {
        argc = 0;
        printf("%s", prompt);
        printf("%s", PROMPT_STRING);
        fflush(stdout);
        fgets(command, COMMAND_SIZE, stdin);
        parse_command();
        execute_command();
    }
}