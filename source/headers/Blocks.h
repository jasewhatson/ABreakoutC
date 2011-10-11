#ifndef _BLOCK_H
#define _BLOCK_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

/*Basically holds block infomation such as location, size, no hits left and 
and extra special powerups the block may hold*/
class Block{
      public:
             Block(int Extra, int NoHits,int BlockYLocation,int BlockXLocation);
             ~Block();
             
             static void freeSDLResources();
             
             static SDL_Surface* BlockImageFull;
             static SDL_Surface* BlockImageMedium;
             static SDL_Surface* BlockImageLow;
             static SDL_Surface* BlockImageUnbreakable;
             
             int NoHits;
             int Extra;
             
             static int BlockHeight;
             static int BlockWidth;
             
             static int BlocksLeft; //The number of blocks left in the game
             
             int BlockYLocation;
             int BlockXLocation;
             
             static void load(SDL_Surface* BlockImageFull,SDL_Surface* BlockImageMedium,
                SDL_Surface* BlockImageLow,SDL_Surface* BlockImageUnbreakable ,
                int BlockHeight, int BlockWidth);
             
             SDL_Surface* getBlockImage();

};

#endif
