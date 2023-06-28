#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include "sem.h"


static char** PathsToGrep = NULL;
static int storedPaths = 0;
static int possibleStorage = 1;

static void die(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

static void insertString(char* str){
	if(storedPaths >= possibleStorage){
		//allocate more memory for the paths
		possibleStorage *= 2;
		PathsToGrep = realloc(PathsToGrep, possibleStorage * sizeof(char*));
		if(PathsToGrep == NULL)
			die("malloc");
	}
		
		PathsToGrep[storedPaths] = strdup(str);
		if(PathsToGrep[storedPaths] == NULL)
			die("strdup");
		storedPaths++;
}

static char* popString(){
	if(storedPaths == 0)
		return NULL;
	char* toReturn = strdup(PathsToGrep[storedPaths - 1]);
	free(PathsToGrep[storedPaths]);
	storedPaths--;
	
	if(storedPaths <= possibleStorage / 2){
		possibleStorage /= 2;
		PathsToGrep = realloc(PathsToGrep, possibleStorage * sizeof(char*));
		if(PathsToGrep == NULL)
			die("malloc");		
	}
	return toReturn;
	
}


static void freePathStorage(){
		for(int i = 0; i < storedPaths; i++){
		free(PathsToGrep[i]);
	}
	free(PathsToGrep);
}

int main(int argc, char** argv){
	
	char* test = "Hello1";
	char* test1 = "Hello2";
	char* test2 = "Hello3";
	char* test3 = "Hello4";
	char* test4 = "Hello4";
	
	//init paths to grep
	PathsToGrep = malloc(sizeof(char*) * possibleStorage);
	
	insertString(test);
	insertString(test1);
	insertString(test2);
	insertString(test3);
	insertString(test4);
	
	for(int i = 0; i < storedPaths; i++){
		printf("i: %d possible: %d stored: %d\n",i, possibleStorage, storedPaths);
		printf("%s\n",PathsToGrep[i]);
	}
	
	char* temp = popString();
	
		for(int i = 0; i < storedPaths; i++){
		printf("i: %d possible: %d stored: %d\n",i, possibleStorage, storedPaths);
		printf("%s\n",PathsToGrep[i]);
	}
	printf("popped string: %s\n", temp);
	printf("possible %d\n", possibleStorage);
	
	freePathStorage();
	free(temp);
	
	return 0;
}
