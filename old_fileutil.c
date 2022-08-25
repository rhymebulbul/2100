//
// Created by rhyme on 23/08/22.
//
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include<stdio.h>

int one(){
    /*
     *  Function to output the first 10 words of the sample file.
     *  Shows entire file contents if the file contains less than 10 words.
     */
    int filedescriptor, wordlength;
    int wordsprinted = 0;
    char buffer[1024];
    int whitespaces[10];
    // Open file named sample in read only mode and store file descriptor
    filedescriptor = open("sample", O_RDONLY);
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
        // Print first word
        lseek(filedescriptor, 0, SEEK_SET); // Move file offset to beginning of file
        read(filedescriptor, buffer, whitespaces[0]);   // Reads in chars by char till first whitespace
        write(1, buffer, whitespaces[0]);   // Writes out first word to console
        // Checks if there are more than 10 words in sample file and ignores the rest
        if (wordsprinted>9) { // Check if more than 10 words
            wordsprinted = 9; // Ignore words after the 10th
        }
        // Print rest of the words
        for(int i=0; i<wordsprinted; i++){  // Iterate over at least first 10 words of sample file
            lseek(filedescriptor, whitespaces[i], SEEK_SET);    // Move file offset to start of this word
            wordlength = whitespaces[i+1] - whitespaces[i]; // Get length of word to print
            read(filedescriptor, buffer, wordlength);   // Read word into buffer
            write(1, buffer, wordlength);   // Print word from buffer
        }
        close(filedescriptor);
    }
    return 0;
}

int two(char sourcefile[]){
    /*
     *  Function to output the first 10 words of the file from given path
     *  Shows entire file contents if the file contains less than 10 words.
     *  @path[]: absolute path to file to open
     */
    int filedescriptor, wordlength;
    int wordsprinted = 0;
    char buffer[1024];
    int whitespaces[10];
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
        // Print first word
        lseek(filedescriptor, 0, SEEK_SET); // Move file offset to beginning of file
        read(filedescriptor, buffer, whitespaces[0]);   // Reads in chars by char till first whitespace
        write(1, buffer, whitespaces[0]);   // Writes out first word to console
        // Checks if there are more than 10 words in sample file and ignores the rest
        if (wordsprinted>9) { // Check if more than 10 words
            wordsprinted = 9; // Ignore words after the 10th
        }
        // Print rest of the words
        for(int i=0; i<wordsprinted; i++){  // Iterate over at least first 10 words of sample file
            lseek(filedescriptor, whitespaces[i], SEEK_SET);    // Move file offset to start of this word
            wordlength = whitespaces[i+1] - whitespaces[i]; // Get length of word to print
            read(filedescriptor, buffer, wordlength);   // Read word into buffer
            write(1, buffer, wordlength);   // Print word from buffer
        }
        close(filedescriptor);
    }
    return 0;
}

int three(char *sourcefile, char *destfile, char *numwords){
    /*
     *  Function to output the first 10 words of the file from given path
     *  Shows entire file contents if the file contains less than 10 words.
     *  @path[]: absolute path to file to open
     */
    int filedescriptor, wordlength;
    int destinationdescriptor = 1;
    int wordsprinted = 0;
    char buffer[1024];
    int whitespaces[10];
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
            printf("opened!\n");
            destinationdescriptor = open(destfile, O_WRONLY | O_APPEND | O_CREAT, 0664);   // Open fd to destination file in append
            printf("dd %d\n", destinationdescriptor);
        }
        // Print first word
        lseek(filedescriptor, 0, SEEK_SET); // Move file offset to beginning of file
        read(filedescriptor, buffer, whitespaces[0]);   // Reads in chars by char till first whitespace
        //lseek(destinationdescriptor, 0, SEEK_SET); // Move file offset to beginning of file
        write(destinationdescriptor, buffer, whitespaces[0]);   // Writes out first word to console
        // Checks if there are more than 10 words in sample file and ignores the rest
        if (wordsprinted>9) { // Check if more than 10 words
            wordsprinted = 9; // Ignore words after the 10th
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
        if (destfile){  // Append to destination file if provided
            printf("closed!\n");
            close(destinationdescriptor);
        }
    }
    return 0;
}

int four(char *sourcefile, char *destfile, int numwords){
    printf("sourcefile:  %s\n", sourcefile);
    printf("destfile:  %s\n", destfile);
    printf("numwords:  %d\n", numwords);
    /*
     *  Function to output the first 10 words of the file from given path
     *  Shows entire file contents if the file contains less than 10 words.
     *  @path[]: absolute path to file to open
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
     *
     */
    char* sourcefile = "sample";
    char* destfile = NULL;
    int numwords = 10;
//    if (argc == 1){ // Check if no arguments are provided
//        //two("sample");  // Call util with sample file
//        //printf("sourcefile:%s\n", sourcefile);
//    }
//    else
    if (argc >= 2){
        if (argv[1][0] != '-'){ // Check if argument is not -option
            if (argv[1][0] == '/'){ // Check if argument is path
                //home = getenv("HOME");
                //strcat(home, argv[1]);
                sourcefile = argv[1];
                //printf("sourcefile:%s\n", sourcefile);
                //two(argv[1]);  // Call util passed sourcefile
            }
        } //else { // Check if argument is a -option
        for(int i=1; i<argc; i++){  // Iterate over arguments passed
            if (argv[i][0] == '-'){ // Double check we are specifying an option as an argument
                if (argv[i][1] == 'a'){ // Checks if second character is -a
                    //printf("%s\n", argv[i+1]);   //argv[i+1]
                    //three(argv[1])
                    destfile = argv[i+1];
                    //printf("destfile:%s\n", destfile);
                } else if (argv[i][1] == 'n'){  // Checks if second character is -n
                    //printf("%s\n", argv[i+1]);   //argv[i+1]
                    numwords = atoi(argv[i+1]);
                    //printf("numwords:%s\n", numwords);
                }
                i++; // Skip over next argument as that is data we already worked with
            }
        }
        //}
    }
    four(sourcefile, destfile, numwords);
    //exit(0);
    return 0;
}
