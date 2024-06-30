#include <allegro.h>
#define YES 0
#define NO  1
#define MAXSCENERY  96
#define MAXELEMENTS 96
#define MAXROOMS    48
#define MAPMAXX     16
#define MAPMAXY     16
#define UP          0
#define DOWN        1
#define LEFT        2
#define RIGHT       3


BITMAP *scenery[MAXSCENERY];
PALETTE pal;
BITMAP *buffer;

extern FONT *font;
int roomX, roomY;
int currentRoom;

volatile int counter;
int actualCounter;
