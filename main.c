#include "header.h"
int main()
{
	FPS = 10.0;
    initscr();
    noecho();
    curs_set(0);
    getmaxyx(stdscr, maxY, maxX);
    setTrail();
	setObj();
    gameOver = 0;
    input = 'o';
	input2 = 'o';
	setPlayer(1, 1, S, 1);
	setPlayer(maxX-2, maxY-2, 'i', 2);
	
    start_color();
	
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
    char play;
    printf("Hello. Press any key to play\n");
    scanf(" %c", &play);

    pthread_t p1Control, p2Control, make;
    pthread_create(&make, NULL, makeMap, NULL);
    pthread_create(&p1Control, NULL, inputP1, NULL);
    //pthread_create(&p2Control, NULL, inputP2, NULL);
    pthread_join(make, NULL); // Start make area thread
    pthread_join(p1Control, NULL); // Start control thread
    //pthread_join(p2Control, NULL); // Start control thread

    endwin();
   
    return 0;
}