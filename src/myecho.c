#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/utilities.h"
#include "../include/myecho.h"

extern size_t argc;
extern string_t * tokens;

void execute_echo(void){
    iterator_t argv_iterator = 1;
    while (argv_iterator < argc){
        printf("%s",tokens[argv_iterator]);
        if (argv_iterator != argc - 1){
            printf(" ");
        }
        argv_iterator ++;
    }
    printf("\n");
}
