#ifndef _FEMTO_SHELL_H_
#define _FEMTO_SHELL_H_

#define true 1
#define PROMPT_SIZE 1000
#define COMMAND_SIZE 1000
#define JOKES_COUNT 4
#define FORMAL_PROMPT 'A'
#define INFORMAL_PROMPT 'B'
#define PROMPT_STRING " > "
#define SPACE_CHAR ' '
#define NULL_TERMINATOR '\0'
#define IDENTICAL 0
#define CARRIAGE_RETURN '\r'
#define NEW_LINE '\n'
#define EXIT_COMMAND "exit"
#define RETURN_STATUS 0

typedef unsigned char shell_t; 
typedef char prompt_t[PROMPT_SIZE];
typedef char command_t[COMMAND_SIZE];
typedef signed int iterator_t;


prompt_t jokes [JOKES_COUNT] = {
    "A3tiny command",
    "Elshell shelk yaba ektb",
    "lw 7slt moshkla msh bta3ty",
    "Fakar abl ma tktb abos eidk"

};

void get_joke_prompt(prompt_t joke_reference);

#endif