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
    
    char* m3 = malloc(80);
    printList();
    
    char* m4 = malloc(16);
    printList();    

    free(m1);
    printList();
    
    
    free(m2);
    printList();
    
    free(m3);
    printList();
    
    free(m4);
    printList();
    
    m1 = malloc(1024);
    printList();
    
    m2 = malloc(16);
    printList();
    
    m3 = malloc(47);
    printList();
    
    m4 = malloc(64);
    printList();  
    
    free(m1);
    printList();
    
    
    free(m2);
    printList();
    
    free(m3);
    printList();
    
    free(m4);
    printList();
    

    exit(EXIT_SUCCESS);
}
