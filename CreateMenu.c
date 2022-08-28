/* CreateMenu command get 2 parameters: the name of the restaurant,
   and the number of types of items (such as main dish, salad, etc...)
   
   The user will need to enter the type of item (Type Dish). Then enter the name of the item and the price in shekels (NIS),
   moving to the second item type when the user enters the word "Stop".
   At the end of receiving all the items, the command will create the menu,
   according to the name of the restaurant (first parameter).
   At the beginning of the file will appear the name of the restaurant, in the second line the type of item, then the name of the item and price in shekels (NIS),
   and at the bottom of the page will appear the message "Bon Appetit."
   In addition, the command will create a folder for saving orders from this restaurant. The folder name will be restaurantName_Order. */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int printDish(int fd, char* buff);
void Error(char* msg) { fprintf(stderr, "Error:%s\n", msg);  exit(1); } //function that dealing with inappropriate input

int main(int argc, char* argv[]) {
	int fd, countD = 0, countDN = 1, l = 97;
	char file[50] = "", dir[50] = "", buff[50];
	char letter = 'a';
	if (argc != 3) Error("The call was not activated as required\n");
	strcat(file, argv[1]);
	strcat(file, ".txt");
	if ((fd = open(file, O_WRONLY | O_CREAT, 0664)) == -1) { //open menu file
		perror("open to"); return(-1);
	}
	strcat(dir, argv[1]);
	strcat(dir, "_Order");
	if (mkdir(dir, 0777) == -1) { //Create directory (open match folder for restaurant orders)
		perror("create directory");
		return(-1);
	}
	if (write(fd, argv[1], strlen(argv[1])) == -1){
		perror("write"); return(-1);
	}
	if (write(fd, " Menu\n\n", 7) == -1) {
		perror("write"); return(-1);
	}
	while (countD != atoi(argv[2])) { //The user enters the menu
		printf("Insert Type Dish %c:\n", l);
		fgets(buff, 50, stdin);
		if (write(fd, &letter, 1) == -1) {
			perror("write"); return(-1);
		}
		if (write(fd, ". ", 2) == -1) {
			perror("write"); return(-1);
		}
		if (write(fd, buff, strlen(buff)) == -1) {
			perror("write"); return(-1);
		}
		printf("Insert dish name %d:\n", countDN);
		fgets(buff, 50, stdin);
		while (strcmp(buff, "Stop\n") != 0) {
			if (write(fd, "  ", 2) == -1) {
				perror("write"); return(-1);
			}
			fd = printDish(fd, buff);
			countDN = countDN + 1;
			printf("Insert dish name %d:\n", countDN);
			fgets(buff, 50, stdin);
		}
		if (write(fd, "\n", 1) == -1) {
			perror("write"); return(-1);
		}
		countDN = 1;
		countD = countD + 1;
		l = l + 1;
		letter = letter + 1;
	}
	if (write(fd, "\n          Bon Appetit\n", 23) == -1) {
		perror("write"); return(-1);
	}
	printf("Successfully created\n");
	close(fd); return(0);
}

int printDish(int fd, char* buff) { 
	/* Print the dish name into the menu file */

	int j = 0, i = 0, countletter = 0, countnum = 0, point = 0;
	char* str;
	while (!(buff[i] <= '9' && buff[i] >= '0')) {
		countletter = countletter + 1;
		i = i + 1;
	}
	while (buff[i] != '\n') {
		countnum = countnum + 1;
		i = i + 1;
	}
	point = 30 - countletter - countnum;
	str = (char*)malloc(point + countletter + countnum + 4);
	i = 0;
	for (i = 0; i < countletter; i++) {
		str[i] = buff[i];
	}
	for (; i < point + countletter - 1; i++) {
		str[i] = '.';
	}
	str[i] = ' ';
	i = i + 1;
	for (; i < countnum + point + countletter; i++) {
		str[i] = buff[strlen(buff) - countnum + j - 1];
		j = j + 1;
	}
	strcat(str, "NIS\n");
	if (write(fd, str, strlen(str)) == -1) {
		perror("write"); return(-1);
	}
	return fd;
}
