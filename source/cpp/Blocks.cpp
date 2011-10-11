#include "../headers/Blocks.h"
#include "../headers/GameImages.h"

SDL_Surface* Block::BlockImageFull;  
SDL_Surface* Block::BlockImageMedium;
SDL_Surface* Block::BlockImageLow;
SDL_Surface* Block::BlockImageUnbreakable; 
int Block::BlockWidth;
int Block::BlockHeight;
int Block::BlocksLeft = 0;
void load();

Block::Block(int Extra, int NoHits, int BlockYLocation,int BlockXLocation){   

      this->NoHits = NoHits;
      this->BlockYLocation = BlockYLocation;
      this->BlockXLocation = BlockXLocation;  
      this->Extra = Extra;
}

void Block::load(SDL_Surface* BlockImageFull,SDL_Surface* BlockImageMedium,
                SDL_Surface* BlockImageLow,SDL_Surface* BlockImageUnbreakable,
                int BlockHeight, int BlockWidth){
     
     Block::BlockHeight = BlockHeight;
     Block::BlockWidth = BlockWidth;
                    
     Block::BlockImageFull = BlockImageFull;   
     Block::BlockImageMedium = BlockImageMedium;
     Block::BlockImageLow = BlockImageLow;
     Block::BlockImageUnbreakable =  BlockImageUnbreakable;
}

SDL_Surface *Block::getBlockImage(){
   switch(this->NoHits){
       case 5:
            return Block::BlockImageUnbreakable;
       case 3:
            return Block::BlockImageFull;
       case 2:
            return Block::BlockImageMedium;
       case 1: 
            return Block::BlockImageLow;                                   
   }
}

void Block::freeSDLResources(){
    SDL_FreeSurface(BlockImageFull);
    SDL_FreeSurface(BlockImageMedium);
    SDL_FreeSurface(BlockImageLow);
    SDL_FreeSurface(BlockImageUnbreakable);
}



