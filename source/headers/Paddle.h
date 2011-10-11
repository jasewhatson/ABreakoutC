#ifndef _Paddle_H
#define	_Paddle_H

#include "SDL/SDL.h"

/*A paddle object which the player can move to interact with the ball'(s)*/
class Paddle{
    public:
        
        Paddle(SDL_Surface* PaddleImage, int width,
        int xScreenLocation, int yScreenLocation);
        int xSpeed,ySpeed,xScreenLocation,yScreenLocation;
        SDL_Surface* PaddleImage;
        /*Moves a paddle the given amount of pixcels in a given direction*/
        void movePaddle(bool left,int amount);   
        /*We are mainly interested the width her so we dont move the paddle out of the screen*/
        void setScreenDims(int width, int height);
        int paddleWidth;    
    private:
        int width;
        int height; 
            
};

#endif	
