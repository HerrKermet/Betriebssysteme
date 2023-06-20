#include "halde.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/// Magic value for occupied memory chunks.
#define MAGIC ((void*)0xbaadf00d)

/// Size of the heap (in bytes).
#define SIZE (1024*1024*1)

/// Memory-chunk structure.
struct mblock {
	struct mblock *next;
	size_t size;
	char memory[];
};

/// Heap-memory area.
static char memory[SIZE];

/// Pointer to the first element of the free-memory list.
static struct mblock *head;

/// Helper function to visualise the current state of the free-memory list.
void printList(void) {
	struct mblock *lauf = head;

	// Empty list
	if (head == NULL) {
		char empty[] = "(empty)\n";
		write(STDERR_FILENO, empty, sizeof(empty));
		return;
	}

	// Print each element in the list
	const char fmt_init[] = "(off: %7zu, size:: %7zu)";
	const char fmt_next[] = " --> (off: %7zu, size:: %7zu)";
	const char * fmt = fmt_init;
	char buffer[sizeof(fmt_next) + 2 * 7];

	while (lauf) {
		size_t n = snprintf(buffer, sizeof(buffer), fmt
			, (uintptr_t) lauf - (uintptr_t)memory, lauf->size);
		if (n) {
			write(STDERR_FILENO, buffer, n);
		}

		lauf = lauf->next;
		fmt = fmt_next;
	}
	write(STDERR_FILENO, "\n", 1);
}

void *malloc (size_t size) {
	//case head is NULL therefore not initialized
	if(head == NULL)
	{
		//create new struct and init it
		head = (struct mblock*) memory;
		head->next = NULL;
		head->size = SIZE- sizeof(struct mblock);
	}
	
	if(size == 0){
		return NULL;
	}
	
	//TESTFALL  wenn size größer als memory ist
	if(size > SIZE - sizeof(struct mblock))
	{
		return NULL;
	}
	
	//check if there is enoug space and if so then return ptr
	//iterate over the linked list till NULL is found or a suitable memory block
	
	//declare iterator
	struct mblock* currMblock = head;
	struct mblock* prevMblock = head;
	
	//while not at the end of the list do ..
	while(currMblock->next != NULL){
		//check if current mblock has enough space
		if(currMblock->size >= size){
			//check if only space for the size or space for size and a new mblock
			if(currMblock->size >= (sizeof(struct mblock) + size))
			{
				//enough space for both
				
				//init new block
				struct mblock* newMblock =(struct mblock*) ((char*) currMblock + sizeof(struct mblock) + size);
				newMblock->size = currMblock->size - size - sizeof(struct mblock);
				newMblock->next = currMblock->next;
				
				currMblock->next = MAGIC;
				currMblock->size = size;
				
				//check if the element was the first of the list  (so it does not have a prev block)
				if(prevMblock == head)
				{
					head = newMblock;
				}
				else{
					prevMblock->next = newMblock;
				}
				
				void* toReturn = currMblock + 1;
				return toReturn;				
				
			}
			else{
				// only enough space for the size so we will give the whole mblock (mblock->size can only exceed size by 15 bytes which shouldnt be the problem)
				if(prevMblock == head)
					head = currMblock->next;
				else
					prevMblock->next = currMblock->next;
				
				//adjust currMblock but DONT ALTER SIZE because we need to know the actual size and not the requested size in this context
				currMblock->next = MAGIC;
				
				void* toReturn = currMblock + 1;
				return toReturn;
			}
		}
		prevMblock = currMblock;
		currMblock = currMblock->next;
	}
	
	//we iterated through the whole list and didnt found any matching space so check list end here
	if(currMblock->next == NULL){
		//same cases here  enough space for size and struct or only for size
		
		if(currMblock->size >= size){
			if(currMblock->size >= (sizeof(struct mblock) + size)){
				//init new block
				struct mblock* newMblock = (struct mblock*) ((char*) currMblock + sizeof(struct mblock) + size);
				newMblock->size = currMblock->size - sizeof(struct mblock) - size;
				newMblock->next = NULL;
				
				//adjust current block
				currMblock->next = MAGIC;
				currMblock->size = size;
				
				if(prevMblock == head){
					head = newMblock;
				}
				else
					prevMblock->next = newMblock;
				
				
				//return address
				void* toReturn = currMblock + 1;
				return toReturn;
			}
			else{
				// only enough space for the size so we will give the whole mblock (mblock->size can only exceed size by 15 bytes which shouldnt be the problem)
				if(prevMblock == head)
					head = currMblock->next;
				else
					prevMblock->next = currMblock->next;
				
				//adjust currMblock but DONT ALTER SIZE because we need to know the actual size and not the requested size in this context
				currMblock->next = MAGIC;
				
				void* toReturn = currMblock + 1;
				return toReturn;
				
			}
			
		}
	}
	return NULL;
}

void free (void *ptr) {
	if(ptr == NULL)
		return;
	//get corresponding mblock
	struct mblock* mbp = (struct mblock*) (((char*)ptr) - sizeof(struct mblock));
	
	//make sure that mbp->next is MAGIC
	if(mbp->next != MAGIC)
		return;
	
	//link head with current mbp and free ptr
	mbp->next = head;
	head = mbp;
	
}

void *realloc (void *ptr, size_t size) {
	if(ptr == NULL){
		return malloc(size);
	}
	else if(size == 0)
	{
		//free pointer
		free(ptr);
	}
	
	//use memcpy to copy data
	//get ptr for new address
	void* dest = malloc(size);
	void* result = memcpy(dest, ptr, size);
	if(result == NULL){
		//memcpy failed so we set errno and return null
		free(dest);
		errno = ENOMEM;
		return NULL;
	}
	else
	{
		//memcpy succeeded so we return address
		free(ptr);
		return dest;	
	}
	
	
	return NULL;
}

void *calloc (size_t nmemb, size_t size) {
	// TODO: implement me!
	if(nmemb == 0 || size == 0){
		return NULL; // there is nothing to initialize
	}
	size_t totalSize = nmemb * size;
	
	void* toReturn = malloc(totalSize);
	
	if(toReturn != NULL){
		//init memory
		memset(toReturn, 0, totalSize);
	}
	return toReturn;
}



//Main function only for debugging


//int main(int argc, char** argv){
    //printList();
    //printf("%d\n",1);
    

    //char *m1 = malloc(16);
    //printList();
    //printf("%d\n",2);
    
    //char* m2 = malloc(48);
    //printList();
    //printf("%d\n",3);
    
    //char* m3 = malloc(80);
    //printList();
    //printf("%d\n",4);
    
    //char* m4 = malloc(16);
    //printList();    
    //printf("%d\n",4);

    //free(m1);
    //printList();
    //printf("%d\n",5);
    
    
    //free(m2);
    //printList();
    //printf("%d\n",6);
    
    //free(m3);
    //printList();
    //printf("%d\n",7);
    
    //free(m4);
    //printList();
    //printf("%d\n",8);
    
    //m1 = malloc(1024);
    //printList();
    //printf("%d\n",9);
    
    //m2 = malloc(16);
    //printList();
    //printf("%d\n",10);
    
    //m3 = malloc(47);
    //printList();
    //printf("%d\n",11);
    
    //m4 = malloc(64);
    //printList(); 
    //printf("%d\n",12); 
    
    //free(m1);
    //printList();
    //printf("%d\n",13);
    
    
    //free(m2);
    //printList();
    //printf("%d\n",14);
    
    //free(m3);
    //printList();
    //printf("%d\n",15);
    
    //free(m4);
    //printList();
    //printf("%d\n",16);
	
	//return 0;
//}
