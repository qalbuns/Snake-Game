#include "header.h"
/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getchNew(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getcheNew(void) 
{
  return getch_(1);
}
// =================
void fps(float frame)
{
    int f = 1000/frame;
    msleep(f);
}

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void setPlayer(int x, int y, char dir, int playercode){
	if(playercode == 1)
	{
		p1.coorX = x;
		p1.coorY = y;
		p1.dir = dir;
	}
	else if(playercode == 2)
	{
		p2.coorX = x;
		p2.coorY = y;
		p2.dir = dir;
	}
}

void setTrail(){
	jumTrail = 0;
}

void setObj(){
	int i,j;
	
	//mvprintw(maxY/2, maxX/2, "Game Over.");
	for(i=0;i<5;i++){
		trailObj[i].coorY = (maxY/2)-i+5;
		trailObj[i].coorX = maxX/2;
	}
	counterObj = 0;
}

void printObj(int n){
	int i,j;
	n = n%5;
	//0-4 
	if(n == 4) counterObj ++;
	
	//mvprintw(maxY/2, maxX/2, "Game Over.");
	for(i=0;i<5;i++){
		mvprintw(trailObj[i].coorY, trailObj[i].coorX, " ");
	}
	
	for(i=0;i<5;i++){
		if(counterObj % 4 == 0 || counterObj % 4 == 1){
			trailObj[i].coorY --;
			//trailObj[i].coorX --;
		}else{
			trailObj[i].coorY ++;
			//trailObj[i].coorX ++;
		}
		
	}
	
	for(i=0;i<5;i++){
		mvprintw(trailObj[i].coorY, trailObj[i].coorX, "6");
	}
}

void UpdateDir(int playercode)
{

	if((input == W) && (p1.dir != S )) p1.dir = W ;
	else if((input == S) && (p1.dir != W )) p1.dir = S ;
	else if((input == A) && (p1.dir != D )) p1.dir = A ;
	else if((input == D) && (p1.dir != A )) p1.dir = D ;

	if((input == 'i') && (p2.dir != 'k' )) p2.dir = 'i' ;
	else if((input == 'k') && (p2.dir != 'i' )) p2.dir = 'k' ;
	else if((input == 'j') && (p2.dir != 'l' )) p2.dir = 'j' ;
	else if((input == 'l') && (p2.dir != 'j' )) p2.dir = 'l' ;
}


void UpdateCoor()
{
	trail[jumTrail].coorY = p1.coorY;
	trail[jumTrail].coorX = p1.coorX;
	trail[jumTrail].color = 1;
	
	// printf("Y: %d X: %d", trail[jumTrail].coorY, trail[jumTrail].coorX);
	
	
	// Cek benturan
	if(p1.dir == W) p1.coorY--;
	else if(p1.dir == A) p1.coorX--;
	else if(p1.dir == S) p1.coorY++;
	else if(p1.dir == D) p1.coorX++;
	
	jumTrail++;
	
	trail[jumTrail].coorY = p2.coorY;
	trail[jumTrail].coorX = p2.coorX;
	trail[jumTrail].color = 2;

	if(p2.dir == 'i') p2.coorY--;
	else if(p2.dir == 'j') p2.coorX--;
	else if(p2.dir == 'k') p2.coorY++;
	else if(p2.dir == 'l') p2.coorX++;
	
	
	jumTrail++;
	
	for(int i=0; i<5 && gameOver == 0;i++){
		if(p1.coorY == trailObj[i].coorY && p1.coorX == trailObj[i].coorX ) gameOver = 1;
		if(p2.coorY == trailObj[i].coorY && p2.coorX == trailObj[i].coorX ) gameOver = 1;
	}
	
	for(int i=0;i<jumTrail && gameOver == 0;i++)
	{	
		
		if((p1.coorY == trail[i].coorY && p1.coorX == trail[i].coorX )||
		(p1.coorY == 1 || p1.coorY == maxY-1 || p1.coorX == 0 || p1.coorX == maxX-1))
		{
			gameOver = 1;
		}
		if(p2.coorY == trail[i].coorY && p2.coorX == trail[i].coorX||
		(p2.coorY == 1 || p2.coorY == maxY-1 || p2.coorX == 0 || p2.coorX == maxX-1))
		{
			gameOver = 1;
		}
	}

	
}

void *inputP1(void *vargp)
{
    char c;
    while(1)
    {
        input = getchNew();
		UpdateDir(1);
        // fflush(stdout);
        if(input == ESC) break;
    }
	fflush(stdout);
    pthread_exit(NULL);
}


int checkCoord(int x, int y)
{
	int i;
	int temp = -1;
	for(i=0;i<jumTrail;i++){
		if(trail[i].coorX == x && trail[i].coorY == y)
		{
			temp = trail[i].color;
			break;
		}		
	}
	return temp;
}

void *makeMap(void *vargp)
{
	int score = 0,i, j,k;
	
	
	//RED(0);
	//GREEN(0);
	
	for(i=0;i<maxX;i++){
		for(j=0;j<maxY;j++) 
		{
			if(j == 0 ||j == maxY-1 || i == 0 || i == maxX-1)
			{
				mvprintw(j, i, "*");
			}
			
		}
	
	}
	
	
	
	attron(COLOR_PAIR(1));
	mvprintw(p1.coorY, p1.coorX, "%c", 'O');
	attron(COLOR_PAIR(2));
	mvprintw(p2.coorY, p2.coorX, "X");
	
	
	refresh();
	while(gameOver == 0)
	{
		UpdateCoor();
		mvprintw(0, 0, "Score: %d  (%0.2f) ", score, FPS);
		score++;
		
		if(gameOver == 1)
		{
			for(i=0;i<5;i++){
				mvprintw(trailObj[i].coorY, trailObj[i].coorX, " ");
			}
			clear();
			mvprintw(maxY/2, maxX/2 - 5, "Game Over.");
			mvprintw(maxY/2 + 1, maxX/2 - 5, "Score: %d", score);
			gameOver = 1;
		}
		attron(COLOR_PAIR(1));
		mvprintw(p1.coorY, p1.coorX, "O");
		attron(COLOR_PAIR(2));
		mvprintw(p2.coorY, p2.coorX, "X");
		attron(COLOR_PAIR(3));
		if(gameOver != 1) printObj(score);
		refresh();
		fps(FPS);
		FPS += 0.05;
		fflush(stdout);
	}
	pthread_exit(NULL);
}