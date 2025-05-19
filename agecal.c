#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int birth_year, current_year, age;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    current_year = t->tm_year + 1900;
    printf("Enter your birth year: ");
    scanf("%d", &birth_year);
    age = current_year - birth_year;
    printf("Your age is: %d\n", age);
   sleep(2);
 int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);
    return 0;
}