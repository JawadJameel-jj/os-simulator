#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char answer[10];
    int score = 0;
    
    printf("Answer the following questions to see if I can guess your gender.\n");
    
    printf("Q1. Do you prefer to wear dresses? (yes or no)\n");
    scanf("%s", answer);
    if (strcmp(answer, "yes") == 0) {
        score += 2;
    } else if (strcmp(answer, "no") == 0) {
        score -= 2;
    }
    
    printf("Q2. Do you enjoy watching sports? (yes or no)\n");
    scanf("%s", answer);
    if (strcmp(answer, "yes") == 0) {
        score += 1;
    } else if (strcmp(answer, "no") == 0) {
        score -= 1;
    }
    
    printf("Q3. Do you prefer to wear makeup? (yes or no)\n");
    scanf("%s", answer);
    if (strcmp(answer, "yes") == 0) {
        score += 2;
    } else if (strcmp(answer, "no") == 0) {
        score -= 2;
    }
    
    printf("Q4. Do you enjoy shopping? (yes or no)\n");
    scanf("%s", answer);
    if (strcmp(answer, "yes") == 0) {
        score += 2;
    } else if (strcmp(answer, "no") == 0) {
        score -= 2;
    }
    
    printf("Q5. Do you like to play video games? (yes or no)\n");
    scanf("%s", answer);
    if (strcmp(answer, "yes") == 0) {
        score -= 1; 
    } else if (strcmp(answer, "no") == 0) {
        score += 1;
    }
    if (score > 3) {
        printf("I think you are female.\n");
    } else if (score < -3) {
        printf("I think you are male.\n");
    } else {
        printf("I'm not sure what gender you are.\n");
    }
   sleep(2);
    int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);

    return 0;
}