#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 80
#define HEIGHT 40
#define PACMAN1 'C'
#define PACMAN2 'P'
#define WALL '|'
#define FOOD '*'
#define EMPTY ' '
#define DEMON 'X'
#define ANGEL 'A'
#define PORTAL 'O'

int res = 0;
int score1 = 0;
int score2 = 0;
int totalSteps = 0;
int pacman1_x, pacman1_y;
int pacman2_x, pacman2_y;
char board[HEIGHT][WIDTH];
int food = 0;
int curr = 0;

// Coordinates for portals in all four directions
int portal_east_x = WIDTH - 2;
int portal_east_y = HEIGHT / 2;
int portal_west_x = 1;
int portal_west_y = HEIGHT / 2;
int portal_north_x = WIDTH / 2;
int portal_north_y = 1;
int portal_south_x = WIDTH / 2;
int portal_south_y = HEIGHT - 2;

void addDemons(int numDemons) {
    int i, j;
    while (numDemons--) {
        int i = (rand() % (HEIGHT + 1));
        int j = (rand() % (WIDTH + 1));
        if (board[i][j] != WALL && board[i][j] != PACMAN1 && board[i][j] != PACMAN2 && board[i][j] != PORTAL) {
            board[i][j] = DEMON;
        }
    }
}

void addAngels(int numAngels) {
    int i, j;
    while (numAngels--) {
        int i = (rand() % (HEIGHT + 1));
        int j = (rand() % (WIDTH + 1));
        if (board[i][j] != WALL && board[i][j] != PACMAN1 && board[i][j] != PACMAN2 && board[i][j] != PORTAL) {
            board[i][j] = ANGEL;
        }
    }
}

void initialize() {
    int i, j, count;
    srand(time(NULL));
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (i == 0 || j == WIDTH - 1 || j == 0 || i == HEIGHT - 1) {
                board[i][j] = WALL;
            } else
                board[i][j] = EMPTY;
        }
    }
    count = (WIDTH * HEIGHT) / 15; // Adjust to add more walls
    while (count != 0) {
        i = (rand() % (HEIGHT + 1));
        j = (rand() % (WIDTH + 1));
        if (board[i][j] != WALL && board[i][j] != PACMAN1 && board[i][j] != PACMAN2) {
            board[i][j] = WALL;
            count--;
        }
    }
    addDemons((WIDTH * HEIGHT) / 80); // Adjust to add more demons
    addAngels((WIDTH * HEIGHT) / 200); // Adjust to add fewer angels
    pacman1_x = WIDTH / 4;
    pacman1_y = HEIGHT / 2;
    pacman2_x = (3 * WIDTH) / 4;
    pacman2_y = HEIGHT / 2;
    board[pacman1_y][pacman1_x] = PACMAN1;
    board[pacman2_y][pacman2_x] = PACMAN2;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (i % 2 == 0 && j % 2 == 0 && board[i][j] != WALL && board[i][j] != DEMON && board[i][j] != PACMAN1 && board[i][j] != PACMAN2 && board[i][j] != ANGEL) {
                board[i][j] = FOOD;
                food++;
            }
        }
    }
    // Adding portals
    board[portal_east_y][portal_east_x] = PORTAL;
    board[portal_west_y][portal_west_x] = PORTAL;
    board[portal_north_y][portal_north_x] = PORTAL;
    board[portal_south_y][portal_south_x] = PORTAL;
}

void draw() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int console_width;

    // Get console width
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    console_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    int i, j;
    system("cls");
    for (i = 0; i < HEIGHT; i++) {
        // Calculate left padding
        int left_padding = (console_width - WIDTH) / 2;
        for (int k = 0; k < left_padding; k++) {
            printf(" ");
        }
        for (j = 0; j < WIDTH; j++) {
            if (i == pacman1_y && j == pacman1_x) {
                printf("\x1b[35m%c\x1b[0m", board[i][j]); // Pink color for Pacman 1
            } else if (i == pacman2_y && j == pacman2_x) {
                printf("\x1b[34m%c\x1b[0m", board[i][j]); // Blue color for Pacman 2
            } else if (board[i][j] == WALL) {
                printf("\x1b[33m%c\x1b[0m", board[i][j]); // Orange color for walls
            } else if (board[i][j] == FOOD) {
                printf("\x1b[32m%c\x1b[0m", board[i][j]); // Green color for food
            } else if (board[i][j] == DEMON) {
                printf("\x1b[31m%c\x1b[0m", board[i][j]); // Red color for demon
            } else if (board[i][j] == ANGEL) {
                printf("\x1b[36m%c\x1b[0m", board[i][j]); // Cyan color for angel
            } else if (board[i][j] == PORTAL) {
                printf("\x1b[33m%c\x1b[0m", board[i][j]); // Yellow color for portal
            } else {
                printf("%c", board[i][j]);
            }
        }
        printf("\n");
    }
    printf("Player 1 Score: %d\n", score1);
    printf("Player 2 Score: %d\n", score2);
    printf("Total Steps: %d\n", totalSteps);
    printf("Use buttons for w(up), a(left), d(right), and s(down) for Player 1\n");
    printf("Use buttons for i(up), j(left), l(right), and k(down) for Player 2\n");
    printf("Press q to quit\n");
}

void movePlayer(int move_x, int move_y, int player) {
    int x, y;
    if (player == 1) {
        x = pacman1_x + move_x;
        y = pacman1_y + move_y;
    } else {
        x = pacman2_x + move_x;
        y = pacman2_y + move_y;
    }
    totalSteps++;
    if (board[y][x] != WALL) {
        if (board[y][x] == FOOD) {
            if (player == 1) {
                score1++;
            } else {
                score2++;
            }
            food--;
            curr++;
            if (food == 0) {
                res = 2;
                return;
            }
        } else if (board[y][x] == DEMON) {
            res = player; // Player is dead
        } else if (board[y][x] == ANGEL) {
            // Provide higher score boost to the player who collided with the angel
            if (player == 1) {
                score1 += 20; // Increase player 1's score more
            } else {
                score2 += 20; // Increase player 2's score more
            }
        } else if (board[y][x] == PORTAL) {
            // Teleport the player to the corresponding portal
            if (player == 1) {
                if (x == portal_east_x && y == portal_east_y) {
                    pacman1_x = portal_west_x + move_x;
                    pacman1_y = portal_west_y + move_y;
                } else if (x == portal_west_x && y == portal_west_y) {
                    pacman1_x = portal_east_x + move_x;
                    pacman1_y = portal_east_y + move_y;
                } else if (x == portal_north_x && y == portal_north_y) {
                    pacman1_x = portal_south_x + move_x;
                    pacman1_y = portal_south_y + move_y;
                } else if (x == portal_south_x && y == portal_south_y) {
                    pacman1_x = portal_north_x + move_x;
                    pacman1_y = portal_north_y + move_y;
                }
            } else {
                if (x == portal_east_x && y == portal_east_y) {
                    pacman2_x = portal_west_x + move_x;
                    pacman2_y = portal_west_y + move_y;
                } else if (x == portal_west_x && y == portal_west_y) {
                    pacman2_x = portal_east_x + move_x;
                    pacman2_y = portal_east_y + move_y;
                } else if (x == portal_north_x && y == portal_north_y) {
                    pacman2_x = portal_south_x + move_x;
                    pacman2_y = portal_south_y + move_y;
                } else if (x == portal_south_x && y == portal_south_y) {
                    pacman2_x = portal_north_x + move_x;
                    pacman2_y = portal_north_y + move_y;
                }
            }
            return;
        }
        if (player == 1) {
            board[pacman1_y][pacman1_x] = EMPTY;
            pacman1_x = x;
            pacman1_y = y;
            board[pacman1_y][pacman1_x] = PACMAN1;
        } else {
            board[pacman2_y][pacman2_x] = EMPTY;
            pacman2_x = x;
            pacman2_y = y;
            board[pacman2_y][pacman2_x] = PACMAN2;
        }
    }
}

void moveMonsters() {
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (board[i][j] == DEMON) {
                int dx = rand() % 3 - 1;
                int dy = rand() % 3 - 1;
                if (board[i + dy][j + dx] == EMPTY) {
                    board[i + dy][j + dx] = DEMON;
                    board[i][j] = EMPTY;
                }
            }
        }
    }
}

int main() {
    char ch1, ch2; // Separate variables for input of each player
    int totalFood;
    totalSteps = 0;
    initialize();
    food -= 35;
    totalFood = food;
    printf("Enter Y to continue: \n");
    ch1 = getch();
    if (ch1 != 'Y' && ch1 != 'y') {
        printf("Exit Game! ");
        return 1;
    }
    while (1) {
        draw();
        printf("Total Food count: %d\n", totalFood);
        printf("Total Food eaten: %d\n", curr);
        if (res == 1) {
            system("cls");
            printf("Game Over! Player 1 Dead by Demon\n");
            printf("Player 1 Score: %d, Player 2 Score: %d\n", score1, score2);
            if (score1 > score2)
                printf("Player 1 Wins with a score of %d!\n", score1);
            else if (score1 < score2)
                printf("Player 2 Wins with a score of %d!\n", score2);
            else
                printf("It's a tie!\n");
            exit(1);
        }
        if (res == 2) {
            system("cls");
            printf("Game Over! Player 2 Dead by Demon\n");
            printf("Player 1 Score: %d, Player 2 Score: %d\n", score1, score2);
            if (score1 > score2)
                printf("Player 1 Wins with a score of %d!\n", score1);
            else if (score1 < score2)
                printf("Player 2 Wins with a score of %d!\n", score2);
            else
                printf("It's a tie!\n");
            exit(1);
        }
        moveMonsters();

        // Read input for both players
        ch1 = getch();
        ch2 = getch();

        // Move Player 1
        switch (ch1) {
            case 'w':
                movePlayer(0, -1, 1);
                break;
            case 's':
                movePlayer(0, 1, 1);
                break;
            case 'a':
                movePlayer(-1, 0, 1);
                break;
            case 'd':
                movePlayer(1, 0, 1);
                break;
        }

        // Move Player 2
        switch (ch2) {
            case 'i':
                movePlayer(0, -1, 2);
                break;
            case 'k':
                movePlayer(0, 1, 2);
                break;
            case 'j':
                movePlayer(-1, 0, 2);
                break;
            case 'l':
                movePlayer(1, 0, 2);
                break;
        }

        // Check if the 'q' key is pressed to quit the game
        if (ch1 == 'q' || ch2 == 'q') {
            printf("Game Over!\n");
            printf("Player 1 Score: %d, Player 2 Score: %d\n", score1, score2);
            if (score1 > score2)
                printf("Player 1 Wins with a score of %d!\n", score1);
            else if (score1 < score2)
                printf("Player 2 Wins with a score of %d!\n", score2);
            else
                printf("It's a tie!\n");
            return 0;
        }
    }
    return 0;
}
