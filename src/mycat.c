#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../include/utilities.h"
#include "../include/mycat.h"

extern size_t argc;
extern uint8_t tokens[TOCKENS_COUNT][COMMAND_SIZE];

void execute_cat(void)
{
    int fd, num_read, num_write;
    uint8_t buf[READ_BUFFER_SIZE];

    if (argc != 2)
    {
        printf("Usage: %s file_name\n", tokens[0]);
        return;
    }

    fd = open(tokens[1], O_RDONLY);
    if (fd < 0)
    {
        printf("Could not open the file\n");
        return;
    }

    while ((num_read = read(fd, buf, READ_BUFFER_SIZE)) != 0)
    {
        if (num_read < 0)
        {
            printf("Could not read from the file\n");
            return;
        }

        num_write = write(1, buf, num_read);
        if (num_write < 0)
        {
            printf("Could not write to stdout\n");
            return;
        }
    }

    if (close(fd) != SUCCESSFUL_CLOSE)
    {
        perror("Failed to close file: ");
        return;
    }

    return;
}
