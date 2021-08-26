/*
 * Program based on the utility look.
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

static char *prefix;

/*
 * Helper method to print error message to stdout and exit the program.
 */
void error(char *string) {
    printf("my-look: %s\n", string);
    exit(1);
}

/*
 * Helper method to print help information about the program.
 */
void help() {
    printf("Program based on the utility look.\n");
    printf("Prints all lines from the given file starting with 'prefix'.\n");
    printf("Command usage: ./my-look [options] [filename] prefix\n");
    printf("Optional arguments:\n");
    printf("-V : Prints information about the utility and exits.\n");
    printf("-h : Prints help information about the utility and exits.\n");
    printf("-f : Uses the following argument as the input dictionary.\n");
    printf("Without the -f option, stdin is used for dictionary input.\n");
    exit(0);
}

/*
 * Helper method to compare lines from input/file stream with prefix string.
 */
void compare(FILE *stream) {
    char *line = NULL;  // getline() allocates memory to buffer
    size_t length = 0;
    while (getline(&line, &length, stream) != -1) {  // Reading stream
        int compare = strncasecmp(line, prefix, strlen(prefix));
        if (compare == 0) {
            printf("%s", line);  // Printing matching lines
        }
    }
    free(line);  // Freeing buffer memory
}

/*
 * Helper method to check for invalid command usage.
 */
void checkargs(int argc, char* argv[], const char * optstring) {
    extern int optind;
    int option;

    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'V' :
                printf("my-look from CS537 Summer 2021\n");
                exit(0);
            case 'h' :
                help();
            case '?' :  // Any invalid option will make the program exit
                error("invalid command line");
            default :  // -f was used without a filename
                break;
        }
    }

    // Checking the number of (non-option) arguments
    if (((argc - 1) > optind) || (argc < 2)) {
        help();
    }
}

int main(int argc, char* argv[]) {
    const char* optstring = ":Vhf:";
    char *filename = NULL;
    FILE *file;
    int option;
    extern char *optarg;

    checkargs(argc, argv, optstring);  // Checking command usage
    optind = 1;  // Resetting optind

    // Parsing argv[] for optional arguments using getopt() and handling cases
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'f' :  // uses next argument as name of input dictionary
                filename = optarg;
                break;
            default :  // Invalid optional argument
                error("invalid command line");
        }
    }

    prefix = argv[optind];

    if (filename != NULL) {  // Case 1 : Filename provided
        file = fopen(filename, "r");
        if (file == NULL) {
            error("cannot open file");  // Checking fopen() return value
        }
        compare(file);

        int close = fclose(file);
        if (close != 0) {
            error("cannot close file");  // Checking fclose() return value
        }
    } else {  // Case 2 : Input from stdin
        compare(stdin);
    }

    return 0;
}
