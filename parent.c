#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CHILD_TO_PARENT "CHILD_TO_PARENT"
#define PARENT_TO_CHILD "PARENT_TO_CHILD"

#define BUF_SIZE 0x100

int main(void)
{   
    printf("Starting Parent...\n");

    mkfifo(PARENT_TO_CHILD, 0666);
    mkfifo(CHILD_TO_PARENT, 0666);
    printf("Created Pipe Files!\n");

    int child_out = open(PARENT_TO_CHILD, O_WRONLY);
    printf("Opened Parent to Child Pipe!\n");

    int child_in = open(CHILD_TO_PARENT, O_RDONLY);
    printf("Opened Child to Parent Pipe!\n");

    while(1)
    {
        char buffer[BUF_SIZE] = {};

        printf("rot13> ");
        if(fgets(buffer, BUF_SIZE, stdin))
        {
            write(child_out, buffer, BUF_SIZE);

            char returned[BUF_SIZE] = {};
            read(child_in, returned, BUF_SIZE);

            printf(" = %s\n\n", returned);
        }
        else break;
    }

    printf("Exiting...\n");
    close(child_in);
    close(child_out);
    exit(0);
}