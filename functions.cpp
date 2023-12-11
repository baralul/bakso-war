#include "functions.hpp"

#include <ctime>
#include <string>

static short int state;

const char *explosion[3] = {"\\||//",
                            "--  --",
                            "//||\\"};

void delay(int noOfSeconds)
{
    int milliSeconds = 1000 * noOfSeconds;
    clock_t start = clock();
    while (clock() < start + milliSeconds)
        ;
}

void loseMessage()
{
    delay(2000);
    clear();
    int height = getmaxy(stdscr);
    int width = getmaxx(stdscr);

    mvprintw(height / 2 - 4, width / 2 - 28, "YOU HAVE FAILED TO DEFEND BAKSOVILLE.");
    refresh();
    delay(2000);
    mvprintw(height / 2 - 2, width / 2 - 28, "The people of Baksoville were counting on you. You let them down.");
    refresh();
    delay(2000);
    mvprintw(height / 2, width / 2 - 28, "The aliens have successfully stolen the secret bakso recipe.");
    refresh();
    delay(2000);
    mvprintw(height / 2 + 2, width / 2 - 28, "MISSION: BAKSO HEIST SUCCESSFUL");
    refresh();
    delay(3000);

    state = GAME_LOST;
}

void winMessage()
{
    delay(2000);
    clear();
    int height = getmaxy(stdscr);
    int width = getmaxx(stdscr);

    mvprintw(height / 2 - 4, width / 2 - 30, "YOU HAVE SAVED THE BAKSO RECIPE!");
    refresh();
    delay(2000);
    mvprintw(height / 2 - 2, width / 2 - 30, "The aliens from Baksoville have been thwarted in their attempt to steal the bakso recipe.");
    refresh();
    delay(2000);
    mvprintw(height / 2, width / 2 - 30, "The secret bakso recipe remains safe on Earth.");
    refresh();
    delay(2000);
    mvprintw(height / 2 + 2, width / 2 - 30, "MISSION: BAKSO DEFENDED!");
    refresh();
    delay(3000);

    state = GAME_WON;
}

Player::Player(int xMax, int yMax)
{
    maxWidth = xMax - 10;
    maxHeight = yMax - 6;
    loc = xMax / 2;
}

void Player::printSpaceship()
{
    for (int i = 0; i < 3; i++)
        mvprintw(maxHeight + i, loc - 3, spaceship[i]);
}

void Player::moveRight()
{
    if (loc >= maxWidth)
        return;

    for (int i = 0; i < 3; i++)
        mvprintw(maxHeight + i, loc - 3, "      ");

    loc += 2;
    printSpaceship();
}

void Player::moveLeft()
{
    if (loc <= 10)
        return;

    for (int i = 0; i < 3; i++)
        mvprintw(maxHeight + i, loc - 3, "      ");

    loc -= 2;
    printSpaceship();
}

void Player::Shoot(Player &P, EnemyFleet &E)
{
    int move, currentLoc = loc;

    for (int i = maxHeight - 2; i >= -1 && !state; i--)
    {
        int ch1 = mvinch(i, currentLoc);
        int ch2;

        if (E.level[0] % 2 == 1)
            ch2 = mvinch(i, currentLoc - 1);
        else if (E.level[0] % 2 == 0)
            ch2 = mvinch(i, currentLoc + 1);

        if (ch1 == '\\' || ch1 == '/' || ch1 == '^' || ch2 == '\\' || ch2 == '/' || ch2 == '^')
        {
            int lvl;
            bool check = false;

            for (int l = 0; l < 4; l++)
                if (i - 2 == E.level[l])
                {
                    lvl = l;
                    check = true;
                    break;
                }

            if (!check)
                continue;

            ++E.shipsDown;
            mvprintw(getmaxy(stdscr) - 1, getmaxx(stdscr) - 16, "Ships Left: %d ", 40 - E.shipsDown);

            for (int j = lvl * 10; j < (lvl + 1) * 10; j++)
            {
                if (currentLoc >= E.fleet[j].loc && currentLoc <= E.fleet[j].loc + 5 && !E.hit[j])
                {
                    for (int k = 0; k < 3; k++)
                        mvprintw(E.level[j / 10] + k, E.fleet[j].loc, explosion[k]);

                    refresh();
                    delay(30);
                    E.hit[j] = true;
                    E.printFleet();
                    goto hit;
                }
            }

        hit:
            mvprintw(i + 1, currentLoc, " ");
            flushinp();

            if (E.shipsDown == 40)
                winMessage();

            return;
        }

        mvprintw(i + 1, currentLoc, " ");
        mvprintw(i, currentLoc, "*");
        moveFleet(E);
        refresh();
        delay(30);
        move = getch();

        if (move != ERR)
        {
            switch (move)
            {
            case KEY_LEFT:
                P.moveLeft();
                break;
            case KEY_RIGHT:
                P.moveRight();
                break;
            case 'p':
            case 'P':
                pauseGame();
                break;
            case 'e':
            case 'E':
                state = GAME_ENDED;
                goto return_to_menu;
            default:;
            }
        }
    }

return_to_menu:;
    flushinp();
}

EnemyFleet::EnemyFleet(int height, int width)
{
    shipsDown = 0;
    yMax = 1;
    xMax = width - 10;
    for (int i = 0; i < 4; i++)
        level[i] = yMax + 5 * i;

    for (int j = 0; j < 4; j++)
    {
        int L = 9;
        for (int i = 10 * j; i < 10 * (j + 1); i++)
        {
            fleet[i].loc = L;
            L += 8;
        }
    }
}

void EnemyFleet::printSpaceship(int i)
{
    for (int j = 0; j < 3; j++)
        mvprintw(level[i / 10] + j, fleet[i].loc, fleet[i].spaceship[j]);
}

void EnemyFleet::printFleet()
{
    for (int i = 0; i < 40; i++)
        if (!hit[i])
            printSpaceship(i);
}

void EnemyFleet::shiftFleetRight()
{
    for (int i = 0; i < 40; i++)
    {
        for (int j = 0; j < 3; j++)
            mvprintw(level[i / 10] + j, fleet[i].loc, "      ");
    }

    for (int i = 0; i < 40; i++)
        ++fleet[i].loc;

    printFleet();
}

void EnemyFleet::shiftFleetLeft()
{
    for (int i = 0; i < 40; i++)
    {
        for (int j = 0; j < 3; j++)
            mvprintw(level[i / 10] + j, fleet[i].loc, "      ");
    }

    for (int i = 0; i < 40; i++)
        --fleet[i].loc;

    printFleet();
}

void EnemyFleet::shiftFleetDown(bool end)
{
    if (level[3] == getmaxy(stdscr) - 10)
        loseMessage();

    for (int i = 0; i < 40; i++)
    {
        for (int j = 0; j < 3; j++)
            mvprintw(level[i / 10] + j, fleet[i].loc, "      ");
    }

    for (int i = 0; i < 4; i++)
        ++level[i];

    mvprintw(getmaxy(stdscr) - 1, getmaxx(stdscr) - 40, "Distance left: %d ", getmaxy(stdscr) - 10 - level[3]);

    if (end)
        shiftFleetLeft();
    else
        shiftFleetRight();
}

void gameTitle(int height, int width)
{
    mvwprintw(stdscr, height / 2 - 11, width / 2 - 34, "");
    mvwprintw(stdscr, height / 2 - 10, width / 2 - 34, "888 88b,         888                      Y8b Y8b Y888P");
    mvwprintw(stdscr, height / 2 - 9, width / 2 - 34, "888 88P'  ,'Y88b 888 ee  dP'Y  e88 88e     Y8b Y8b Y8P   ,'Y88b 888,8,");
    mvwprintw(stdscr, height / 2 - 8, width / 2 - 34, "888 8K   '8' 888 888 P  C88b  d888 888b     Y8b Y8b Y   '8' 888 888 '");
    mvwprintw(stdscr, height / 2 - 7, width / 2 - 34, "888 88b, ,ee 888 888 b   Y88D Y888 888P      Y8b Y8b    ,ee 888 888");
    mvwprintw(stdscr, height / 2 - 6, width / 2 - 34, "888 88P' '88 888 888 8b d,dP   '88 88'        Y8P Y     '88 888 888");
    mvwprintw(stdscr, height / 2 - 5, width / 2 - 34, "");
    mvwprintw(stdscr, height / 2 - 4, width / 2 - 34, "");
}

void gameIntro(int height, int width)
{
    mvprintw(height / 2 - 4, width / 2 - 29, "Aliens from the planet Baksoville are visiting Earth with a unique mission.");
    refresh();
    delay(2000);
    mvprintw(height / 2 - 2, width / 2 - 29, "They have heard about the legendary bakso on Earth and want to steal the recipe.");
    refresh();
    delay(2000);
    mvprintw(height / 2, width / 2 - 29, "You, as the guardian of the secret bakso recipe, are Earth's last hope.");
    refresh();
    delay(2000);
    mvprintw(height / 2 + 2, width / 2 - 29, "Your mission is to protect the bakso at all costs and thwart the Baksoville invasion.");
    refresh();
    delay(2000);
}

void ControlsAndRules(int height, int width)
{
    clear();
    mvprintw(height / 2 - 6, width / 2 - 18, "             CONTROLS");
    mvprintw(height / 2 - 4, width / 2 - 18, " Press LEFT arrow key to move left.");
    mvprintw(height / 2 - 3, width / 2 - 18, "Press RIGHT arrow key to move right.");
    mvprintw(height / 2 - 2, width / 2 - 18, " Press SPACE key to shoot bullets.");
    mvprintw(height / 2, width / 2 - 39, "                                   RULES");
    mvprintw(height / 2 + 2, width / 2 - 15, "Protect the secret bakso recipe!");
    mvprintw(height / 2 + 3, width / 2 - 30, "Shoot down the Baksoville invaders before they reach the bottom.");
    mvprintw(height / 2 + 4, width / 2 - 25, "If they succeed, humanity loses the beloved bakso.");
    mvprintw(height / 2 + 9, width / 2 - 20, "Press any key to return to the start menu.");
    flushinp();
    refresh();
    getch();
}

void moveFleet(EnemyFleet &E)
{
    if (E.fleet[0].loc == 8)
    {
        E.shiftFleetDown(false);
    }
    else if (E.fleet[39].loc + 6 == E.xMax)
    {
        E.shiftFleetDown(true);
    }

    if (E.level[0] % 2 == 1)
        E.shiftFleetRight();
    else if (E.level[0] % 2 == 0)
        E.shiftFleetLeft();
}

void pauseGame()
{
    int ch;
    flushinp();
    mvprintw(getmaxy(stdscr) - 1, 2, "Press any key to resume");
    refresh();
    while ((ch = getch()) == ERR)
        delay(1);

    if (ch == 'e' || ch == 'E')
        state = GAME_ENDED;

    mvprintw(getmaxy(stdscr) - 1, 2, "P: Pause               ");
    refresh();
}

void startGame(int height, int width)
{
    state = IN_PROGRESS;

    clear();

    keypad(stdscr, true);

    Player P(width, height);
    EnemyFleet E(height, width);

    P.printSpaceship();
    E.printFleet();

    mvprintw(height - 1, 2, "P: Pause                    E: Exit");
    mvprintw(height - 1, width - 16, "Ships Left: %d ", 40 - E.shipsDown);
    mvprintw(height - 1, width - 40, "Distance left: %d ", height - 10 - E.level[3]);

    scrollok(stdscr, true);
    nodelay(stdscr, true);

    curs_set(0);

    int move;

    while (state == IN_PROGRESS)
    {
        move = getch();

        if (move != ERR)
        {
            switch (move)
            {
            case KEY_LEFT:
                flushinp();
                P.moveLeft();
                break;
            case KEY_RIGHT:
                flushinp();
                P.moveRight();
                break;
            case 32:
                P.Shoot(P, E);
                break;
            case 'p':
            case 'P':
                pauseGame();
                break;
            case 'e':
            case 'E':
                goto return_to_menu;
            default:
                continue;
            }
        }

        if (!state)
        {
            napms(30);
            moveFleet(E);
        }
    }

return_to_menu:
    scrollok(stdscr, false);
    nodelay(stdscr, false);
}
void startMenu(int height, int width)
{
    clear();

    keypad(stdscr, true);

    curs_set(0);

    std::string options[3] = {"Start Game", "Controls and Rules", "Exit"};

    int chosen_option, highlight = 0;

    while (true)
    {
        clear();

        gameTitle(height, width);

        for (int i = 0; i < 3; i++)
            if (i == highlight)
            {
                wattron(stdscr, A_REVERSE);
                mvprintw(i + height / 2, width / 2 - 8, options[i].c_str());
                wattroff(stdscr, A_REVERSE);
            }
            else
                mvprintw(i + height / 2, width / 2 - 8, options[i].c_str());

        chosen_option = getch();

        switch (chosen_option)
        {
        case KEY_UP:
            if (highlight == 0)
                break;
            --highlight;
            break;
        case KEY_DOWN:
            if (highlight == 2)
                break;
            ++highlight;
            break;
        default:;
        }

        if (chosen_option == 10)
        {
            switch (highlight)
            {
            case 0:
                clear();
                gameIntro(height, width);
                flushinp();
                mvprintw(height / 2 + 4, width / 2 - 29, "Press any key to start the game.");
                refresh();
                getch();
                startGame(height, width);
                break;
            case 1:
                ControlsAndRules(height, width);
                break;
            case 2:
                goto return_to_start_menu;
                break;
            }
        }
    }

return_to_start_menu:;
}

