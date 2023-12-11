#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_

#include <ncurses.h>

#define GAME_LOST -1
#define IN_PROGRESS 0
#define GAME_WON 1
#define GAME_ENDED 2

struct Enemy
{
    int loc;
    const char *spaceship[3] = {"\\^||^/",
                                " \\||/ ",
                                "  \\/  "};
};

class EnemyFleet
{
public:
    Enemy fleet[40];
    int level[4], shipsDown, yMax, xMax;
    bool hit[40] = {false};

    EnemyFleet(int height, int width);
    void printSpaceship(int i);
    void printFleet();
    void shiftFleetRight();
    void shiftFleetLeft();
    void shiftFleetDown(bool end);
};

class Player
{
    int loc, maxWidth, maxHeight;
    const char *spaceship[3] = {"  /\\  ",
                                " /||\\ ",
                                "/_||_\\"};

public:
    Player(int yMax, int xMax);
    void printSpaceship();
    void moveRight();
    void moveLeft();
    void Shoot(Player &P, EnemyFleet &E);
};

void startMenu(int height, int width);
void startGame(int height, int width);
void moveFleet(EnemyFleet &E);
void pauseGame();

#endif

