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
    char old_file[100];
    char new_file[100];

    printf("Enter the name of the file to move: ");
    fgets(old_file, 100, stdin);

    // Remove the newline character at the end of the input string
    old_file[strcspn(old_file, "\n")] = 0;

    printf("Enter the new name of the file: ");
    fgets(new_file, 100, stdin);

    // Remove the newline character at the end of the input string
    new_file[strcspn(new_file, "\n")] = 0;

    int result = rename(old_file, new_file);

    if (result == 0) {
        printf("File moved successfully.\n");
    } else {
        printf("Error moving file.\n");
    }
 int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);

    return 0;
}