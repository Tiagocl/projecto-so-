#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h> 
#include <unistd.h> 

int main(int argc, char* argv[]) {
    char extension[] = ".epub";
    int value;
    

        for (int i = 1; i < argc; i++) {
            if (fork() == 0) {
                int length = strlen(argv[i]);

                char *str = (char*)malloc(length);
                strcpy(str,argv[i]);

                char *ptr = (char*)malloc(length+1);
                char *x; 
                x = strrchr(argv[i], '.');
                strcpy(x,extension);
                strcat(ptr,argv[i]);

                value = rename(str,ptr);

                printf ("[%d] converting %s...\n", getpid(),str);

                free(str);
                free(ptr);
                exit(0);
            }
        }

    return 0;
}