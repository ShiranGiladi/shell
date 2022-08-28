/* Code to run the shell */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int createArgv(char* buff, char** arr);
void Error(char* msg) { fprintf(stderr, "Error:%s\n", msg);  exit(1); }

int main(int argc, char* argv[]) {
	int status, i, flag;
	char buff[50], path[50] = "";
	char* arr[10] = { '\0' };
	pid_t pid;
	if (argc != 1) Error("The call was not activated as required\n");
	printf("AdvShell>");
	fgets(buff, 50, stdin);
	flag = createArgv(buff, arr);
	while (flag == 0) {
		printf("AdvShell>");
		fgets(buff, 50, stdin);
		flag = createArgv(buff, arr);
	}
	while (strcmp(arr[0], "exit") != 0) {
		/* Run the user's commands using fork (son process) 
		   entring 'exit' command will end the shell from running */

		if ((pid = fork()) == -1) {
			perror("fork() failed"); exit(EXIT_FAILURE);
		}
		if (pid == 0) { //Son process
			if (strcmp(arr[0], "CreateMenu") == 0 || strcmp(arr[0], "getMenu") == 0 || strcmp(arr[0], "MakeOrder") == 0 || strcmp(arr[0], "getPrice") == 0 || strcmp(arr[0], "getOrderNum") == 0) {
				execv(arr[0], arr);
				perror("execl() failed"); exit(EXIT_FAILURE);
			}
			else {
				strcpy(path, "/bin/");
				strcat(path, arr[0]);
				execv(path, arr);
				printf("Not Supported!\n"); exit(EXIT_FAILURE);
			}
		}
		else {
			waitpid(pid, &status, 0);
		}
		for (i = 0; i < 10; i++) //Initializing the array
			arr[i] = '\0';
		printf("AdvShell>");
		fgets(buff, 50, stdin);
		flag = createArgv(buff, arr);
		while (flag == 0) {
			printf("AdvShell>");
			fgets(buff, 50, stdin);
			flag = createArgv(buff, arr);
		}
	}
	printf("GoodBye...\n");
	return 0;
}

int createArgv(char* buff, char** array) {
	/* Divides a string into sub-strings, return 0 in-case the strig is ENTER */

	int i = 0, flag = 0;
	char* p;
	buff[strlen(buff) - 1] = '\0';
	p = strtok(buff, " ");
	while (p != NULL){
		flag = 1;
		array[i] = p;
		i = i + 1;
		p = strtok(NULL, " ");
	}
	return flag;
}
