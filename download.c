#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

void simulate_download(const char *category) {
    int total_time = 60;  // Total time for download in seconds (1 minute)
    int steps = 50;       // Number of steps for the progress bar
    int progress = 0;     // Initial progress

    printf("Starting download for %s...\n", category);

    // Simulate downloading over the course of 1 minute
    for (int i = 0; i <= steps; i++) {
        progress = (i * 100) / steps;

        printf("\r[");
        int hash_count = (i * 50) / steps;
        for (int j = 0; j < hash_count; j++) printf("#");
        for (int j = hash_count; j < 50; j++) printf(" ");
        printf("] %d%%", progress);

        fflush(stdout);
        usleep(1200000); // ~1.2 seconds per step
    }

    printf("\nDownload complete for %s! The file has been downloaded as '%s.txt'.\n", category, category);

    // Create the appropriate .txt file
    char filename[100];
    snprintf(filename, sizeof(filename), "%s.txt", category);
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        if (strcmp(category, "Games") == 0) {
            fprintf(file, "Games Downloaded:\n\n- GTA VI\n- HALF LIFE 3\n- RDR2\n");
        } else if (strcmp(category, "Movies") == 0) {
            fprintf(file, "Movies Downloaded:\n\n- JOHN WICK\n- INTERSTELLAR\n- THE GODFATHER\n");
        } else if (strcmp(category, "Anime") == 0) {
            fprintf(file, "Anime Downloaded:\n\n- ATTACK ON TITAN\n- DEATH NOTE\n- JUJUTSU KAISEN\n");
        }
        fclose(file);
    } else {
        printf("Error creating the file for %s.\n", category);
    }
}

int main(int argc, char *argv[]) {
    int choice;

    printf("Select a download category:\n");
    printf("1. Download Games\n");
    printf("2. Download Movies\n");
    printf("3. Download Anime\n");
    printf("Enter your choice (1, 2, or 3): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            simulate_download("Games");
            break;
        case 2:
            simulate_download("Movies");
            break;
        case 3:
            simulate_download("Anime");
            break;
        default:
            printf("Invalid choice. Please select 1, 2, or 3.\n");
            return 1;
    }

    // Check for command line argument
    if (argc > 1) {
        int x = atoi(argv[1]);
        mkfifo("mypipe", 0666);
        int fd = open("mypipe", O_WRONLY);
        if (fd != -1) {
            write(fd, &x, sizeof(x));
            close(fd);
        } else {
            perror("Failed to open pipe");
        }
    } else {
        printf("No value passed via command line for pipe communication.\n");
    }

    return 0;
}