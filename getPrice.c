/* getPrice command receives 2 parameters: the name of the restaurant, and the name of the item.
   Returns the price of the required item in shekels (NIS). */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

void Error(char* msg) { fprintf(stderr, "Error:%s\n", msg);  exit(1); } //function that dealing with inappropriate input

int main(int argc, char* argv[]) {
	int fd, flag = 1, rbytes, count = 0, i = 0;
	char letter, strPrice[5], file[50] = "", dish[50] = "";
	if(argc < 3) Error("The call was not activated as required\n");
	strcat(file, argv[1]);
	strcat(file, ".txt");
	if ((fd = open(file, O_RDONLY)) == -1) //Opens the relevant file (only in read mode)
	{
		printf("Restaurant Not Found!\n"); return(-1);
	}
	for (i = 2; i < argc; i++) { //Contcatenates strings in-case that dish contain more than 1 word
		strcat(dish, argv[i]);
		strcat(dish, " ");
	}
	if ((rbytes = read(fd, &letter, 1)) == -1)
	{
		perror("read"); return(-1);
	}
	while (flag == 1) { //Search the Dish
		if (letter == dish[count])
			count = count + 1;
		else
			count = 0;
		if ((rbytes = read(fd, &letter, 1)) == -1){
			perror("read"); return(-1);
		}
		if (count == strlen(dish) && letter == '.')
			flag = 0;
		else if (rbytes <= 0) {
			printf("Dish Not Found!\n");
			exit(-1);
		}
	}
	i = 0;
	while (letter != 'N') { //Search the price of the dish
		if (letter <= '9' && letter >= '0') {
			strPrice[i] = letter;
			i = i + 1;
		}
		if ((rbytes = read(fd, &letter, 1)) == -1) {
			perror("read"); return(-1);
		}
	}
	printf("%s NIS\n", strPrice); //Print the price
	close(fd);
	return 0;
}
