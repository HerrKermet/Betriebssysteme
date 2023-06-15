#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include "plist.h"

static char** args;
static int argCount;
static char* cwd;
static struct queue_element* taskList;
static int printStatus = false;

static void die(char* message){
    perror(message);
    exit(EXIT_FAILURE);
}

static void appendToken(char* token){

    argCount++;
    args = realloc(args, argCount * sizeof(char*)); //allocate space for new argument pointer
    if(args == NULL)
        die("Realloc failed");
    args[argCount - 1] = malloc(strlen(token) + 1); // allocate space for the current argument string
    if(args[argCount - 1] == NULL)
        die("Malloc failed");
    int len = strlen(token);
    
    if(len > 0 && token[len - 1] == '\n')	//remove \n if token ends with it
		token[len - 1] = '\0';			
	
    strcpy(args[argCount - 1], token);
}

static bool callbackToList(pid_t pid, const char* cmdLine){
	// if should show only running jobs
	if(!printStatus){
		int status;
		pid_t child_pid = waitpid(pid, &status, WNOHANG);
		if(child_pid == 0)
			printf("(still running) PID: %d Command: %s\n", pid, cmdLine);
		return false;
	}
	// if should show status of zombies
	else{
		int status;
		pid_t child_pid = waitpid(pid, &status, WNOHANG); // get PID from background child

		// if child exited already then remove it from list and print status
		if(child_pid != 0){
			printf("Exitstatus of background task [%s] = %d\n", cmdLine, status);
			char buffer[1337];
			removeElement(child_pid, buffer, 1337);
		}
		return false;				
	}
	
}

int main(int argc, char** argv) {
	// init variables
    const int MAX_LENGTH = 1337;
    taskList = NULL;
    
    cwd = malloc(512 * sizeof(char));   // big enough to store paths
    
    
	if(getcwd(cwd, MAX_LENGTH) == NULL)
        die("Path too long");
    
    while(!feof(stdin))
    {
		args = NULL;
		argCount = 0;
		int isBackground = 0;



		// display path and get input
		printf("%s/ :",cwd);
		char commandLine[MAX_LENGTH];
		fgets(commandLine, MAX_LENGTH + 2, stdin);
		// if last char is not \n then word was too long
		if(commandLine[strlen(commandLine) - 1] != '\n') {
			fprintf(stderr, "Warning: input too long! (max 1337 chars)\n");

			// clear buffer after a line was too long
			char c = 0;
			while ((c != '\n' && c != EOF)) {
				c = getchar();
				if(errno)
					perror("Error: "); // if getchar sets errno
			}

			continue;
		}
		
		char* command = strdup(commandLine);
		if(command == NULL)
			die("Malloc failed");
		

		// init token and append each token to args
		char* token;		
		token = strtok(commandLine, " \t");
		while(token != NULL){
			appendToken(token);
			token = strtok(NULL, " \t");
		}
		args[argCount] = NULL;
		if(args[argCount - 1][0] == '&')
			isBackground = 1;
			

		
		if(strcmp(args[0], "cd") == 0 && argCount == 2){
			//change working directory

			if(chdir(args[1]) == 0){
				if(getcwd(cwd, MAX_LENGTH) == NULL)
					die("Path too long");
				}
			else
			{
				perror("chdir");
				errno = 0;
			}
		}
		else if(strcmp(args[0], "cd") == 0 && argCount != 2){
			printf("cd only accepts one argument\n");
		}
			
		else{
			//start child process with given tokens and stores isBackground if its a "&" task
			pid_t pid = fork();
			if(pid == 0){
				/*DEBUG LINE printf("now executing with following args\n");
				for(int i = 0; i < argCount; i++){
					printf("%d %s\n",i, args[i]);
				}
				*/
				if(isBackground)  // remove the & token at the end 
					args[argCount - 1] = NULL;
				if(strcmp(args[0], "jobs") == 0)
					walkList(callbackToList);
				else
				{
					execvp(args[0], args);
					die("Execv failed");
				}
			}
			//Parent process waits for child or continues if its "&" task
			else{


				// wait for child and return exit status
				if(!isBackground)
				{
					int status;
					pid_t child_pid = waitpid(pid, &status, 0); // Wait for the child process to finish
					if (child_pid == -1)
						die("waitpid failed");
					printf("Exitstatus [");
					for (int i = 0; i < argCount; i++){
						printf(args[i]);
						if(i+1 < argCount)
							printf(" ");
					}
					printf("] = %d\n", status);
				}
				// if it is a background task then append pid and commandline to list
				else
				{
					command[strlen(command) - 2] = '\0'; //remove the "&" from command
					int result = insertElement(pid, command);
					free(command);
					if(result < 0)
						die("Plist error");				
				}
				
				//collect zombies and print status for each
				printStatus = 1;
				walkList(callbackToList);
				printStatus = 0;
				
				
			}
		}
	}
	free(cwd);

    return 0;
}
