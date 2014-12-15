#include <stdlib.h>
#include "GameLib.h"
#include "mylib.h"
#include "Lives.h"
#include "BG.h"
#include "Ship.h"

int SCREEN_WIDTH = 240;
int SCREEN_HEIGHT = 160;

void drawLives(int lives) {
	int i;
	for (i = 0; i < lives; i += 1)
	{
		int j;
		for (j = 0; j < 9; j += 1)
		{
			DMA[3].src = Lives + j*5;
			DMA[3].dst = videoBuffer + 
			OFFSET(2+j, SCREEN_WIDTH-5-2-((i*9)+2));
			DMA[3].cnt = DMA_ON | 5;
		}
	}
}

void clearLives(int lives) {
	int i;
	for (i = 0; i < 9; i += 1)
	{
		DMA[3].src = BG + lives*5;
		DMA[3].dst = videoBuffer 
		+ OFFSET(2+i, SCREEN_WIDTH-5-2-((lives*9)+2));
		DMA[3].cnt = DMA_ON | 5;
	}
}

void drawShip(SHIP ship)
{
	int i;
	for(i=0; i<ship.length; i++)
	{
		DMA[3].src = Ship + (i*ship.width);
		DMA[3].dst = videoBuffer + OFFSET(ship.row+i, ship.col);
		DMA[3].cnt = DMA_ON | ship.width;
	}

}

void clearShip(SHIP ship)
{
	int i;
	for(i=0; i<ship.length; i++)
	{
		DMA[3].src = BG+(ship.row+i)*SCREEN_WIDTH+ship.col;
		DMA[3].dst = videoBuffer+OFFSET(ship.row+i, ship.col);
		DMA[3].cnt = DMA_ON | ship.width;	
	}

}

LANDINGPAD createLandingPad(SHIP ship)
{
	LANDINGPAD p;
	p.length = 1;
	p.width = (rand()%ship.width)+10;
	p.row = rand()%(SCREEN_HEIGHT-(ship.length*2)) + ship.length*2;
	p.col = rand()%(SCREEN_WIDTH-p.width);
	p.color = GREEN;
	return p;
}

TERRAIN* generateTerrain(LANDINGPAD p)
{
	int NUMTERRAIN = SCREEN_WIDTH;
	TERRAIN terrain[NUMTERRAIN];
	TERRAIN *trnptr = terrain;
	int strtspce = 20;
	int trnspce = SCREEN_HEIGHT-strtspce;
	int i;
	// Randomize Terrain
	for (i = 0; i < NUMTERRAIN; i += 1)
	{
		trnptr = terrain + i;
		trnptr->width = 1;
		trnptr->col = i;
		int randHeight = (rand()%trnspce)+strtspce;
		trnptr->row = randHeight;
		if(trnptr->col >= p.col && trnptr->col <= p.col+p.width)
		{
			trnptr->row = p.row + p.length;
		}
		trnptr->length = SCREEN_HEIGHT-(trnptr->row);
		trnptr->color = LTGRAY;
	}
	// Smooth terrain
	for (i=p.col-1; i>-1; i-=1)
	{
		trnptr = terrain + i;
		if((trnptr->row>(trnptr+1)->row&&(trnptr+1)->row>strtspce*3)
		||((trnptr+1)->row>trnspce))
		{
			trnptr->row = (trnptr+1)->row - (rand()%3);
		}
		else
		{
			trnptr->row = (trnptr+1)->row + (rand()%3);
		}
		trnptr->length = SCREEN_HEIGHT-trnptr->row;
	}
	for (i=p.col+p.width; i<SCREEN_WIDTH; i+=1)
	{
		trnptr = terrain + i;
		if((trnptr->row>(trnptr-1)->row&&(trnptr-1)->row>strtspce*3)
		||((trnptr-1)->row>trnspce))
		{
			trnptr->row = (trnptr-1)->row - (rand()%3);
		}
		else
		{
			trnptr->row = (trnptr-1)->row + (rand()%3);
		}
		trnptr->length = SCREEN_HEIGHT-trnptr->row;
	}
	trnptr = terrain;
	return trnptr;
}

void drawTerrain(TERRAIN *trnptr) {
	int NUMTERRAIN = SCREEN_WIDTH;
	int i;
	for (i = 0; i < NUMTERRAIN-1; i += 1)
	{
		trnptr++;
		drawRect(trnptr->row, trnptr->col,
		trnptr->length,trnptr->width,trnptr->color);
	}
}
