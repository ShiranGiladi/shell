/* MakeOrder command accepts 2 parameters: the name of the restaurant and the name of the orderer.
   The command asks the user to enter the item to order and the desired quantity. 
   The system calculates the final amount to be paid after the user has finished choosing (type "Finish"),
   The user confirms by typing "confirm" then the system creates a file for the order in the orderer's name containing
   the items, quantity, final price and the date the order was created.
   The order is saved in a folder named "restaurantName_Order".
   In addition, the command changes the file permissions to read only (to prevent further change) */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int Date(int fd);
int calcPriceForDish(char* buff, char* restaurant);
void Error(char* msg) { fprintf(stderr, "Error:%s\n", msg);  exit(1); } //function that dealing with inappropriate input

int main(int argc, char* argv[]) {
	int fd, sum = 0, status;
	char path[50] = "", buff[50] = { 0 }, dir[50] = "", price[50];
	pid_t pid;
	if (argc != 3) Error("The call was not activated as required\n");
	strcat(path, argv[1]);
	strcat(path, "_Order/");
	strcat(path, argv[2]);
	strcat(path, ".txt");
	strcat(dir, argv[1]);
	strcat(dir, ".txt");
	if ((fd = open(path, O_WRONLY | O_CREAT, 0664)) == -1) //open order file
	{
		printf("Restaurant Not Found!\n"); return(-1);
	}
	if (write(fd, argv[1], strlen(argv[1])) == -1) {
		perror("write"); return(-1);
	}
	if (write(fd, " Order\n\n", 8) == -1) {
		perror("write"); return(-1);
	}
	printf("Insert your order (Finish to finish):\n");
	fgets(buff, 50, stdin);
	while (strcmp(buff, "Finish\n") != 0) { //User enter is order
		if (write(fd, buff, strlen(buff)) == -1) {
			perror("write"); return(-1);
		}
		sum = sum + calcPriceForDish(buff, dir); //Call calcPriceForDish function and sum the total price
		fgets(buff, 50, stdin);
	}
	printf("Total Price: %d NIS (Confirm to approve/else cancle)\n", sum);
	fgets(buff, 50, stdin);
	if (strcmp(buff, "Confirm\n") == 0) {
		if (write(fd, "Total Price: ", 13) == -1) {
			perror("write"); return(-1);
		}
		sprintf(price, "%d", sum);
		if (write(fd, price, strlen(price)) == -1) {
			perror("write"); return(-1);
		}
		if (write(fd, "NIS\n\n", 5) == -1) {
			perror("write"); return(-1);
		}
		Date(fd); //Call Date function
		printf("Order created!\n");
		if (chmod(path, 0444) == -1) {
			perror("chmod"); return(-1);
		}
	}
	else { //Remove the file from the directory in-case that user cancel is order
		if ((pid = fork()) == -1) {
			perror("fork() failed"); exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			execl("/bin/rm", "rm", path, NULL);
			perror("execl() failed"); exit(EXIT_FAILURE);
		}
		else {
			waitpid(pid, &status, 0);
			printf("Order cancle!\n");
		}
	}
	close(fd);
	return 0;
}

int calcPriceForDish(char* buff, char* restaurant) {
	/* Calculates the price of the dish */

	int i = 0, flag = 1, fd, count = 0, index = 0, rbytes;
	char strPrice[5] = { '\0' }, amount[5] = { '\0' };
	char letter;
	char* dish;
	if ((fd = open(restaurant, O_RDONLY)) == -1) {
		perror("open"); return(-1);
	}
	dish = (char*)malloc(sizeof(buff) + 1);
	while (!(buff[i] <= '9' && buff[i] >= '0')) {
		dish[index] = buff[i];
		i = i + 1;
		index = index + 1;
	}
	if ((rbytes = read(fd, &letter, 1)) == -1)
	{
		perror("read"); return(-1);
	}
	while (flag == 1 && rbytes > 0) { //Search the Dish
		if (letter == dish[count])
			count = count + 1;
		else
			count = 0;
		if ((rbytes = read(fd, &letter, 1)) == -1) {
			perror("read"); return(-1);
		}
		if (count == strlen(dish) && letter == '.')
			flag = 0;
	}
	i = 0;
	while (letter != 'N') { //Search the price of the dish
		if (letter <= '9' && letter >= '0') {
			strPrice[i] = letter;
			i = i + 1;
		}
		if (read(fd, &letter, 1) == -1) {
			perror("read"); return(-1);
		}
	}
	i = 0;
	index = 0;
	while (buff[i] != '\n') { //Search the amount of the dish
		if (buff[i] <= '9' && buff[i] >= '0') {
			amount[index] = buff[i];
			index = index + 1;
		}
		i = i + 1;
	}
	close(fd);
	return (atoi(strPrice) * atoi(amount));
}

int Date(int fd) {
	/* Print the current date */

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char day[15], month[15], year[15], date[15] = "";
	sprintf(year, "%d", tm.tm_year + 1900);
	sprintf(month, "%d", tm.tm_mon + 1);
	sprintf(day, "%d", tm.tm_mday);
	if (strlen(day) == 1)
		strcpy(date, "0");
	strcat(date, day);
	strcat(date, "/");
	if (strlen(month) == 1)
		strcat(date, "0");
	strcat(date, month);
	strcat(date, "/");
	strcat(date, year);
	if (write(fd, date, strlen(date)) == -1) {
		perror("write");
		return -1;
	}
	return 0;
}
