#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

int main(int argc, char *argv[]) {
   printf("Beeping 100 times to simulate song.");
   for (int i = 0; i < 100; i++) {
       printf("\a");
       fflush(stdout);
       usleep(500000);
   }
   int x = atoi(argv[1]);
   mkfifo("mypipe", 0666);
   int fd = open("mypipe", O_WRONLY);
   write(fd, &x, sizeof(x));
   close(fd);
   return 0;
}