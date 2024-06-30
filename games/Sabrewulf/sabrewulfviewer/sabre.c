#include<stdlib.h>
#include<stdio.h>
#include<allegro.h>
#include "global.h"
#include "map.h"

int scrollSpeedY[40]={
  1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,
  20,19,18,17,16,14,13,12,12,11,9,7,7,6,5,4,3,1,1,0
};

void my_timer_handler(){
  counter++;
}
END_OF_FUNCTION(my_timer_handler)

int init(void){
  
  allegro_init();
  
  LOCK_VARIABLE(counter);
  LOCK_FUNCTION(my_timer_handler);
  install_timer();
  install_int(my_timer_handler,60);

  set_color_depth(16);

  set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480,0,0);
  install_keyboard();
  
  loadGraphics();
  
  buffer=create_bitmap(512,384);
  clear(buffer);

  roomX=8;
  roomY=10;
  

  return EXIT_SUCCESS;
}

void updateGameLogic(){

  int dir=9;
  int prevX,prevY;
  if(key[KEY_UP]){
    prevY=roomY;
    roomY--;
    dir=UP;
    if(roomY < 0){
      roomY = 0;
    }
    if(prevY != roomY){
      changeRoom(map[prevY][roomX],map[roomY][roomX],UP);
    }
  }
  if(key[KEY_DOWN]){
    dir=DOWN;
    prevY=roomY;
    roomY++;
    if(roomY > MAPMAXY-1){
      roomY = MAPMAXY-1;
    }
    if(prevY != roomY){
      changeRoom(map[prevY][roomX],map[roomY][roomX],DOWN);
    }
  }
  if(key[KEY_LEFT]){
    dir=LEFT;
    prevX=roomX;
    roomX--;
    if(roomX < 0){
      roomX=0;
    }
    if(prevX != roomX){
      changeRoom(map[roomY][prevX],map[roomY][roomX],LEFT);
    }
  }
  if(key[KEY_RIGHT]){
    dir=RIGHT;
    prevX=roomX;
    roomX++;
    if(roomX > MAPMAXX-1){
      roomX=MAPMAXX-1;
    }
    if(prevX != roomX){
      changeRoom(map[roomY][prevX],map[roomY][roomX],RIGHT);
    }
  }

  currentRoom=map[roomY][roomX];
  actualCounter++;
}

int main(void){
  int i;
  init();
  while(!key[KEY_ESC]){

    while(actualCounter < counter){
      updateGameLogic();
    }
    actualCounter=counter=0;
    drawDisplay();
  
  }
  return EXIT_SUCCESS;
}
END_OF_MAIN();
