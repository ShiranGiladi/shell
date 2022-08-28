/* getOrderNum command receives 1 parameter: restaurant name.
   Returns the amount of existing orders for the specific restaurant. */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

void Error(char* msg) { fprintf(stderr, "Error:%s\n", msg);  exit(1); } //function that dealing with inappropriate input

int main(int argc, char* argv[]) {
	int count = 0;
	char path[50] = "./";
	DIR* dir;
	struct dirent* d;
	if (argc != 2) Error("The call was not activated as required\n");
	strcat(path, argv[1]);
	strcat(path, "_Order");
	if ((dir = opendir(path)) == NULL) { //Open the required directory
		perror("open"); return(-1);
	}
	while ((d = readdir(dir)) != NULL) //Count the files
		count = count + 1;
	printf("%d Orders\n", count - 2);
	return 0;
}
