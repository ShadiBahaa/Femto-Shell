#ifndef _FEMTO_SHELL_H_
#define _FEMTO_SHELL_H_

typedef void (*command_handler_t)(void);

void get_joke_prompt(void);
void parse_command(void);
void execute_command(void);

#endif