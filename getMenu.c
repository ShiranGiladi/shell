/* getMenu command receives 1 parameter: restaurant name 
   the command will print tha menu */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void Error(char* msg) { fprintf(stderr, "Error:%s\n", msg);  exit(1); } //function that dealing with inappropriate input

int main(int argc, char* argv[]) {
	int status;
	char file[256];
	pid_t pid;
	if (argc != 2) Error("The call was not activated as required\n");
	if ((pid = fork()) == -1) {
		perror("fork() failed"); exit(EXIT_FAILURE);
	}
	if (pid == 0) { //Son process
		execl("/bin/cat", "cat", strcat(argv[1], ".txt"), NULL); //call 'cat' function on nameRestaurant.txt
		perror("execl() failed"); exit(EXIT_FAILURE);
	}
	else {
		waitpid(pid, &status, 0);
	}
	exit(EXIT_SUCCESS);
}
