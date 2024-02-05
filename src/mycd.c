#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "../include/utilities.h"
#include "../include/mycd.h"

extern size_t argc;
extern string_t * tokens;
extern shell_t shell_type;
extern uint8_t prompt[PROMPT_SIZE];

void execute_cd(void)
{
    if (argc != 2){
        printf("Usage error : cd file_path\n");
        return;
    }
    if (chdir(tokens[1]) == SUCCESSFUL_CD)
    {
        if (shell_type == FORMAL_PROMPT)
        {
            getcwd(prompt, PROMPT_SIZE);
        }
    }
    else
    {
        perror("CD ERROR: ");
    }
}
