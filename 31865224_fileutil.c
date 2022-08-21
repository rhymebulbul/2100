/* 
Rhyme Bulbul 31865224 
19/8/22
*/

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include<stdio.h>



int main(int argc, char *argv[])
{
    char* append = "Append successful\n";
    char buffer[1024];
    int fd;
    int j=0;
    char* home;
    int destination = 1;
    int numwords = 10;



    if (argc == 2){
        // use given file
        home = getenv("HOME");
        strcat(home, argv[1]);
        fd = open(home, O_RDONLY);
    } else {
        // Use sample file
        fd = open("sample", O_RDONLY);
    }

    for (int i=1; i<argc; i++){
//        printf("%s\n", argv[i]);
//        printf("%c\n", argv[i][0]);
        if ( argv[i][0] == '-' ){
            if ( argv[i][1] == 'a' ){
                // destination: argv[i+1]
//                printf("Option: %s\n",argv[i]);
//                printf("Command: %s\n",argv[i+1]);
                home = getenv("HOME");
                strcat(home, argv[i+1]);
                destination = open(home, O_APPEND | O_CREAT);

                write(1, append, strlen(append));

            } else if ( argv[i][1] == 'n' ){
                // numword: argv[i+1]
//                printf("Option: %s\n",argv[i]);
//                printf("Command: %s\n",argv[i+1]);
                numwords = atoi(argv[i+1]);
                printf("%d\n", numwords);
            }
        }
    }

    if ( fd < 0 ) {
        perror("File does not exist!");
        exit(1);
    } else {
        int r = read(fd, buffer, 1024);
        int whitespaces[numwords];
        for (int i = 0; i < r; ++i) {
                if (isspace(buffer[i])){
                    if (!isspace(buffer[i+1])){
                        whitespaces[j] = i;
                        //printf("Whitespace (%d) is: %d\n",j, i);
                        j++;
                    }
            }
        }

        lseek(fd, 0, SEEK_SET);
        read(fd, buffer, whitespaces[0]);
        write(destination, buffer, whitespaces[0]);

        //printf("%d %d\n", numwords, j);

        if(j>numwords){
            j = numwords-1;
        }
        //printf("%d\n", j);
        for(int i=0; i<j; i++){
            lseek(fd, whitespaces[i], SEEK_SET);
            int l = whitespaces[i+1]-whitespaces[i];
            read(fd, buffer, l);
            write(destination, buffer, l);
        }


    }

    close(destination);

    close(fd);
    //exit(0);
    return 0;
}



