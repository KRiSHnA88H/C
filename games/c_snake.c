#include <stdio.h>
#include <stdlib.h>
#include <conio.h>   // for _kbhit() and _getch() -> keyboard input
#include <time.h>    // for random food generation (using time as seed)
#include <windows.h> // for system("cls") on Windows

// ==== Game Constants ====
#define ROWS 25      // Number of rows in the game board
#define COLS 25      // Number of columns in the game board
#define MAX_LEN 256  // Maximum snake length

// ==== Global Variables ====

// Board matrix (ROWS x COLS)
char board[ROWS][COLS];

// Game states
int isgameover = 0;  // Flag to end the game
int eaten = 1;       // Indicates if food has been eaten
int snake_len = 1;   // Initial snake length
int dx = 1, dy = 0;  // Initial direction (moving right)

// Snake head position (start from middle of board)
int posx = ROWS / 2;
int posy = COLS / 2;

// Snake body coordinates (a[i][0] -> row, a[i][1] -> col)
int a[MAX_LEN][2];

// Food position
int foodx, foody;

// Timing variables
clock_t last = 0;    // To track last move time
int delay = 150;     // Snake speed (milliseconds)

// ==== FUNCTIONS ====

// Draw empty board (borders = '*', inside = ' ')
void draw_board() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (i == 0 || j == 0 || i == ROWS - 1 || j == COLS - 1) {
                board[i][j] = '*';  // Border
            } else {
                board[i][j] = ' ';  // Empty space
            }
        }
    }
}

// Place snake on board
void snake() {
    // Shift body (move each segment to the previous segment’s position)
    for (int i = snake_len; i > 0; i--) {
        a[i][0] = a[i - 1][0];
        a[i][1] = a[i - 1][1];
    }

    // Update head position
    a[0][0] = posx;
    a[0][1] = posy;

    // Draw snake on board
    for (int k = 0; k < snake_len; k++) {
        if (k == 0)
            board[a[k][0]][a[k][1]] = '@'; // Snake head
        else
            board[a[k][0]][a[k][1]] = 'O'; // Snake body
    }
}

// Print board to screen (with ANSI colors)
void print_board() {
    fflush(stdout);
    printf("\033[H"); // Move cursor to top-left (instead of clearing screen)

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (i == 0 || j == 0 || i == ROWS - 1 || j == COLS - 1)
                printf("\033[35m%c\033[0m", board[i][j]);   // Purple border
            else if (i == foodx && j == foody)
                printf("\033[31m%c\033[0m", board[i][j]);   // Red food
            else
                printf("\033[32m%c\033[0m", board[i][j]);   // Green snake / empty
        }
        printf("\n");
    }
}

// Update snake’s head position
void transform(int x, int y) {
    posx += x;
    posy += y;
}

// Handle keyboard input
void input() {
    if (_kbhit()) {  // Check if a key was pressed
        int m = _getch();

        // Handle arrow keys (they come as two-byte sequence)
        if (m == 0 || m == 224) {
            int arrow = _getch(); // Get actual arrow code
            switch (arrow) {
            case 72: // Up
                if (dx != 1 || snake_len == 1) { dx = -1; dy = 0; }
                break;
            case 80: // Down
                if (dx != -1 || snake_len == 1) { dx = 1; dy = 0; }
                break;
            case 75: // Left
                if (dy != 1 || snake_len == 1) { dx = 0; dy = -1; }
                break;
            case 77: // Right
                if (dy != -1 || snake_len == 1) { dx = 0; dy = 1; }
                break;
            }
        } else {
            // WASD keys as alternative controls
            switch (m) {
            case 'w': if (dx != 1 || snake_len == 1) { dx = -1; dy = 0; } break;
            case 's': if (dx != -1 || snake_len == 1) { dx = 1; dy = 0; } break;
            case 'a': if (dy != 1 || snake_len == 1) { dx = 0; dy = -1; } break;
            case 'd': if (dy != -1 || snake_len == 1) { dx = 0; dy = 1; } break;
            case 'q': isgameover = 1; break; // Quit game
            }
        }
    }
}

// Check collisions (walls or self)
void collision() {
    // Check wall collision
    if (posx == ROWS - 1 || posx == 0 || posy == 0 || posy == COLS - 1) {
        isgameover = 1;
    }

    // Check self collision
    for (int i = 1; i < snake_len; i++) {
        if (posx == a[i][0] && posy == a[i][1]) {
            isgameover = 1;
        }
    }
}

// Generate food if needed
void food() {
    if (eaten) {
        int max = ROWS - 2, min = 1;
        foodx = (rand() % (max - min + 1)) + min;
        foody = (rand() % (max - min + 1)) + min;
        eaten = 0; // Reset eaten flag
    }
    board[foodx][foody] = '#'; // Place food on board
}

// Check if food is eaten
void Eaten() {
    if (posx == foodx && posy == foody) {
        eaten = 1;
        snake_len++; // Increase length
        if (snake_len >= MAX_LEN) {
            printf("You won!\n");
            exit(0);
        }
    }
}

// ==== MAIN FUNCTION ====
int main() {
    int difficulty = 0;
    srand(time(NULL)); // Seed random number generator

    // Show game instructions
    printf("========== Snake Game ==========\n");
    printf("Controls:\n");
    printf("  Arrow Keys or WASD to move\n");
    printf("  Press 'q' to quit\n\n");

    // Difficulty selection
    printf("Choose Difficulty (1 = Easy, 2 = Medium, 3 = Hard): ");
    if (scanf("%d", &difficulty) != 1) {
        printf("Invalid input! Defaulting to Medium.\n");
        difficulty = 2;
    }

    switch (difficulty) {
    case 1: delay = 125; break; // Easy
    case 2: delay = 100; break; // Medium
    case 3: delay = 50; break;  // Hard
    default:
        printf("Invalid choice! Defaulting to Medium.\n");
        delay = 100;
        break;
    }
    system("cls"); // Clear screen

    // Hide cursor for smooth rendering
    printf("\033[?25l");

    last = clock(); // Start timer

    // Move cursor to top-left
    printf("\033[H");

    // ==== Game Loop ====
    while (!isgameover) {
        input(); // Handle input

        clock_t now = clock();
        // Move snake after a delay (speed control)
        if ((now - last) * 1000 / CLOCKS_PER_SEC >= delay) {
            transform(dx, dy); // Move snake
            Eaten();           // Check if food eaten
            collision();       // Check collisions
            last = now;        // Reset timer

            draw_board();      // Redraw board
            snake();           // Draw snake
            food();            // Place food
            print_board();     // Display board
        }
    }

    // Restore cursor
    printf("\033[?25h");
    printf("Game Over!\n");
    return 0;
}
