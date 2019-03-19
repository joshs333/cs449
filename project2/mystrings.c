/********************************************************************************
 * @file mystrings.c
 * @brief prints the ASCII strings of length 4+ in a file
 * @author joshs333@live.com (jjs231@pitt.edu)
 *******************************************************************************/
#include <stdio.h> /* printf() fseek() fwrite() fopen() fread() */
#include <string.h> /* strcmp() strcpy() */
/**
 * @brief help() prints a help message for the user
 **/
void help() {
    printf("mystrings.c is a utility to print all the ASCII strings from a file.\n");
    printf("Usage:\n");
    printf("    mystrings [options] -a <file_name>\n");
    printf("\n");
    printf("options:\n");
    printf("    -h | --help                 prints this help message\n");
}

int main(int argc, char** argv) {
    if(argc < 2) {
        help();
        return 0;
    }
    char filename[40];
    int filename_given = 0;
    int help_printed = 0;
    
    // iterate through arguments
    int i = -1;
    while(i < argc - 1) {
        //increment at beginning.
        ++i;
        if(argv[i][0] == '-') {
            // --args 
            if(argv[i][1] == '-') {
                // help
                if(strcmp("help", argv[i] + 2) == 0) {
                    if(!help_printed) { //(only print help once)
                        help_printed = 1;
                        help();
                    }
                    continue;
                }
                printf("(0)Error! Unknown option: %s\n", argv[i]);
                continue;
            }
            // -h
            if(argv[i][1] == 'h') {
                if(!help_printed) {
                    help_printed = 1;
                    help();
                }
                continue;
            }
            // -a (gets the filename)  
            if(argv[i][1] == 'a') {
                if(argc == i + 2) {
                    filename_given = 1;
                    strcpy(filename, argv[i+1]);
                    ++i;
                    continue;
                } else {
                    printf("(1)Error! Not enough args to provide argument!\n");
                    continue;
                }
            }
            printf("(2)Error! Unknown option: %s\n", argv[i]);
            continue;
        }
    } // end argument loop

    // exit if no filename was given
    if(!filename_given) {
        printf("(3)Error! No filename provided!\n");
        if(!help_printed) {
            help();
        }
        return 0;
    }

    // File Opening here
    FILE *fp;
    fp = fopen(filename, "rb");
    long int size = 0;

    // go to end of file, get length, return to beginning
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // The current character
    // expressed in a null terminated string
    char current[2];
    current[1] = 0;

    // variables to track position    
    int first_line = 1;
    // store the first 4 characters until we get a printable string
    char collected[5];
    collected[4] = 0;
    int collected_count = 0;
    while(size > 0) {
        // get a new char and see if it's a valid ASCII character
        fread(current, sizeof(char), 1, fp);
        if((current[0] >= 32 && current[0] <= 126) || current[0] == 9) {
            // if we havent reached 4 chars cache the string
            if(collected_count < 4) {
                collected[collected_count] = *current;
                ++collected_count;
            }
            // once we hit the 4th we can print the caches characters
            if(collected_count == 4) {
                if(!first_line) {
                    printf("\n");
                }
                first_line = 0;
                collected_count = 5;
                printf("%s", collected);
            // after that we can just print the current character
            } else if(collected_count == 5) {
                printf("%s", current);
            }
        } else {
            // once we hit a non-valid character we reset
            collected_count = 0;
        }
        // decrement remaining characters
        --size;
    }
    // the last string we printed won't have a new line so we give it one.
    printf("\n");
    // exit! :)
    return 0;
} /* main() */
