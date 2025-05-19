#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ncurses.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_QUEUE 100  // use a constant for max queue size.

sem_t *semaphore = NULL;

int ram, hard, cores, activecores;
int ogRam, ogHard;
int fd[2];

struct p_param {
    int pid;
    char filename[100];
    int r;
    int h;
};

int iterator = -1, front = -1;
struct p_param queue[MAX_QUEUE];
struct p_param *running = NULL;
struct p_param obj;

// Function to wait so certain outputs are visible.
void press_any_key() {
    printf("Press any key to continue...");
    getchar();
    getchar();
}

// Check if the queue is empty.
int empty() {
    if (front == -1 && iterator == -1) {
        return 1;
    }
    return 0;
}

// Display the current queue.
bool displayqueue() {
    printf("Current Queue:\n");
    int found = 0;
    for (int i = front + 1; i <= iterator;) {
        if (queue[i].pid != 0) {
            found = 1;
            printf("%d, %s, %d, %d\n", queue[i].pid, queue[i].filename, queue[i].r, queue[i].h);
            i = (i + 1) % MAX_QUEUE;
            if (i == front) {
                break;
            }
        } else {
            i = (i + 1) % MAX_QUEUE;
            if (i == front) {
                break;
            }
        }
    }
    if (!found) {
        printf("Queue is currently empty.\n");
    }
    return found;
}

// Remove a process from the queue by PID.
void removequeue(int pid) {
    int found = 0;
    for (int i = 0; i < MAX_QUEUE; i++) {
        if (pid == queue[i].pid) {
            queue[i].pid = 0;
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Process with PID %d not found in the queue.\n", pid);
    }
}

// Destroy all processes in the system.
void destroyall() {
    pid_t pid;
    int anyKilled = 0;
    // Kill running processes.
    for (int i = 0; i < cores; i++) {
        if (running[i].pid != 0) {
            pid = running[i].pid;
            running[i].pid = 0;
            kill(pid, SIGKILL);
            anyKilled = 1;
        }
    }
    // Kill queued processes.
    for (int i = 0; i < MAX_QUEUE; i++) {
        if (queue[i].pid != 0) {
            pid = queue[i].pid;
            queue[i].pid = 0;
            kill(pid, SIGKILL);
            anyKilled = 1;
        }
    }
    front = -1;
    iterator = -1;
    activecores = cores;
    ram = ogRam;
    hard = ogHard;
    if (anyKilled) {
        printf("All processes have been destroyed.\n");
    } else {
        printf("No active or queued processes to destroy.\n");
    }
}

// Display active processes running on cores.
void activeprocesses() {
    printf("Active Processes on Cores:\n");
    int active = 0;
    for (int i = 0; i < cores; i++) {
        if (running[i].pid != 0) {
            printf("The process %s with process ID %d\n", running[i].filename, running[i].pid);
            active = 1;
        }
    }
    if (!active) {
        printf("No active processes currently running.\n");
    }
}

// Compile a C file.
void compilefile(char *filename) {
    char command[100];
    sprintf(command, "gcc -o %s.out %s", filename, filename);
    if (system(command) != 0) {
        perror("GCC Compilation Failed");
        exit(EXIT_FAILURE);
    }
}

// Run the compiled executable in a new terminal.
void runfile(char *filename, int pid) {
    char cmd[100];
    sprintf(cmd, "x-terminal-emulator -e './%s.out %d' &", filename, pid);
    system(cmd);
}

// Returns the index of the first available (empty) core, or -1 if none are free.
int emptycore() {
    for (int i = 0; i < cores; i++) {
        if (running[i].pid == 0) {
            return i;  // Return index of the empty core.
        }
    }
    return -1;  // No empty core found.
}

// Manage queues.
void insert_to_queue(struct p_param obj) {
    iterator = (iterator + 1) % MAX_QUEUE;
    queue[iterator].pid = obj.pid;
    strcpy(queue[iterator].filename, obj.filename);
    queue[iterator].r = obj.r;
    queue[iterator].h = obj.h;
}

// Create threads for tasks.
void *threads(void *arg) {
    sem_t *semaphore = (sem_t *) arg;
    sem_wait(semaphore);
    if (activecores > 0) {
        activecores--;
        if (empty() != 1) {  // If queue is not empty.
            front = (front + 1) % MAX_QUEUE;
            // Check if resources are sufficient.
            if (ram < queue[front].r || hard < queue[front].h) {
                printf("Insufficient resources for process %d. Re-queuing it.\n", queue[front].pid);
                insert_to_queue(queue[front]);  // Reinsert if insufficient resources.
                sem_post(semaphore);
                press_any_key();
                return NULL;
            }
            int x = emptycore();
            running[x].pid = queue[front].pid;
            running[x].r = queue[front].r;
            strcpy(running[x].filename, queue[front].filename);
            // Update system resources.
            ram -= queue[front].r;
            hard -= queue[front].h;
            // Compile and run the file.
            compilefile(queue[front].filename);
            runfile(queue[front].filename, queue[front].pid);
            // Reset queue if empty.
            if (front == iterator) {
                front = -1;
                iterator = -1;
            }
        } else {
            printf("No process in queue to execute.\n");
        }
    } else {
        // All cores busy, reinsert process back into queue.
        printf("All cores are currently occupied. Re-queuing process.\n");
        front = (front + 1) % MAX_QUEUE;
        insert_to_queue(queue[front]);
        press_any_key();
    }
    sem_post(semaphore);
    return NULL;
}

// Monitors finished processes via pipe, frees resources, and schedules new ones if needed.
void *idreturn(void *arg) {
    while (1) {
        int flag = 0;
        int fd = open("mypipe", O_RDONLY);
        int x = 0;
        read(fd, &x, sizeof(x));
        close(fd);
        if (x != 0) {
            flag = 1;
            for (int i = 0; i < cores; i++) {
                if (running[i].pid == x) {
                    // Free the core and update resources.
                    running[i].pid = 0;
                    ram += running[i].r;
                    activecores++;
                    break;
                }
            }
        }
        // If queue is not empty and a process just finished, spawn a new thread.
        if (empty() != 1 && flag == 1) {
            pthread_t p;
            pthread_create(&p, NULL, threads, semaphore);
            pthread_join(p, NULL);
        }
    }
}

// Create a process.
int create_process(char *filename, int r, int h) {
    pthread_t t;
    if (pipe(fd) == -1) {
        perror("Pipe Error");
        exit(EXIT_FAILURE);
    }
    pid_t p = fork();
    if (p == -1) {
        printf("Process not created: %s\n", filename);
        return 0;
    } else if (p == 0) {
        int x = getpid();
        // Child process writes its PID to the pipe.
        close(fd[0]);
        write(fd[1], &x, sizeof(x));
        close(fd[1]);
        exit(EXIT_SUCCESS);
    } else {
        close(fd[1]); // Close writing end of pipe in parent.
        int id = 0;
        read(fd[0], &id, sizeof(id)); // Read child's PID.
        obj.pid = id;
        if ((iterator + 1) % MAX_QUEUE != front) {
            strcpy(obj.filename, filename);
            obj.r = r;
            obj.h = h;
            insert_to_queue(obj);
            printf("Process '%s' with PID %d created and added to queue.\n", filename, id);
        } else {
            printf("Queue is full right now. Try again later.\n");
            return 0;
        }
    }
    pthread_create(&t, NULL, threads, semaphore);
    pthread_join(t, NULL);
    return 0;
}

// OS name.
void intro() {
    system("clear");
    printf("\t\t\t\tWelcome to jigglyOS\n");
    printf("\t\t    23F-0574 Ali Meekal & 23F-0765 Jawad Jameel    \n");
    sleep(5);
    system("clear");
}

// Booting animation.
int opening() {
    system("clear");
    initscr();
    curs_set(0);
    int X, Y;
    getmaxyx(stdscr, Y, X);
    int textX = X / 2 - 3;
    int textY = Y / 2;
    char chars[] = {'\\', '|', '/', '-'};
    for (int i = 0; i < 20; i++) {
        mvprintw(textY, textX, "OS BOOTING...");
        mvprintw(textY + 1, textX, "[%c]", chars[i % 4]);
        refresh();
        usleep(500000);
        mvprintw(textY + 1, textX, "[ ]");
        refresh();
    }
    endwin();
    system("clear");
    return 0;
}

// Shutting down animation.
int shuttingdown() {
    system("clear");
    initscr();
    curs_set(0);
    int X, Y;
    getmaxyx(stdscr, Y, X);
    int textX = X / 2 - 3;
    int textY = Y / 2;
    char chars[] = {'\\', '|', '/', '-'};

    for (int i = 0; i < 20; i++) {
        mvprintw(textY, textX, "SHUTTING DOWN...");
        mvprintw(textY + 1, textX, "[%c]", chars[i % 4]);
        refresh();
        usleep(500000);
        mvprintw(textY + 1, textX, "[ ]");
        refresh();
    }
    system("pkill -f gnome-terminal");
    return 0;
}

// Display time.
int cloc() {
    time_t currentTime;
    struct tm *localTime;
    currentTime = time(NULL);
    localTime = localtime(&currentTime);
    printf("Current Time: %02d:%02d:%02d\n",
           localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
    return 0;
}

// Main program.
int main() {
    printf("\033[1;31m"); // Set the text color to bold red.
    int option;
    intro();
    opening();
    pthread_t checkid;
    pthread_create(&checkid, NULL, idreturn, NULL);
    printf("Input RAM(MBs), HARD DISK(MBs), & Cores: ");
    scanf("%d", &ram);
    scanf("%d", &hard);
    scanf("%d", &cores);
    getchar();
    ogRam = ram;
    ogHard = hard;
    running = (struct p_param *)malloc(cores * sizeof(struct p_param));
    for (int i = 0; i < cores; i++) {
        running[i].pid = 0;
    }
    activecores = cores;
    char semaphore_name[100];
    sprintf(semaphore_name, "semaphore_%d", getpid());
    semaphore = sem_open(semaphore_name, O_CREAT | O_EXCL, 0644, cores);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    do {
    	system("clear");
        cloc();
        printf("\nMenu:\n");
        printf("(0). Shut Down\n");
        printf("(1). Notepad\n");
        printf("(2). Calculator\n");
        printf("(3). Create File\n");
        printf("(4). Copy File\n");
        printf("(5). Move File\n");
        printf("(6). Delete File\n");
        printf("(7). Tic Tac Toe Game\n");
        printf("(8). Temperature Calculator\n");
        printf("(9). Reverse String\n");
        printf("(10). Letter Count\n");
        printf("(11). Age Calculator\n");
        printf("(12). Play Song\n");
        printf("(13). Number Guessing\n");
        printf("(14). Gender Guessing\n");
        printf("(15). Download\n");
        printf("(16). Kernel Mode\n");
        printf("(17). RAM Info\n");
        printf("(18). Hard Disk Info\n");
        printf("(19). Waiting Queue\n");
        printf("(20). Active Processes\n");
        printf("Choice: ");
        scanf("%d", &option);
        switch (option) {
            case 0:
                shuttingdown();
                break;
            case 1: create_process("notepad.c", 10, 20); break;
            case 2: create_process("calculator.c", 5, 10); break;
            case 3: create_process("create.c", 2, 5); break;
            case 4: create_process("copy.c", 2, 5); break;
            case 5: create_process("move.c", 2, 5); break;
            case 6: create_process("delete.c", 2, 0); break;
            case 7: create_process("ttt.c", 20, 30); break;
            case 8: create_process("tempcal.c", 5, 8); break;
            case 9: create_process("revstr.c", 2, 3); break;
            case 10: create_process("lcount.c", 2, 30); break;
            case 11: create_process("agecal.c", 2, 15); break;
            case 12: create_process("psong.c", 2, 30); break;
            case 13: create_process("numguess.c", 6, 12); break;
            case 14: create_process("gender.c", 6, 12); break;
            case 15: create_process("download.c", 30, 50); break;
            case 16: {
                int choice;
                do {
                    system("clear");
                    printf("Kernel Mode Menu:\n");
                    printf("(0). Exit Kernel Mode\n");
                    printf("(1). Destroy All Processes\n");
                    printf("(2). Delete a Process from Ready Queue\n");
                    printf("Choice: ");
                    scanf("%d", &choice);
                    if (choice == 0) {
                        break;
                    } else if (choice == 1) {
                        destroyall();
                        press_any_key();
                    } else if (choice == 2) {
                        bool found = displayqueue();
                        if (found) {
                        int id;
                        printf("Enter process ID to delete: ");
                        scanf("%d", &id);
                        removequeue(id);
                        press_any_key();
                        }
                        else {
                        press_any_key();
                        }
                    } else {
                        printf("Invalid choice!\n");
                        press_any_key();
                    }
                } while (choice != 0);
                break;
            }
            case 17:
                printf("RAM: %d MB\n", ram);
                press_any_key();
                break;
            case 18:
            	printf("Hard Disk: %d MB\n", hard);
            	press_any_key();
                break;
            case 19:
                bool found = displayqueue();
                press_any_key();
                break;
            case 20:
                activeprocesses();
                press_any_key();
                break;
            default:
                printf("Invalid option selected.\n");
                press_any_key();
                break;
        }
    } while (option != 0);
    pthread_join(checkid, NULL);
    if (sem_close(semaphore) < 0) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }
    printf("\033[0m"); // Reset text color.
    return 0;
}