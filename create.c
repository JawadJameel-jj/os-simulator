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

int main(int argc, char *argv[]) {
    FILE *fp;
    char filename[100];

    printf("Enter the name of the file to create: ");
    scanf("%s", filename);

    fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Error creating file.\n");
        return 1;
    }

    printf("File created successfully.\n");

    fclose(fp);
         int x = atoi(argv[1]);
     mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);

    return 0;
}