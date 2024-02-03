#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/utilities.h"
#include "../include/mymv.h"

extern size_t argc;
extern uint8_t tokens[TOCKENS_COUNT][COMMAND_SIZE];

void execute_move(void)
{
    if (argc != 3)
    {
        printf("Usage Error: ./mymv src dest");
        return;
    }
    int src_fd, des_fd;
    src_fd = open(tokens[1], O_RDONLY);
    if (src_fd == FILE_OPEN_ERROR)
    {
        perror("Open Src File error: ");
        return;
    }
    des_fd = open(tokens[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    ssize_t read_bytes, written_bytes;
    uint8_t buffer[READ_BUFFER_SIZE];
    while ((read_bytes = read(src_fd, &buffer, READ_BUFFER_SIZE)) != 0)
    {
        printf("Read : %d\n", read_bytes);
        if (read_bytes < 0)
        {
            perror("Reading src file error: ");
            return;
        }
        written_bytes = write(des_fd, buffer, read_bytes);
        printf("Write : %d\n", written_bytes);
        if (written_bytes < 0)
        {
            perror("Writing dest file error: ");
            return;
        }
    }
    if (close(src_fd) != SUCCESSFUL_CLOSE)
    {
        perror("Failed to close src: ");
        return;
    }
    if (close(des_fd) != SUCCESSFUL_CLOSE)
    {
        perror("Failed to close src: ");
        return;
    }
    if (remove(tokens[1]) != SUCCESSFUL_REMOVE)
    {
        perror("Failed to move : ");
        return;
    }
}