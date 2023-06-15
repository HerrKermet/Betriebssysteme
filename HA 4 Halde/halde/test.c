#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "halde.h"

int main(int argc, char *argv[]) {
    printList();

    char *m1 = malloc(16);
    printList();
    
    char* m2 = malloc(48);
    printList();

    free(m1);
    printList();
    
    char* m3 = malloc(80);
    printList();
    
    free(m2);
    printList();
    
    free(m3);
    printList();

    exit(EXIT_SUCCESS);
}
