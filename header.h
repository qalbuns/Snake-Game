#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <time.h>
#include <errno.h>
#include <ncurses.h>



#define W 119
#define A 97
#define S 115
#define D 100
#define ESC 27

char input;
char input2;
int maxX, maxY;
char character;
int gameOver;
float FPS;
typedef struct{
	int coorX, coorY;
	char dir;
}player;

typedef struct{
	int coorX, coorY;
	int color;
}jejak;

player p1,p2;
jejak trail[1000 * 1000] ,trailObj[5];
int counterObj;

int jumTrail;

// Create getch func
static struct termios old, current;
void initTermios(int echo) ;
void resetTermios(void);
char getch_(int echo);
char getchNew(void);
char getcheNew(void);
// ===================
void setPlayer(int x, int y, char dir, int playercode);
void setTrail();
void setObj();
void printObj(int n);
void UpdateDir(int playercode);
void UpdateDir2(int playercode);
void UpdateCoor();
void *inputP1(void *vargp);
//void *inputP2(void *vargp);
void *makeMap(void *vargp);
int checkCoord(int x, int y);

int msleep(long msec); // Milisecond Sleep
void fps(float frame); // frame per second


