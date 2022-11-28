#include <stdlib.h>
#include <string.h>

#include <pandoc.h>

int done = 0;
const char test[] = "$\\textit{K}$ -trivial, $\\textit{K}$ -low and ${{\\mathrm{\\textit{MLR}}}}$ -low Sequences: A Tutorial";
const int BUFFER_LENGTH = 1024;

int reader(char *buf, void *user_data) {
    if (done) {
        return 0;
    }
    strncpy(buf, test, BUFFER_LENGTH);
    done = 1;
    return strlen(test);
}

void writer(const char *buf, int len, void *user_data) {
    fwrite(buf, 1, len, stdout);
}

int main() {
    pandoc_init();
    pandoc(BUFFER_LENGTH, "markdown", "plain", NULL, &reader, &writer, NULL);
    printf("\n");      /* if desired */
    pandoc_exit();
}