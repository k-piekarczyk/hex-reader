#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define RESET  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"
#define GREY "\033[01;30m"

#define LINE_LEN 16

unsigned int print_line(unsigned char *buffer, size_t read, unsigned int address) {
    printf(GREY "%08x " RED "> " RESET, address);

    for (int i = 0; i < LINE_LEN; i++) {
        if (i != 0 && i % (LINE_LEN / 2) == 0) {
            if (read > (LINE_LEN / 2))
                printf(GREY " -  " RESET);
            else
                printf("    ");
        }

        if (i >= read)
            printf(RESET "   ");
        else if (i % 2 == 0)
            printf(GREEN "%02x " RESET, buffer[i]);
        else
            printf(YELLOW "%02x " RESET, buffer[i]);
    }

    printf(RED "| " RESET);

    for (int i = 0; i < LINE_LEN; i++) {

        if (i != 0 && i % (LINE_LEN / 2) == 0) {
            if (read > (LINE_LEN / 2))
                printf(GREY " -  " RESET);
            else
                printf("    ");
        }

        if (i >= read)
            printf(RESET "  ");
        else if (buffer[i] == 0)
            printf(GREY "0 " RESET);
        else if (isspace(buffer[i]))
            printf(GREY ". " RESET);
        else if (!isprint(buffer[i]))
            printf(GREY "? " RESET);
        else
            printf(BLUE "%1c " RESET, buffer[i]);
    }

    printf(RESET "\n");
    address += LINE_LEN;
    return address;
}

void read_file(char *file_name) {
    unsigned char buffer[LINE_LEN];
    FILE *fp;

    fp = fopen(file_name, "rb");

    if (fp == NULL) {
        printf("No file: %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    size_t nread;
    unsigned int addres = 0;

    do {
        nread = fread(buffer, 1, LINE_LEN, fp);
        addres = print_line(buffer, nread, addres);
    } while (nread == LINE_LEN);

    fclose(fp);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("No file name given!\n");
        exit(EXIT_FAILURE);
    }

    read_file(argv[1]);

    printf(RESET);
    return 0;
}