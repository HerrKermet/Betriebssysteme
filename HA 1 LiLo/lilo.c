#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int value;
	struct Node* ptr;
} Node;

static Node* start = NULL;    

static int insertElement(int value) {
	if (value < 0)
		return -1;
	Node* currentNode = start;
	if (start == NULL) {
		start = malloc(sizeof(Node));
		//init root value with -1 to know its the root
		start->value = -1;
	}
	// start and value valid
	
	// if start is empty, put newNode at start
	if(start->ptr == NULL) {
		Node* newNode = malloc(sizeof(Node));
		start->ptr = newNode;
		newNode->value = value;
		return value;
	}
	
	// get to last node
	while (currentNode->ptr != NULL){
		currentNode = currentNode->ptr;
		if (currentNode->value == value) return -1; //found duplicate
	}
	// create new node and add it to list
	Node* newNode = malloc(sizeof(Node));
	if(newNode != NULL){
		 currentNode->ptr = newNode;
		 newNode->value = value;
		 return newNode->value;
		 }
	
	return -1;
}

static int removeElement(void) {
	int value;
	// check if root is not null and if there is atleast one element
	if (start != NULL && start->ptr != NULL){
		Node* nextNode = NULL;
		Node* node2Remove = NULL;
		// check if there is a second entry
		if (start->ptr->ptr != NULL) {
			nextNode = start->ptr->ptr;
		}
		// delete first element and link root to second element or null
		node2Remove = start->ptr;
		start->ptr = nextNode;
		value = node2Remove->value;
		free(node2Remove);
		return value;		
	} 
	
	// if list is empty and removeElement is called, free root
	else {
		free(start);
		start = NULL;
		return -1;
	}
}

int main (int argc, char* argv[]) {
	printf("insert 47: %d\n", insertElement(47));
	printf("insert 11: %d\n", insertElement(11));
	printf("insert 23: %d\n", insertElement(23));
	printf("insert 11: %d\n", insertElement(11));
	printf("insert 20: %d\n", insertElement(20));
	printf("insert 25: %d\n", insertElement(25));


	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	
	//TODO add more tasks:
	printf("insert 20: %d\n", insertElement(20));
	printf("insert 25: %d\n", insertElement(25));
	
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	
	//delete root to free space
	printf("remove: %d\n", removeElement());
	



	exit(EXIT_SUCCESS);
}


