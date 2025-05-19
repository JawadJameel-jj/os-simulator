#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int x = atoi(argv[1]);
    char filename[100];
    char text[1000];
    FILE *fp;
    printf("Enter file name: ");
    scanf("%s", filename);
    fp = fopen(filename, "a+");
    if (fp == NULL) {
        printf("Error creating file!\n");
        return 1;
    }
    printf("Enter text (press '=' to save and exit):\n");
    fseek(fp, 0, SEEK_SET);
    while (fgets(text, sizeof(text), fp) != NULL) {
        printf("%s", text);
    }
    while (fgets(text, sizeof(text), stdin) != NULL) {
        if (text[0] == '=') {
            break;
        }
        fprintf(fp, "%s", text);
        fflush(fp);
    }
    fclose(fp);
    
    mkfifo("mypipe", 0666);
    int fd = open("mypipe", O_WRONLY);
    write(fd, &x, sizeof(x));
    close(fd);

    return 0;
}