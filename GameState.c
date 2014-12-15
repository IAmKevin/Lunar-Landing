#include "Title.h"
#include "BG.h"
#include "GameOver.h"
#include "Win.h"
#include "GameLib.h"
#include "mylib.h"
#include "text.h"
#include <stdlib.h>
#include <stdio.h>

void title(int *seed) {
	(*seed)++;
	DMA[3].src = Title;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_ON | 38400;

	while(!KEY_DOWN_NOW(BUTTON_START))(*seed)++;
	while(KEY_DOWN_NOW(BUTTON_START))(*seed)++;
}

int win(int *seed) {
	(*seed)++;
	int i;
	for (i = 0; i < 20; i += 1)
	{
		DMA[3].src = Win + i*200;
		DMA[3].dst = videoBuffer + OFFSET(10+i, 20);
		DMA[3].cnt = DMA_ON | 200;
	}

	while(!(KEY_DOWN_NOW(BUTTON_SELECT)||
	KEY_DOWN_NOW(BUTTON_START)))(*seed)++;
	if(KEY_DOWN_NOW(BUTTON_SELECT)){
	while(KEY_DOWN_NOW(BUTTON_SELECT))(*seed)++;return TITLE;}
	while(KEY_DOWN_NOW(BUTTON_START))(*seed)++;return GAME;
}

void lose(int *seed) {
	DMA[3].src = GameOver;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_ON | 38400;
	while(!KEY_DOWN_NOW(BUTTON_SELECT))(*seed)++;
	while(KEY_DOWN_NOW(BUTTON_SELECT))(*seed)++;
}

int game(SHIP ship, int seed) {
	srand(seed);
	int gamestate = DRAWINIT;
	int screenWidth = 240;
	int screenHeight = 160;
	int lives = 5;
	int timer = 0;
	int gravity = 1;
	int playing = 1;

	// Create old ship
	SHIP shipbase = ship;
	SHIP oldship = ship;

	// Create landing pad
	LANDINGPAD p = createLandingPad(ship);

	// Create terrain
	int NUMTERRAIN = screenWidth;
	TERRAIN *terrain = generateTerrain(p);

	int i;
	// Game Loop
	while(1){
	switch(gamestate) {
	case PAUSE:
		while(!(KEY_DOWN_NOW(BUTTON_LEFT) || 
		KEY_DOWN_NOW(BUTTON_RIGHT) || 
		KEY_DOWN_NOW(BUTTON_UP)||
		KEY_DOWN_NOW(BUTTON_SELECT)));

		if(KEY_DOWN_NOW(BUTTON_LEFT) || 
		KEY_DOWN_NOW(BUTTON_RIGHT) || 
		KEY_DOWN_NOW(BUTTON_UP));
		{
			gamestate = DRAWINIT;
			
		}
		// Return to title when select is pressed
		if(KEY_DOWN_NOW(BUTTON_SELECT))
		{
			return TITLE;
		}
		playing = 1;
		gamestate = START;
		break;
	case DRAWINIT:
		// Set initial background
		DMA[3].src = BG;
		DMA[3].dst = videoBuffer;
		DMA[3].cnt = DMA_ON | 38400;

		// Draw the ship
		drawShip(ship);

		// Draw Lives
		drawLives(lives);

		// Draw the landing pad
		drawRect(p.row, p.col, p.length, p.width, p.color);

		// Draw the terrain
		drawTerrain(terrain);

		gamestate = PAUSE;
		break;
	case START:

	while(playing) {
/////////////////// BUTTON CONTROLS //////////////////

		// Return to title when select is pressed
		if(KEY_DOWN_NOW(BUTTON_SELECT))
		{
			return TITLE;
		}
		// Left movement
		if(KEY_DOWN_NOW(BUTTON_LEFT) && ship.fuel > 0)
		{
			if(ship.col > 0)
			{
				ship.col-=ship.cdel;
				ship.fuel--;
			}
		}
		// Right movement
		if(KEY_DOWN_NOW(BUTTON_RIGHT) && ship.fuel > 0)
		{
			if(ship.col+ship.width < screenWidth)
			{
				ship.col+=ship.cdel;
				ship.fuel--;
			}
		}
		// Account for gravity and thrust
		if(KEY_DOWN_NOW(BUTTON_UP) && ship.fuel > 0)
		{
			if(timer > 5)
			{
				timer = 0;
				ship.rdel-=ship.thrust;
			}
			else
			{
				timer++;
			}
			ship.row+=ship.rdel;
			ship.fuel--;
		}
		else
		{
			ship.row+=ship.rdel;
			if(timer > 5)
			{
				timer = 0;
				ship.rdel+=gravity;
			}
			else
			{
				timer++;
			}
		}
		
		waitForVblank();

///////////////////////// Collision Check //////////////////////////

		// Check pad landing
		if((ship.row+ship.length >= p.row) && 
		(ship.row+ship.length/2 < p.row+p.length) &&
		(ship.col+ship.width/2 > p.col) && 
		(ship.col-ship.width/2 < p.col + p.width))
		{
			ship.row = p.row - ship.length;
			clearShip(oldship);
			drawShip(ship);
			return WIN;
		}
		// Check ground collision
		if((ship.row+ship.length >= screenHeight) && lives > 0)
		{
			clearShip(oldship);
			ship = shipbase;
			lives--;
			clearLives(lives);
			oldship = ship;
			gamestate = PAUSE;
			playing = 0;
		}

		// Check terrain collision
		for (i = 0; i < NUMTERRAIN; i += 1)
		{
			if((ship.row+ship.length >= terrain[i].row) && 				(ship.col+ship.width > terrain[i].col) && 
			(ship.col < terrain[i].col + terrain[i].width) && 				(ship.row <= terrain[i].row+terrain[i].length))
			{
				clearShip(oldship);
				ship = shipbase;
				lives--;
				clearLives(lives);
				oldship = ship;
				gamestate = PAUSE;
				playing = 0;
			}
		}

		// Draw if more lives after collision
		if(lives > 0) 
		{
			// Draw lives
			drawLives(lives);

			// Draw the ship
			clearShip(oldship);
			drawShip(ship);
		}

		// Check lives
		if(lives <= 0)
		{
			clearShip(oldship);
			drawShip(ship);
			return LOSE;
		}

		oldship = ship;
	}
		break;
	}
	}
	return TITLE;
}
