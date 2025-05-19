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
    FILE *src_file, *dest_file;
    char src_filename[100], dest_filename[100], buffer[1024];
    size_t bytes_read;

    while (1)
    {
        printf("Enter the source filename: ");
        scanf("%s", src_filename);

        src_file = fopen(src_filename, "rb");
        if (src_file != NULL) {
            break;
        } else {
            printf("Error: Cannot open source file '%s'\n", src_filename);
        }
    }
    
    while (1)
    {
        printf("Enter the destination filename: ");
        scanf("%s", dest_filename);

        dest_file = fopen(dest_filename, "wb");
        if (dest_file != NULL) {
            break;
        } else {
            printf("Warning: Cannot open destination file '%s', creating a new file...\n", dest_filename);
            dest_file = fopen(dest_filename, "wb");
            if (dest_file == NULL) {
                printf("Error: Cannot create destination file '%s'\n", dest_filename);
                fclose(src_file);
                return 1;
            }
        }
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src_file)) > 0)
    {
        fwrite(buffer, 1, bytes_read, dest_file);
    }

    fclose(src_file);
    fclose(dest_file);

    printf("File copied successfully!\n");
   int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);

    return 0;
}