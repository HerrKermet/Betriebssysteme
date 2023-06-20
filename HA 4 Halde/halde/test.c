#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "halde.h"

int main(int argc, char *argv[]) {
    printList();
    
	//allocate 4 different sized pointer
    char *m1 = malloc(16);
    printList();
    
    char* m2 = malloc(48);
    printList();
    
    char* m3 = malloc(80);
    printList();
    
    char* m4 = malloc(16);
    printList();    

	//free mentioned pointer
    free(m1);
    printList();
    
    
    free(m2);
    printList();
    
    free(m3);
    printList();
    
    free(m4);
    printList();
    
    //allocate ptr with enough size to check each case:
    // 1: no free struct has enough space
    m1 = malloc(1024);
    printList();
    // 2: struct has exactly enough space
    m2 = malloc(16);
    printList();
    // 3: struct has enough space but not enough for a new struct
    m3 = malloc(47);
    printList();
    // 4: struct has enough space for struct and size
    m4 = malloc(64);
    printList(); 
    
    
    //free each ptr
    free(m1);
    printList();
    
    
    free(m2);
    printList();
    
    free(m3);
    printList();
    
    free(m4);
    printList();
    
    //Edgecase  size is bigger than size (1024 * 1024) in this case
    char* edgeCasePtr = malloc(1024 * 1024 +1);
	printList();
	
	free(edgeCasePtr);
	printList();
	
    

    exit(EXIT_SUCCESS);
}
