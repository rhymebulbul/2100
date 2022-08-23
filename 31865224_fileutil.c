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

int two(char path[]){
    //printf("%s\n",path);
    /*
     *  Function to output the first 10 words of the sample file.
     *  Shows entire file contents if the file contains less than 10 words.
     */
    int filedescriptor, wordlength;
    int wordsprinted = 0;
    char buffer[1024];
    int whitespaces[10];
//    if (path == NULL){
//        // Open file named sample in read only mode and store file descriptor
//        filedescriptor = open("sample", O_RDONLY);
//    }else{
        filedescriptor = open(path, O_RDONLY);
    //}
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

int main(int argc, char *argv[]) {
    //char* home;
    if (argc == 1){
        /*
         * Basic task one
         * Call one
         */
        //one();
        two("sample");
    } else if (argc >= 2){
        printf("%d\n", argc);
        //printf("%s\n", argv[1]);
        if (argv[1][0] != '-'){ // Check if argument is not -option
            if (argv[1][0] == '/'){ // Check if argument is path
                //home = getenv("HOME");
                //strcat(home, argv[1]);
                two(argv[1]);  // Call task two
            }
        }
    }
    //exit(0);
    return 0;
}
