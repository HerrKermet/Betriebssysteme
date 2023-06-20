#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "halde.h"

int main(int argc, char *argv[]) {
    printList();
    printf("%d\n",1);
    

    char *m1 = malloc(16);
    printList();
    printf("%d\n",2);
    
    char* m2 = malloc(48);
    printList();
    printf("%d\n",3);
    
    char* m3 = malloc(80);
    printList();
    printf("%d\n",4);
    
    char* m4 = malloc(16);
    printList();    
    printf("%d\n",4);

    free(m1);
    printList();
    printf("%d\n",5);
    
    
    free(m2);
    printList();
    printf("%d\n",6);
    
    free(m3);
    printList();
    printf("%d\n",7);
    
    free(m4);
    printList();
    printf("%d\n",8);
    
    m1 = malloc(1024);
    printList();
    printf("%d\n",9);
    
    m2 = malloc(16);
    printList();
    printf("%d\n",10);
    
    m3 = malloc(47);
    printList();
    printf("%d\n",11);
    
    m4 = malloc(64);
    printList(); 
    printf("%d\n",12); 
    
    free(m1);
    printList();
    printf("%d\n",13);
    
    
    free(m2);
    printList();
    printf("%d\n",14);
    
    free(m3);
    printList();
    printf("%d\n",15);
    
    free(m4);
    printList();
    printf("%d\n",16);
    

    exit(EXIT_SUCCESS);
}
