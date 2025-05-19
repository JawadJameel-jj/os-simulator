#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
char board[3][3];
void init_board(int argc, char *argv[]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}
void print_board() {
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
}
int game_over() {
    // check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return 1;
        }
    }
    for (int j = 0; j < 3; j++) {
        if (board[0][j] != ' ' && board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
            return 1;
        }
    }
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return 1;
    }
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return 1;
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                return 0;
            }
        }
    }
    return 2; 
}
void get_move(int player, int *row, int *col) {
    printf("Player %d's turn (row, col): ", player);
    scanf("%d %d", row, col);
}

int main(int argc, char *argv[]) {
    int player = 1; 
    int row, col;

    init_board(argc, argv);

    while (1) {
        print_board(); 
        get_move(player, &row, &col);
        if (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ') {
            printf("Invalid move, try again.\n");
            continue;
        }
        board[row][col] = (player == 1) ? 'X' : 'O';
        int result = game_over();
        if (result == 1) {
            printf("Player %d wins!\n", player);
            break;
        } else if (result == 2) {
            printf("Tie game!\n");
            break;
        }
        player = (player == 1) ? 2 : 1;
    }

    print_board();
 int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);
    printf("Press any key to continue...");
    getchar();
    getchar();
    return 0;
}