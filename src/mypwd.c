#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/utilities.h"
#include "../include/mypwd.h"

extern size_t argc;

void execute_pwd(void)
{
    if (argc != 1){
        printf("Usage error : no arguments required\n");
        return ;
    }
    uint8_t directory_buffer[DIRECTORY_BUFFER_SIZE];
    if (getcwd(directory_buffer, DIRECTORY_BUFFER_SIZE) == NULL)
    {
        perror("PWD ERROR: ");
    }
    else
    {
        printf("%s\n", directory_buffer);
    }
}