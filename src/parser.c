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
#include "../include/parser.h"

extern size_t argc;

extern string_t *tokens;

extern file_descriptor_t *redirections_indices;

extern string_t redirections[REDIRECTIONS_COUNT];

extern file_descriptor_t files_dup[];

extern file_descriptor_t file_to_close[REDIRECTIONS_COUNT];

extern flag_t redirections_flags[REDIRECTIONS_COUNT];

extern uint8_t command[COMMAND_SIZE];

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