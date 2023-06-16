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
	// TODO: implement me!
	if(head == NULL){
		//Initialize head
		head = (struct mblock*) memory;
		head->next = NULL;
		head->size = SIZE - sizeof(struct mblock);
	}
	
	//check if there is enough space
	if(1){
		//case head->next is not null (can only be null or a pointer)
		if(head->next != NULL && head->next != MAGIC){
			//head->next points to the next free mblock with size of head->next->size (only if next is not the MAGIC)
			
			//find suitable free space or go to last free space if there is none
			struct mblock* currMblock = head;
			struct mblock* prevMblock = head;
			while(currMblock->next != NULL){
				//check if current mblock has enough space
				if(currMblock->size >= (size + sizeof(struct mblock))){
					//get pointer to return
					void* ptr = ((char*) currMblock) + sizeof(struct mblock);
					
					//create new mblock after the occupied space
					struct mblock* newMblock = (struct mblock*) ((char*) currMblock + size + sizeof(struct mblock));
					newMblock->size = currMblock->size - size - sizeof(struct mblock);
					newMblock->next = currMblock->next;
					
					//shift head and adjust current struct
					currMblock->size = size;
					head = currMblock->next;
					currMblock->next = MAGIC;
					
					
					return ptr;
				}
				else{
					//Not enough space so shift to next mblock
					prevMblock = currMblock;
					currMblock = currMblock->next;
				}
			}
			//currMblock reached end of linked list
			if(currMblock->size >= size){
				// we have enough space for the requested size
				// but do we have enough to add a struct too?
				if(currMblock->size >= size + sizeof(struct mblock))
				{
					//add size and new struct
					void* ptr;
					//init newMblock and put it after the requested memory
					struct mblock* newMblock = (struct mblock*) ((char*) currMblock + sizeof(struct mblock) + size);
					newMblock->next = NULL;
					newMblock->size = currMblock->size - size - sizeof(struct mblock);
					
					currMblock->next = MAGIC;
					currMblock->size = size;
					
					//adjust previous block->next of linked list
					prevMblock->next = newMblock;
					
					ptr = (char*) currMblock + sizeof(struct mblock);
					return ptr;
									
				}
				else
				{
					//only enough space for the size
					void* ptr;
					
					currMblock->next = MAGIC;
					// dont change size because we have no struct after it (just giving some more space couldnt hurt here)
					
					//adjust previous block of linked list to be the last block
					prevMblock->next = NULL;
					
					// return the pointer
					ptr = (char*) currMblock + sizeof(struct mblock);
					return ptr;
					
				}
			}
		}
		//case head->next is null
		if(head->next == NULL && head->size >= size + sizeof(struct mblock)){
			//make new mblock right after the allocated space
			struct mblock* newMblock = (struct mblock*)(((char*) head) + size + sizeof(struct mblock));
			
			//get reference to return pointer with requested size
			void* ptr = (void*)(((char*) head) + sizeof(struct mblock));
			
			//init new mblock
			newMblock->next = NULL;
			newMblock->size = head->size - size - sizeof(struct mblock);
			
			//adjust head mblock
			head->size = size;
			head->next = MAGIC;
			
			//shift head
			head = newMblock;
			return ptr;			
		}
	}
	return NULL;
}

void free (void *ptr) {
	// TODO: implement me!
	
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
	// TODO: implement me!
	return NULL;
}

void *calloc (size_t nmemb, size_t size) {
	// TODO: implement me!
	return NULL;
}

//int main(int argc, char** argv){
	//printList();

	//char *m1 = malloc(16);
	//printList();
	
	//char* m2 = malloc(48);
	//printList();

	//free(m1);
	//printList();
	
	//char* m3 = malloc(80);
	//printList();
	
	////free(m2);
	//printList();
	
	////free(m3);
	//printList();
	
	//printf("printing unused stuff %p %p\n", m2, m3);
	
	//return 0;
//}
