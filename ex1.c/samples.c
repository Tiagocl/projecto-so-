#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    int n,m;
    time_t t;
    FILE *file; 

    file = fopen(argv[1], "r");
    n = atoi(argv[2]);
    m = atoi(argv[3]);

    // fseek(file, 10,SEEK_SET);
    // printf

    fseek(file, 0, SEEK_END); 
    size_t length = ftell(file)-m;

    char *str = (char*)malloc(m); //array de char 
    int d;
    srand((unsigned) time(&t));
    for(int i =0;i<n;i++) {
        d = rand() % length;
        printf(">");
        fseek(file,d,SEEK_SET); // apontar para a pos d;
        
        fread(str,1,m,file);//ler m char de size 1 a de file para str
        if (str == "\n") {
            continue;
        }
        fputs(str,stdout);
        printf("<\n");
     
    }
    free(str);
    fclose(file);

    return 0;
}