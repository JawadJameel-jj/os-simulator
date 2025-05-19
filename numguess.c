#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int secret_number, user_guess;

    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate a random number between 1 and 100
    secret_number = rand() % 100 + 1;

    printf("I'm thinking of a number between 1 and 100.\n");

    do {
        printf("Enter your guess: ");
        scanf("%d", &user_guess);

        if (user_guess > secret_number) {
            printf("Too high! Try again.\n");
        } else if (user_guess < secret_number) {
            printf("Too low! Try again.\n");
        } else {
            printf("Congratulations! You guessed the correct number: %d\n", secret_number);
        }
    } while (user_guess != secret_number);
int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);


    return 0;
}