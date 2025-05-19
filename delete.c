#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char filename[100];
    int status;

    printf("Enter the name of the file to delete: ");
    scanf("%s", filename);

    if (remove(filename) == 0) {
        printf("File deleted successfully.\n");
        status = 1;
    } else {
        printf("Error deleting file.\n");
        status = 0;
    }

    // Create FIFO if it doesn't exist
    mkfifo("mypipe", 0666);

    // Open FIFO and write status
    int fd = open("mypipe", O_WRONLY);
    write(fd, &status, sizeof(status));
    close(fd);

    return 0;
}