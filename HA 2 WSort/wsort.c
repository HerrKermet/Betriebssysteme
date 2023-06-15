#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
//compare function for qsort
static int cmp(const void* p1, const void* p2){
    return strcmp(* (char * const *) p1, * (char * const *) p2);
}

//free memory if error occured
static void freeMemory(char** arr, int length){
    if(length < 0)
        return;
    for(int i = 0; i < length; free(arr[i++]));
    free(arr);
}

int main(int argc, char** argv) {
// initialize MAX and buffer with size MAX + 2 for the \n and the \0
const int MAX = 100;
char buffer[MAX + 2];

//init string array and check if malloc failed
char** words = malloc(sizeof(char*));

if(words == NULL)
{
    perror("Malloc failed");
    exit(EXIT_FAILURE); //Malloc failed
}

// check how many lines are currently read
int lines = 0;

//read line by line from stdin with (Max + 2) - 1 to read everything with a length of 101 and throw error if 101 symbol is not \n
while(fgets(buffer, MAX + 2, stdin) != NULL) {

    // check if string is empty
    int empty = 1;
    for(int i = 0; i < MAX + 2; i++){
        if(buffer[i] == '\0')
            break;      //break if end of string is reached
        if(!isspace(buffer[i])){
            empty = 0;  //if string contains non "empty" char then proceed and insert it to words
            break;
        }
    }
    if(empty)
        continue;

    // if last char is not \n then word was too long
    if(buffer[strlen(buffer) - 1] != '\n') {
        fprintf(stderr, "Error: line too long\n");
        if(errno)
            perror("Error: "); // if printf sets errno

        // clear buffer after a line was too long
        char c = 0;
        while ((c != '\n' && c != EOF)) {
            c = getchar();
            if(errno)
                perror("Error: "); // if getchar sets errno
        }

        continue;
    }

    // realloc string array with new size and check for failure of realloc
    char** newPtr = realloc(words, (lines + 1) * sizeof(char*));
    if (newPtr == NULL)
    {
        freeMemory(words, lines);
        perror("Malloc failed");
        exit(EXIT_FAILURE); //Malloc failed
    }
    // if realloc did not failed then proceed
    words = newPtr;
    lines++;

    words[lines - 1] = malloc(sizeof(buffer) + 1);
    if(words[lines - 1] == NULL)
    {
		freeMemory(words, lines);
        perror("Malloc failed");
        exit(EXIT_FAILURE); //Malloc failed
    }
    strcpy(words[lines - 1], buffer);
}

qsort(words, lines, sizeof(char*), cmp);


for(int i = 0; i < lines; i++){
    fputs(words[i], stdout);
    if(errno)
        perror("Error: ");
}

//free allocated memory
freeMemory(words, lines);


exit(EXIT_SUCCESS);
}
