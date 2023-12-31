#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <unistd.h>
#include "sem.h"

struct statistics {
	int lines;
	int lineHits;
	int files;
	int fileHits;
	int dirs;
	int activeGrepThreads;
	int maxGrepThreads;
	int activeCrawlThreads;
};

// (module-)global variables
static struct statistics stats;
// TODO: add variables if necessary
static SEM* sem;
static SEM* grepSem;
static SEM* hasChanged;
static SEM* openFileSem;
//static int MAX_LINE = 4096;
static char** trees;
static char stringToSearch[4097];
// function declarations
static void* processTree(void* path);
static void processDir(char* path);
static void processEntry(char* path, struct dirent* entry);
static void* processFile(void* path);
// TODO: add declarations if necessary

static void usage(void) {
	fprintf(stderr, "Usage: palim <string> <max-grep-threads> <trees...>\n");
	exit(EXIT_FAILURE);
}

static void die(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * \brief Initializes necessary data structures and spawns one crawl-Thread per tree.
 * Subsequently, waits passively on statistic updates and on update prints the new statistics.
 * If all threads terminated, it frees all allocated resources and exits/returns.
 */

int main(int argc, char** argv) {
	if(argc < 4) {
		usage();
	}

	// convert argv[2] (<max-grep-threads>) into long with strtol()
	errno = 0;
	char *endptr;
	stats.maxGrepThreads = strtol(argv[2], &endptr, 10);

	// argv[2] can not be converted into long without error
	if(errno != 0 || endptr == argv[2] || *endptr != '\0') {
		usage();
	}

	if(stats.maxGrepThreads <= 0) {
		fprintf(stderr, "max-grep-threads must not be negative or zero\n");
		usage();
	}

	// TODO: implement me!
	
	struct rlimit limits;
	if (getrlimit(RLIMIT_NOFILE, &limits) == -1) {
        die("getrlimit");
    }
	//init semaphore for critical sections
	sem = semCreate(1);
	grepSem = semCreate(stats.maxGrepThreads);
	hasChanged = semCreate(0);
	openFileSem = semCreate(limits.rlim_cur);
	if(sem == NULL || grepSem == NULL || openFileSem == NULL || hasChanged == NULL)
		die("semCreate() failed");
	
	// get the requested string to search
	strcpy(stringToSearch, argv[1]);
	//init tree array and fill it
	int noOfTrees = argc - 3;
	trees = malloc(noOfTrees * sizeof(char*));
	for(int i = 0; i < noOfTrees; i++){
			trees[i] = argv[i + 3];
	}
	
	//for (int i = 0; i < noOfTrees; i++) {
		//printf("Processing Tree (%d) %s\n",i, trees[i]);
    //}
	
	//create a pthread for each tree
	pthread_t crawlThreads[noOfTrees];
	

	//start a thread for each tree
	for(int i = 0; i < noOfTrees; i++){
		// increase number of active crawlthreads
		P(sem);
		stats.activeCrawlThreads += 1;
		stats.dirs += 1;
		V(sem);
		pthread_create(&crawlThreads[i], NULL, processTree, (void*) trees[i]);
	}
	
	//wait for all threads to finish	
	while(1){
		if(stats.activeGrepThreads > stats.maxGrepThreads) printf("ALERT:  TOO MANY GREPS\n");
		
		//if stats has been changed then print statistics
		

		P(hasChanged);
		//printf("##########\nDirs : %d\nFiles: %d\nCrawl: %d\nGrep : %d\nLineHits: %d\n##########\n",stats.dirs, stats.files, stats.activeCrawlThreads, stats.activeGrepThreads, stats.lineHits);

		printf("\r%d/%d lines, %d/%d files, %d directories, %d active threads", stats.lineHits, stats.lines, stats.fileHits, stats.files, stats.dirs, stats.activeGrepThreads);
		fflush(stdout);
		P(sem);
		if(stats.activeCrawlThreads <= 0 && stats.activeGrepThreads <= 0){
			V(sem);
			//printf("Every Thread has finished.  Programm will now end.\n");
			break;
		}
		V(sem);		
		
	}
	
	//We dont have any more threads that  can signal the change they made so we print the end result here
	printf("\r%d/%d lines, %d/%d files, %d directories, %d active threads", stats.lineHits, stats.lines, stats.fileHits, stats.files, stats.dirs, stats.activeGrepThreads);
	fflush(stdout);
	
	free(trees);
	printf("\n");
	return EXIT_SUCCESS;
}


/**
 * \brief Acts as start_routine for crawl-Threads and calls processDir().
 *
 * It updates the stats.activeCrawlThreads field.
 *
 * \param path Path to the directory to process
 *
 * \return Always returns NULL
 */
static void* processTree(void* path) {
	//TODO: implement me!
	
	//detach thread
	pthread_detach(pthread_self());
	
	//printf("Thread on Path: %s\n",(char*)path);
	
	
	processDir((char*) path);
	
	//decrease number of crawl threads before finishing
	P(sem);
	stats.activeCrawlThreads -= 1;
	V(hasChanged);
	V(sem);
	
	return NULL;
}

/**
 * \brief Iterates over all directory entries of path and calls processEntry()
 * on each entry (except "." and "..").
 *
 * It updates the stats.dirs field.
 *
 * \param path Path to directory to process
 */

static void processDir(char* path) {
	// TODO: implement me!
	//printf("Now processingDir at %s\n",path);
	
	//open current directory
	//using semaphore to keep track of opened files to prevent too many ofpen files error
	P(openFileSem);
	DIR* dir = opendir(path);
	if(dir == NULL)
		die("opendir");
		
	//init dirent and stat buffer
	struct dirent* entry = NULL;
	
	//reset errno for readdir
	errno = 0;
	entry = readdir(dir);
	
	while(entry != NULL){
		//reset errno for readdir
		errno = 0;

			
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
			//skip . and .. directories
			entry = readdir(dir);
			continue;
		}
		
		// if its an entry process it
		char newPath[256];
		strcpy(newPath, path);
		strcat(newPath, "/");
		strcat(newPath, entry->d_name);
		
		//printf("Processing entry at path %s\n",newPath);
		processEntry(newPath, entry);
		entry = readdir(dir);

	}
	if(errno)
		die("readdir");
	
	closedir(dir);
	V(openFileSem);
	
}

/**
 * \brief Spawns a new grep-Thread if the entry is a regular file and calls processDir() if the entry
 * is a directory.
 *
 * It updates the stats.activeGrepThreads if necessary. If the maximum number of active grep-Threads is
 * reached the functions waits passively until another grep-Threads can be spawned.
 *
 * \param path Path to the directory of the entry
 * \param entry Pointer to struct dirent as returned by readdir() of the entry
 */
static void processEntry(char* path, struct dirent* entry) {
	//TODO: implement me!
	struct stat buf;
	if(lstat(path, &buf) == -1){
		die("lstat");
	}
	
	if(S_ISDIR(buf.st_mode)){
		//is a directory
		//printf("Found DIRECTORY: %s in %s\n",entry->d_name, path);
		//increase number of processed dirs
		P(sem);
		stats.dirs += 1;
		V(hasChanged);
		V(sem);
		
		//process given directory
		processDir(path);
		
		
	} else if(S_ISREG(buf.st_mode)){
		// is a regular file
		//printf("Found REGFILE: %s in %s\n",entry->d_name, path);
		
		
		
		//create grepThread
		//printf("	Creating a grepThread for %s\n",path);
		P(grepSem);
		//adjust number of grepthreads
		P(sem);
		stats.activeGrepThreads += 1;
		V(hasChanged);
		V(sem);
		char* p = strdup(path);
		pthread_t grepThread;
		pthread_create(&grepThread, NULL, processFile, (void*) p);	
		if(pthread_detach(grepThread))		
			die("pthread_detach");
	}
	

}

/**
 * \brief Acts as start_routine for grep-Threads and searches all lines of the file for the
 * search pattern.
 *
 * It updates the stats.files, stats.lines, stats.fileHits, stats.lineHits
 * stats.activeGrepThreads fields if necessary.
 *
 * \param path Path to the file to process
 *
 * \return Always returns NULL
 */
static void* processFile(void* p) {
	//TODO: implement me!	
	char* path = (char*) p;
	//increase number of searched files
	P(sem);
	stats.files += 1;
	V(hasChanged);
	V(sem);
	//printf("		Thread (%ld) processes %s\n", pthread_self(),path);
	
	//process file here and count lines etc
	
	//check if we can open another file  if not we block and wait
	P(openFileSem);
	FILE* file = fopen(path,"r");
	if(file == NULL && errno != EACCES)
		die("fopen");
		
	//if permission is denied then skip current file
	if(errno != EACCES)
	{
		char line[4097]; // 4096 for max line and and another one for \0
		
		int hasFileHit = 0;
		
		while(fgets(line, 4096, file) != NULL){
			//increase line count
			P(sem);
			V(hasChanged);
			stats.lines += 1;
			V(sem);
			if(strstr(line, stringToSearch) != NULL){
				//line contained the string
				P(sem);
				stats.lineHits += 1;
				V(hasChanged);
				//if file was not counted yet, then count it
				if(!hasFileHit){
					stats.fileHits += 1;
					hasFileHit = 1;
					V(hasChanged);
				}
				V(sem);
				//printf("%s\n",line);
			}
		}
		fclose(file);
	}
	//signal that we can open another file
	V(openFileSem);
	errno = 0;
	
	//printf("		Thread (%ld) finished %s\n", pthread_self(),path);
	free(path);
	//adjust number of grepThreads;
	P(sem);
	stats.activeGrepThreads -= 1;
	V(hasChanged);
	V(sem);
	
	//decrease number of grepThreads
	V(grepSem);
	return NULL;
}

