#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#define true 1
#define false 0
#define PROMPT_SIZE 1000
#define COMMAND_SIZE 1000
#define TOCKENS_COUNT 1000
#define DIRECTORY_BUFFER_SIZE 100
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
#define COMMANDS_COUNT 4
#define SUCCESSFUL_CD 0
#define SUCCESSFUL_SET_ENV 0
#define SYSTEM_FAILED -1
#define FILE_OPEN_ERROR -1
#define READ_BUFFER_SIZE 100
#define SUCCESSFUL_REMOVE 0
#define SUCCESSFUL_CLOSE 0

typedef unsigned char uint8_t;
typedef unsigned char shell_t;
typedef signed int iterator_t;
typedef unsigned char *string_t;
typedef unsigned char boolean_t;


#endif
