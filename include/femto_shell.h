#ifndef _FEMTO_SHELL_H_
#define _FEMTO_SHELL_H_

void get_joke_prompt(void);
boolean_t parse_command(void);
boolean_t execute_command(void);
void set_commands_handlers(void);
boolean_t handle_redirections(void);
boolean_t reset_default_files(void);
void reset_tokens(void);

#endif