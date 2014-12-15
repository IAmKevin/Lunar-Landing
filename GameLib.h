enum {TITLE, GAME, WIN, LOSE, PAUSE, DRAWINIT, START};

typedef struct
{
	int row;
	int col;
	int rdel;
	int cdel;
	int length;
	int width;
	int thrust;
	int fuel;
	unsigned short color;

} SHIP;

typedef struct
{
	int row;
	int col;
	int length;
	int width;
	unsigned short color;

} LANDINGPAD;

typedef struct
{
	int row;
	int col;
	int length;
	int width;
	unsigned short color;

} TERRAIN;

// Prototypes
void drawLives(int lives);
void clearLives(int lives);
void drawShip(SHIP ship);
void clearShip(SHIP ship);
LANDINGPAD createLandingPad(SHIP ship);
TERRAIN* generateTerrain(LANDINGPAD p);
void drawTerrain(TERRAIN *trnptr);
int game(SHIP ship, int seed);
void title(int *seed);
int win(int *seed);
void lose(int *seed);

