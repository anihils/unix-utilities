/*
 * Program based on the utility rev.
 *
 *  Optional arguments -
 * -V : prints information about the utility
 * -h : prints help information about the utility
 * -f <filename> : <filename> is the input dictionary
 * Copyright 2021 Shalini Ramakrishnan
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

static char *filename = NULL;

/*
 * Helper method to print error message to stdout and exit the program.
 */
void invalid(char *string) {
    printf("my-rev: %s\n", string);
    exit(1);
}

/*
 * Helper method to print help information about the program and exit.
 */
void help() {
    printf("Program based on the utility rev.\n");
    printf("Reverses all lines in the given file.\n");
    printf("Command usage: ./my-rev [options] [filename]\n");
    printf("Optional arguments:\n");
    printf("-V : Prints information about the utility and exits.\n");
    printf("-h : Prints help information about the utility and exits.\n");
    printf("-f : Uses the following argument as the input file.\n");
    printf("Without the -f option, stdin is used for input.\n");
    exit(0);
}

/*
 * Helper method to reverse lines from input/file stream.
 */
char *reverse(char *line) {
    char temp;
    int middle = (strlen(line)/2);
    for (int i = 0; i < middle; i++) {
        int k = i + 1;  // distance i from last character
        if (*(line + strlen(line) - 1) == '\n') {
            k++;  // ignoring null character
        }
        temp = *(line + i);
        *(line + i) = *(line + strlen(line) - k);
        *(line + strlen(line) - k) = temp;
    }
    return line;
}

/*
 * Helper method to check validity of arguments.
 */
void printrev(FILE *stream) {
    char *line = NULL;
    size_t len = 100;

    while (getline(&line, &len, stream) != -1) {  // Reading input
        printf("%s", reverse(line));  // Printing reversed lines
        if (feof(stdin)) {
            break;
        }
    }
    free(line);  // Freeing buffer
}

/*
 * Helper method to check validity of arguments.
 */
void checkargs(int argc, char* argv[], const char *optstring) {
    extern int optind;
    int option;

    // Parsing all option characters
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'V' :  // prints information about the utility
                printf("my-rev from CS537 Summer 2021\n");
                exit(0);
            case 'h' :  // prints help information about the utility
                help();
            case '?' :
                invalid("invalid command line");
            case ':' :  // -f was used without a filename
                invalid("invalid command line");
            default :
                break;
        }
    }

    // Checking the number of required(non-optional) arguments
    if (optind != argc) {
        invalid("invalid command line");
    }
}

int main(int argc, char* argv[]) {
    FILE *file;
    int option;
    extern char *optarg;
    const char* optstring = ":Vhf:";

    checkargs(argc, argv, optstring);
    optind = 1;  // Resetting optind to parse options again

    // Parsing argv[] for optional arguments using getopt() and handling cases
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'f' :  // uses next argument as name of input file
                filename = optarg;
                break;
            default :  // Invalid optional argument
                invalid("invalid command line");
        }
    }

    // Reversing stream
    if (filename != NULL) {  // Case 1 : Filename provided
        file = fopen(filename, "r");
        if (file == NULL) {
            invalid("cannot open file");  // Checking fopen() return value
        }

        printrev(file);  // Prinintg reversed lines from file

        int close = fclose(file);  // Closing rewritten file
        if (close != 0) {
            invalid("cannot close file");
        }
    } else {  // Case 2 : Input from stdin
        printrev(stdin);  // // Prinintg reversed lines from stdin
    }

    return 0;
}
