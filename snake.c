#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define MAX_LENGTH 100
#define DELAY 100000

int main() {
    int row, col;
    int x[MAX_LENGTH], y[MAX_LENGTH]; // x and y coordinates of snake's body
    int length = 3; // initial length of snake
    int direction = KEY_RIGHT; // initial direction of snake
    int foodx, foody; // coordinates of food
    int score = 0; // current score

    // initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    // get window size
    getmaxyx(stdscr, row, col);

    // initialize snake's body
    for (int i = 0; i < length; i++) {
        x[i] = row/2;
        y[i] = col/2 - i;
    }

    // generate initial food
    srand(time(0));
    foodx = rand() % (row-2) + 1;
    foody = rand() % (col-2) + 1;

    // main game loop 
    while (1) {
        clear();

        // draw snake
        for (int i = 0; i < length; i++) {
            mvprintw(x[i], y[i], "o");
        }

        // draw food
        mvprintw(foodx, foody, "x");

        // display score
        mvprintw(0, 0, "Score: %d", score);

        // move snake
        switch (direction) {
            case KEY_UP:
                x[0]--;
                break;
            case KEY_DOWN:
                x[0]++;
                break;
            case KEY_LEFT:
                y[0]--;
                break;
            case KEY_RIGHT:
                y[0]++;
                break;
        }

        // check for collision with walls or self
        if (x[0] == 0 || x[0] == row-1 || y[0] == 0 || y[0] == col-1) {
            mvprintw(row/2, col/2-5, "GAME OVER");
            refresh();
            sleep(2);
            break;
        }

        for (int i = 1; i < length; i++) {
            if (x[0] == x[i] && y[0] == y[i]) {
                mvprintw(row/2, col/2-5, "GAME OVER");
                refresh();
                sleep(2); 
                break;
            }
        }

        // check for collision with food
        if (x[0] == foodx && y[0] == foody) {
            length++;
            score++;
            foodx = rand() % (row-2) + 1;
            foody = rand() % (col-2) + 1;
        }

        refresh();
        usleep(DELAY);

        // get user input
        int ch = getch();

        switch (ch) {
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                if (direction + ch != KEY_UP + KEY_DOWN && direction + ch != KEY_LEFT + KEY_RIGHT) {
                    direction = ch;
                }
                break;
            case 'q':
            case 'Q':
                endwin();
                return 0;
        }
    }

    endwin();
    return 0;
}
