#include "mylib.h"
#include "GameLib.h"

int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	int seed = 0;

	// Create the shipbase
	SHIP shipbase;
	int SHIPSTARTROW = 0;
	int SHIPSTARTCOL = 120;
	int MAXFUEL = 300;
	shipbase.row = SHIPSTARTROW;
	shipbase.col = SHIPSTARTCOL;
	shipbase.width = 11;
	shipbase.length = 20;
	shipbase.rdel = 1;
	shipbase.thrust = 1;
	shipbase.cdel = 1;
	shipbase.fuel = MAXFUEL;
	shipbase.color = WHITE;

	int state = TITLE;
	while(1)
	{
		switch(state) {
		case TITLE: 
			title(&seed);
			state = GAME;
			break;
		case GAME: 
			state = game(shipbase,seed);
			break;
		case WIN:
			state = win(&seed);
			break;
		case LOSE: lose(&seed);
			state = TITLE;
			break;
		}
	}
	return 0;
}


