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
    char str[] = "Success!\n";
    char buffer[1024];
    int fd;
    int whitespaces[10];
    int j=0;


    if (argc == 2){
        // use given file
        char* home = getenv("HOME");
        strcat(home, argv[1]);
        fd = open(home, O_RDONLY);
    } else {
        // Use sample file
        fd = open("sample", O_RDONLY);
    }

  


    
    


    if ( fd < 0 ) {
        perror("File does not exist!");
        exit(1);
    } else {

        int r = read(fd, buffer, 1024);

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
        write(1, buffer, whitespaces[0]);
        
        if(j>9){
            j = 9;
        }
        //printf("%d\n", j);
        for(int i=0; i<j; i++){
            lseek(fd, whitespaces[i], SEEK_SET);
            int l = whitespaces[i+1]-whitespaces[i];
            read(fd, buffer, l);
            write(1, buffer, l);
        }
        exit(0);
        
    }


    close(fd);
    
    return 0;
}

