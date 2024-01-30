#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "femto_shell.h"

void get_joke_prompt(prompt_t joke_reference){
    srand(time(0));
    strcpy(joke_reference,jokes[rand()%JOKES_COUNT]);
}

void parse_Command(command_t command){
    iterator_t command_iterator = 0;
    command_t temp_command;
    while (command[command_iterator]==SPACE_CHAR){
        command_iterator ++;
    }
    iterator_t tmp_command_iterator = 0;
    while (command[command_iterator]!= NULL_TERMINATOR && command[command_iterator]!=SPACE_CHAR && command[command_iterator]!=NEW_LINE && command[command_iterator]!=CARRIAGE_RETURN){
        temp_command[tmp_command_iterator] = command[command_iterator];
        command_iterator++;
        tmp_command_iterator++;
    }
    temp_command[tmp_command_iterator] = NULL_TERMINATOR;
    if (strcmp(EXIT_COMMAND,temp_command)==IDENTICAL){
        printf("GoodBye!\n");
        exit(RETURN_STATUS);
    }else {
        printf("You said: %s\n",command);
    }
}

int main(void){
    printf("Choose your shell type: (A for formal B for informal) : ");
    shell_t shell_type;
    prompt_t prompt;
    shell_type = getchar();
    while (true)
    {
        if (shell_type == FORMAL_PROMPT){
            getcwd(prompt,PROMPT_SIZE);
            break;
        }else if (shell_type == INFORMAL_PROMPT){
            get_joke_prompt(prompt);
            break;
        }else {
            printf("Wrong! Try again : ");
            shell_type = getchar();
        }
    }
    getchar();
    while (true)
    {
        printf("%s",prompt);
        printf("%s",PROMPT_STRING);
        command_t command;
        fflush(stdout);
        fgets(command,COMMAND_SIZE,stdin);
        parse_Command(command);
    }
    
}
