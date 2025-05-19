#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include<ctype.h>
int main(int argc, char *argv[])
{
    char str[100]; 
    int i, count = 0;
    printf("Enter a string: ");
    fgets(str, 100, stdin);
    for (i = 0; str[i] != '\0'; i++)
{
        if (isalpha(str[i]))
{
            count++;
        }
    }   
    printf("Number of letters: %d\n", count);
int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);
    printf("Press any key to continue...");
    getchar();
    return 0;
}