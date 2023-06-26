#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include "sem.h"

static int counter = 0;

static void* increment(void* arg){
	counter = counter + *(int*) arg;
	
	return NULL;
}


int main(int argc, char** argv){
	int num = 1;
	increment((void*)&num);
	increment((void*)&num);
	increment((void*)&num);
	increment((void*)&num);
	increment((void*)&num);
	increment((void*)&num);
	increment((void*)&num);
	
	pthread_t thread1;
	
	printf("%d\n", counter);
	return 0;
}

