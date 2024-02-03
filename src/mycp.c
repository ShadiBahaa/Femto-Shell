#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../include/utilities.h"
#include "../include/mycp.h"

extern size_t argc;
extern uint8_t tokens[TOCKENS_COUNT][COMMAND_SIZE];

void execute_copy(void){
    int fd_src, fd_des, num_read, num_write;
    uint8_t buf[READ_BUFFER_SIZE];

    if (argc != 3)
    {
        printf("Usage: %s file_name file_name\n", tokens[0]);
        return;
    }

    fd_src = open(tokens[1], O_RDONLY);
    if (fd_src < 0)
    {
        printf("Could not open the source file\n");
        return;
    }
    fd_des = open(tokens[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_des < 0)
    {
        printf("Could not open the dest file\n");
        return;
    }
    while ((num_read = read(fd_src, buf, READ_BUFFER_SIZE)) != 0)
    {
        if (num_read < 0)
        {
            printf("Could not read from the file\n");
            return;
        }

        num_write = write(fd_des, buf, num_read);
        if (num_write < 0)
        {
            printf("Could not write to stdout\n");
            return;
        }
    }
    if (close(fd_src) != SUCCESSFUL_CLOSE)
    {
        perror("Failed to close src: ");
        return;
    }
    if (close(fd_des) != SUCCESSFUL_CLOSE)
    {
        perror("Failed to close src: ");
        return;
    }
    return;
}
