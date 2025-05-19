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

int main(int argc, char *argv[])
{
    int choice;
    float temperature;
    
    printf("Enter temperature: "); 
    scanf("%f", &temperature);
    
    printf("Choose conversion:\n");
    printf("1. Celsius to Fahrenheit\n");
    printf("2. Fahrenheit to Celsius\n");
    scanf("%d", &choice);
    
    switch(choice)
{
        case 1:
            temperature = temperature * 9/5 + 32;
            printf("Temperature in Fahrenheit: %.2f\n", temperature);
            break;
        case 2:
            temperature = (temperature - 32) * 5/9;
            printf("Temperature in Celsius: %.2f\n", temperature);
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
    sleep(5);
     int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);
    return 0;
}