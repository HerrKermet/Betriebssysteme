#include <stdio.h>
#include <stdlib.h>

// TODO: structs, global variables, etc.

typedef struct Node {     //wir habe ein struct Node in welchem wir die Node Selber beschreiben und einen zeiger auf das nächste element setzen.
	int data;
	struct Node* next;
}Node; 

struct verketteteListe {     //das hier ist die struct implementierung der liste hier benutzen wir einen Node Pointer variable die auf den head(erstes Element in der Liste) der liste zeigt.
	struct Node* head;

};

struct verketteteListe Liste = { NULL };     //hier wird die verkettete Liste Initialisiert


//insert element into our verkettete Liste
static int insertElement(int value) {

	if (Liste.head == NULL){ 
		struct Node* newNode = malloc(sizeof(struct Node));   //this allocates memopry to a new Node in the Linked list, by this we can add as much nodes as we have memory 
		if (newNode == NULL){
			return -1;            //this if statement checks if die Fehlerbehandlung für malloc , denn wenn malloc keinen speicher allopkieren kann wird null returnt
		}
		newNode->data = value;   //hiermit belegen wir dem vorher mit malloc angefragten speicher auf den wert der durch "value" übergeben wird.(Alternative schreibweise für newNode->data = value |  (*newNode).data = value; )
		newNode->next = NULL;     // hiermit wird der wert auf den der neu eingefügte wert zeigt auf Null gesetzt weil wir den wert an das ende unserer liste einfügen.     
		Liste.head = newNode;            //hier wird geschaut ob die Liste leer ist und wenn ja wird Der head auf den neuen eingefügten wert gesetzt.
		return Liste.head->data;
	}else {
		struct Node* current = Liste.head;          //hier wird ein pointer initialisiert der auf auf den head der linked list zeigt
		while (current->next != NULL) {
			if (current->data == value){
				return -1;
			}			
			current = current->next;
								
		}
		struct Node* newNode = malloc(sizeof(struct Node));   //this allocates memopry to a new Node in the Linked list, by this we can add as much nodes as we have memory 
		if (newNode == NULL){
			return -1;            //this if statement checks if die Fehlerbehandlung für malloc , denn wenn malloc keinen speicher allopkieren kann wird null returnt
		}
		newNode->data = value;   //hiermit belegen wir dem vorher mit malloc angefragten speicher auf den wert der durch "value" übergeben wird.(Alternative schreibweise für newNode->data = value |  (*newNode).data = value; )
		newNode->next = NULL;     // hiermit wird der wert auf den der neu eingefügte wert zeigt auf Null gesetzt weil wir den wert an das ende unserer liste einfügen.
		current->next = newNode;					//nachdem die loop beendet wurde wird wechseln wir den next pointer von dem vorherigen wert auf unser neue eingefügtem wert.


		
		
		return current->next->data;
	}
	return -1;


	// TODO: implement me!
}

static int removeElement(void) {
	struct Node* current = Liste.head;
	if(current == NULL){
		return -1;
	}

	if(current != NULL){
	    struct Node* temp = Liste.head;
	    int result = temp->data;
		Liste.head = temp->next;
		free(temp);
		
		
    return result;
	}

	// TODO: implement me!
	return -1;
}

int main (int argc, char* argv[]) {
	printf("insert 47: %d\n", insertElement(47));
	printf("insert 11: %d\n", insertElement(11));
	printf("insert 23: %d\n", insertElement(23));
	printf("insert 11: %d\n", insertElement(11));

	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());

	printf("insert 47: %d\n", insertElement(47));
	printf("insert 11: %d\n", insertElement(11));
	printf("insert 23: %d\n", insertElement(23));
	printf("insert 11: %d\n", insertElement(11));
	
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	// TODO: add more tests

	exit(EXIT_SUCCESS);
}
