#include<stdlib.h>
#include<stdio.h>
#include<allegro.h>
#include "global.h"

void changeRoom(int from, int to, int dir){
  int i=0;
  BITMAP *tmp;
  int done=NO;
  int scrollcount;
  extern rooms[MAXROOMS][MAXELEMENTS];
  extern scrollSpeedY[40];

  switch(dir){

  case UP:
    // Create tall double screen, draw old room at bottom, new room at top.
    // Scroll from bottom to top
    tmp = create_bitmap(512,384*2);
    clear(tmp);
    // Draw old room at bottom
    while(done==NO){
      if(rooms[from][i]==999){
	done=YES;
      }
      else{
	blit(scenery[rooms[from][i]],tmp,0,0,
	     (rooms[from][i+2])*2,
	     384+(rooms[from][i+1])*2,256,256);
      i+=3;
      }
    }
    // Draw new room at the top (leaving room for status area)
    i=0;
    done=NO;
    while(done==NO){
      if(rooms[to][i]==999){
	done=YES;
      }
      else{
	blit(scenery[rooms[to][i]],tmp,0,0,
	     (rooms[to][i+2])*2,
	     (rooms[to][i+1])*2,256,256);
      i+=3;
      }
    }
    
    int dx=384;
    for(i=0;i<40;i++){
      vsync();
      dx-=scrollSpeedY[i];
      blit(tmp,screen,0,dx,64,48,512,384);
    }
    
    break;


  case DOWN:
    // Create tall double screen, draw old room at top, new room at bottom.
    // Scroll from top to bottom
    tmp = create_bitmap(512,384*2);
    clear(tmp);
    // Draw old room at top
    while(done==NO){
      if(rooms[from][i]==999){
	done=YES;
      }
      else{
	blit(scenery[rooms[from][i]],tmp,0,0,
	     (rooms[from][i+2])*2,
	     (rooms[from][i+1])*2,256,256);
      i+=3;
      }
    }
    // Draw new room at the bottom (leaving room for status area)
    i=0;
    done=NO;
    while(done==NO){
      if(rooms[to][i]==999){
	done=YES;
      }
      else{
	blit(scenery[rooms[to][i]],tmp,0,0,
	     (rooms[to][i+2])*2,
	     384+(rooms[to][i+1])*2,256,256);
      i+=3;
      }
    }
    
    dx=0;
    for(i=0;i<40;i++){
      vsync();
      dx+=scrollSpeedY[i];
      blit(tmp,screen,0,dx,64,48,512,384);
    }
    break;


  case LEFT:
    // Create wide double screen, draw old room at right, new room at left.
    // Scroll from bottom to top
    tmp = create_bitmap(1024,384);
    clear(tmp);
    // Draw old room at right
    while(done==NO){
      if(rooms[from][i]==999){
	done=YES;
      }
      else{
	blit(scenery[rooms[from][i]],tmp,0,0,
	     512+(rooms[from][i+2])*2,
	     (rooms[from][i+1])*2,256,256);
      i+=3;
      }
    }
    // Draw new room at the left (leaving room for status area)
    i=0;
    done=NO;
    while(done==NO){
      if(rooms[to][i]==999){
	done=YES;
      }
      else{
	blit(scenery[rooms[to][i]],tmp,0,0,
	     (rooms[to][i+2])*2,
	     (rooms[to][i+1])*2,256,256);
      i+=3;
      }
    }
    
    dx=512;
    for(i=0;i<40;i++){
      vsync();
      dx-=((scrollSpeedY[i])*1.28);
      blit(tmp,screen,dx,0,64,48,512,384);
    }
    break;


  case RIGHT:
    // Create wide double screen, draw old room at left, new room at right.
    // Scroll from left to rigt
    tmp = create_bitmap(1024,384);
    clear(tmp);
    // Draw old room at bottom
    while(done==NO){
      if(rooms[from][i]==999){
	done=YES;
      }
      else{
	blit(scenery[rooms[from][i]],tmp,0,0,
	     (rooms[from][i+2])*2,
	     (rooms[from][i+1])*2,256,256);
      i+=3;
      }
    }
    // Draw new room at the top (leaving room for status area)
    i=0;
    done=NO;
    while(done==NO){
      if(rooms[to][i]==999){
	done=YES;
      }
      else{
	blit(scenery[rooms[to][i]],tmp,0,0,
	     512+(rooms[to][i+2])*2,
	     (rooms[to][i+1])*2,256,256);
      i+=3;
      }
    }
    
    dx=1;
    for(i=0;i<40;i++){
      vsync();
      dx+=((scrollSpeedY[i])*1.37);
      blit(tmp,screen,dx,0,64,48,512,384);
    }
    break;

  }
  
  destroy_bitmap(tmp);

}


void loadGraphics(void){

    scenery[0]=load_bitmap("gfx/70bc.bmp",NULL);
    scenery[1]=load_bitmap("gfx/71b3.bmp",NULL);   
    scenery[2]=load_bitmap("gfx/7298.bmp",NULL);
    scenery[3]=load_bitmap("gfx/72f6.bmp",NULL);
    scenery[4]=load_bitmap("gfx/7462.bmp",NULL);
    scenery[5]=load_bitmap("gfx/7523.bmp",NULL);
    scenery[6]=load_bitmap("gfx/771f.bmp",NULL);
    scenery[7]=load_bitmap("gfx/785e.bmp",NULL);
    scenery[8]=load_bitmap("gfx/78f2.bmp",NULL);
    scenery[9]=load_bitmap("gfx/7947.bmp",NULL);
    scenery[10]=load_bitmap("gfx/7981.bmp",NULL);
    scenery[11]=load_bitmap("gfx/7b11.bmp",NULL);
    scenery[12]=load_bitmap("gfx/7bb7.bmp",NULL);
    scenery[13]=load_bitmap("gfx/7c0c.bmp",NULL);
    scenery[14]=load_bitmap("gfx/7ccd.bmp",NULL);
    scenery[15]=load_bitmap("gfx/7e4b.bmp",NULL);
    scenery[16]=load_bitmap("gfx/8047.bmp",NULL);
    scenery[17]=load_bitmap("gfx/81c5.bmp",NULL);
    scenery[18]=load_bitmap("gfx/8382.bmp",NULL);
    scenery[19]=load_bitmap("gfx/83aa.bmp",NULL);
    scenery[20]=load_bitmap("gfx/83d2.bmp",NULL);
    scenery[21]=load_bitmap("gfx/8427.bmp",NULL);
    scenery[22]=load_bitmap("gfx/847c.bmp",NULL);
    scenery[23]=load_bitmap("gfx/8558.bmp",NULL);
    scenery[24]=load_bitmap("gfx/85c8.bmp",NULL);
    scenery[25]=load_bitmap("gfx/86da.bmp",NULL);
    scenery[26]=load_bitmap("gfx/8702.bmp",NULL);
    scenery[27]=load_bitmap("gfx/872a.bmp",NULL);
    scenery[28]=load_bitmap("gfx/8806.bmp",NULL);
    scenery[29]=load_bitmap("gfx/89c3.bmp",NULL);
    scenery[30]=load_bitmap("gfx/8b80.bmp",NULL);
    scenery[31]=load_bitmap("gfx/8c5c.bmp",NULL);
    scenery[32]=load_bitmap("gfx/8ccc.bmp",NULL);
    scenery[33]=load_bitmap("gfx/8d3c.bmp",NULL);
    scenery[34]=load_bitmap("gfx/8e18.bmp",NULL);
    scenery[35]=load_bitmap("gfx/8f2a.bmp",NULL);
    scenery[36]=load_bitmap("gfx/90a8.bmp",NULL);
    scenery[37]=load_bitmap("gfx/93c4.bmp",NULL);
    scenery[38]=load_bitmap("gfx/955d.bmp",NULL);
    scenery[39]=load_bitmap("gfx/95cd.bmp",NULL);
    scenery[40]=load_bitmap("gfx/9673.bmp",NULL);
    
    
    
    
}   

void drawRoom(int roomNo){
  
  int i=0;
  int done=NO;

  extern rooms[MAXROOMS][MAXELEMENTS];
  clear(buffer);
  while(done==NO){

    if(rooms[roomNo][i]==999){
      done=YES;
    }
    else{
     
      //draw_sprite(buffer,
      //	  scenery[rooms[roomNo][i]],
      //	  (rooms[roomNo][i+2])*2,
      //	  (rooms[roomNo][i+1])*2);
      blit(scenery[rooms[roomNo][i]],buffer,0,0,
	   (rooms[roomNo][i+2])*2,
	   (rooms[roomNo][i+1])*2,256,256);
      i+=3;
    }
  }
}    

void drawDisplay(void){
  

  
  drawRoom(currentRoom);
  vsync();
  blit(buffer,screen,0,0,64,48,512,384);
  
}     
