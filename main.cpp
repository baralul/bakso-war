#include "functions.hpp"

int main()
{
    initscr();
    noecho();

    int height = getmaxy(stdscr);
    int width = getmaxx(stdscr);

    startMenu(height, width);

    clear();
    flushinp();

    mvprintw(height / 2, width / 2 - 9, "Press any key to exit.");
    refresh();
    getch();

    endwin();

    return 0;
}

