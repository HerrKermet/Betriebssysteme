#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int cmp(const void* p1, const void* p2){
    return strcmp(* (char * const *) p1, * (char * const *) p2);
}

int main(int argc, char** argv) {
const int MAX = 102;
char buffer[102];
char** words = malloc(MAX * sizeof(char));
if(words == NULL)
    exit(EXIT_FAILURE); //Malloc failed
int lines = 0;

while(fgets(buffer, MAX-100, stdin) != NULL && !feof(stdin)) {
    lines++;
    words = realloc(words, lines * MAX * sizeof(char));
    if (words == NULL)
        exit(EXIT_FAILURE); //malloc failed

    words[lines - 1] = malloc(sizeof(buffer) + 1);
    if(words[lines - 1] == NULL)
        exit(EXIT_FAILURE); //malloc failed
    strcpy(words[lines - 1], buffer);
}

qsort(words, lines, sizeof(char*), cmp);

for (int i = 0; i < lines; i++) {
    printf("%s", words[i]);
}




exit(EXIT_SUCCESS);
}
