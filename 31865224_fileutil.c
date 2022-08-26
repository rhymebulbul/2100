/*
 * FIT2100
 * Programming Assignent Part A
 * Rhyme Bulbul - 31865224
 * 19/8/22 - 26/9/22
 */
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

int fileutil(char *sourcefile, char *destfile, int numwords){
    /*
     *  Function to output the first 10 words of the file from given path
     *  Shows entire file contents if the file contains less than 10 words.
     *
     *  @sourcefile: absolute path to file to open
     *  @destfile: absolute path to file to append to
     *  @numwords: number of words to write from source file
     *  Output: displays or writes number of words from source file
     */

    int filedescriptor,  written;
    char* appendsuccessful ="Append successful!\n";
    int destinationdescriptor = 1;
    int wordsprinted = 0;
    int buffersize = 65536;
    char buffer[buffersize];
    int whitespaces[numwords];
    int filelength = 0;
    // Check to display or append
    if (destfile){  // Append to destination file if provided
        destinationdescriptor = open(destfile, O_WRONLY | O_APPEND | O_CREAT, 0664);   // Open fd to destination file in append
    }
    // Open file from given path in read only mode and store file descriptor
    filedescriptor = open(sourcefile, O_RDONLY, 0664);
    // Check if file descriptor is valid
    if ( filedescriptor<0 ) { // Print error if failed to open file and exit with status one
        perror(strerror(1));
        exit(1);
    } else { // Proceed if file opened succeessfully
        // Read file length in number of chars
        filelength = read(filedescriptor, buffer, buffersize);
        for (int i = 0; i < filelength; ++i){   // Iterate over all characters in file
            if(wordsprinted < numwords){    // Only iterates over first n words
                if(isspace(buffer[i])){     // Checks for whitespace
                    whitespaces[wordsprinted] = i;  // Stores locations of each whitespace
                    wordsprinted++;     // Increment count of whitespaces and so words counted
                }
            }
        }
        // Write the first numwords at least from file
        written = write(destinationdescriptor, buffer, whitespaces[wordsprinted-1]);   // Writes out first word to console
        if (written > 0 && destinationdescriptor > 1){  // check if append successful
            write(1, appendsuccessful, strlen(appendsuccessful));
        }
        close(filedescriptor);
        close(destinationdescriptor);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    /*
     * Checks command line arguments and calls function with appropriate arguments passed
     * @argc: number of command line arguments passed
     * @argv: vector representing all command line arguments passed
     * @return: 0 if completed normally
     */
    char* sourcefile = "sample";
    char* destfile = NULL;
    int numwords = 10;
    if (argc >= 2){
        if (argv[1][0] != '-'){ // Check if argument is not -option
            if (argv[1][0] == '/'){ // Check if argument is path
                sourcefile = argv[1];
            }
        } // Check if argument is a -option
        for(int i=1; i<argc; i++){  // Iterate over arguments passed
            if (argv[i][0] == '-'){ // Double check we are specifying an option as an argument
                if (argv[i][1] == 'a'){ // Checks if second character is -a
                    destfile = argv[i+1];
                } else if (argv[i][1] == 'n'){  // Checks if second character is -n
                    numwords = atoi(argv[i+1]);
                }
                i++; // Skip over next argument as that is data we already worked with
            }
        }
    }
    fileutil(sourcefile, destfile, numwords);  // Call fileutil function with passed arguments
    return 0;
}
