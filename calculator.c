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
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    char operator;
    double num1, num2, result;

    printf("Enter an operator (+, -, *, /): ");
    scanf("%c", &operator);

    printf("Enter two numbers: ");
    scanf("%lf %lf", &num1, &num2);

    switch (operator) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 == 0) {
                printf("Error: division by zero\n");
                return 1;
            } else {
                result = num1 / num2;
            }
            break;
        default:
            printf("Error: invalid operator\n");
            return 1;
    }

    printf("%.2lf %c %.2lf = %.2lf\n", num1, operator, num2, result);
    sleep(5);
 int x = atoi(argv[1]);
     mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);
    

     

    return 0;
}