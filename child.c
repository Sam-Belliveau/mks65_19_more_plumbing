#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CHILD_TO_PARENT "CHILD_TO_PARENT"
#define PARENT_TO_CHILD "PARENT_TO_CHILD"

#define BUF_SIZE 0x100

static char rot13(char c)
{
    if('a' <= c && c <= 'z')
    {
        c -= 'a' - 13;
        if(c >= 26) c -= 26;
        c += 'a';
    }

    else if('A' <= c && c <= 'Z')
    {
        c -= 'A' - 13;
        if(c >= 26) c -= 26;
        c += 'A';
    }

    return c;
}

int main(void)
{   
    printf("Starting Child...\n");

    mkfifo(PARENT_TO_CHILD, 0666);
    mkfifo(CHILD_TO_PARENT, 0666);
    printf("Created Pipe Files!\n");

    int parent_in = open(PARENT_TO_CHILD, O_RDONLY);
    printf("Opened Parent to Child Pipe!\n");

    int parent_out = open(CHILD_TO_PARENT, O_WRONLY);
    printf("Opened Child to Parent Pipe!\n");

    int bytes_read;
    char buffer[BUF_SIZE];
    char *i;

    printf("Starting Main Loop...\n");
    while(1)
    {   
        memset(buffer, '\0', BUF_SIZE);
        bytes_read = read(parent_in, buffer, BUF_SIZE);

        if(bytes_read)
        {
            printf("[DATA RECIEVED] %s\n", buffer);
            for(i = buffer; *i; ++i) *i = rot13(*i);
            printf("[DATA SENT] %s\n", buffer);
            write(parent_out, buffer, bytes_read);
        } else break;
    }

    printf("Exiting...\n");
    close(parent_in);
    close(parent_out);
    exit(0);
}