#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

// key code
//#define vk_space 32
//#define vk_enter 10
int key_pressed = 0;

// color
const short c_wall = 1;
const short c_bakso = 2;
const short c_door = 3;
const short c_space = 4;
const short c_life = 5;
const short c_player = 6;
const short c_bullet = 6;
const short c_enemy = 7;
const short c_box = 8;
//const short c_hud = 1;

bool EXIT = false;

// starting default (global variable)
short level = 1;
short score = 0;
short lifes = 3;

int apples_in_level = 0;
int bullet_shoot = false;

// level size
int current_lvl_x;
int current_lvl_y;

// window width & height
int w, h;

// index map object
#define i_wall 1
#define i_bakso 2
#define i_door 3
#define i_space 4
#define i_life 5
#define i_exit 6
#define i_enemy_v 7
#define i_box 8
#define i_enemy_h 9

/////////////////////////////////////////////////////
// menu
const char *menu_logo[5] = {
	"888 88b,         888                      Y8b Y8b Y888P",
	"888 88P'  ,'Y88b 888 ee  dP'Y  e88 88e     Y8b Y8b Y8P   ,'Y88b 888,8,",
	"888 8K   '8' 888 888 P  C88b  d888 888b     Y8b Y8b Y   '8' 888 888 '",
	"888 88b, ,ee 888 888 b   Y88D Y888 888P      Y8b Y8b    ,ee 888 888",
	"888 88P' '88 888 888 8b d,dP   '88 88'        Y8P Y     '88 888 888",
};

// get char len
//int str_len(const char* str) {
	int size = 0;
	while(*str++) ++size;
	return size;
}

