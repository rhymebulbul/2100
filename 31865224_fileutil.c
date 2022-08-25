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
    int filedescriptor, wordlength;
    int destinationdescriptor = 1;
    int wordsprinted = 0;
    char buffer[1024];
    int whitespaces[numwords];
    // Open file from given path in read only mode and store file descriptor
    filedescriptor = open(sourcefile, O_RDONLY);
    // Check if file descriptor is valid
    if ( filedescriptor<0 ) { // Print error if failed to open file and exit with status one
        perror(strerror(1));
        exit(1);
    } else { // Proceed if file opened succeessfully
        // Read file length in number of chars
        int filelength = read(filedescriptor, buffer, 1024);
        // Iterate over each char and check for whitespaces
        for (int i = 0; i < filelength; ++i) {
            if (isspace(buffer[i])){ // Checks if the current char is a whitespace
                if (!isspace(buffer[i+1])){ // Checks if the next char is a char and not a whitespace
                    whitespaces[wordsprinted] = i; // Stores the location of the whitespace
                    wordsprinted++; // Increments counter for stored whitespace locations
                }
            }
        }
        // Check to display or append
        if (destfile){  // Append to destination file if provided
            destinationdescriptor = open(destfile, O_WRONLY | O_APPEND | O_CREAT, 0664);   // Open fd to destination file in append
        }
        // Print first word
        lseek(filedescriptor, 0, SEEK_SET); // Move file offset to beginning of file
        read(filedescriptor, buffer, whitespaces[0]);   // Reads in chars by char till first whitespace
        //lseek(destinationdescriptor, 0, SEEK_SET); // Move file offset to beginning of file
        write(destinationdescriptor, buffer, whitespaces[0]);   // Writes out first word to console
        // Checks if there are more than 10 words in sample file and ignores the rest
        if (wordsprinted>numwords-1) { // Check if more than 10 words
            wordsprinted = numwords-1; // Ignore words after the 10th
        }
        // Print rest of the words
        for(int i=0; i<wordsprinted; i++){  // Iterate over at least first 10 words of sample file
            lseek(filedescriptor, whitespaces[i], SEEK_SET);    // Move file offset to start of this word
            wordlength = whitespaces[i+1] - whitespaces[i]; // Get length of word to print
            read(filedescriptor, buffer, wordlength);   // Read word into buffer
            //lseek(destinationdescriptor, whitespaces[i], SEEK_SET);
            write(destinationdescriptor, buffer, wordlength);   // Print word from buffer
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
