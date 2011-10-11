#ifndef _Ball_H
#define	_Ball_H

#include "SDL/SDL.h"

/*A ball object. It bounce off blocks, walls and the paddle*/
class Ball{
    public:
        
        /*Constructor - Ball image/graphic, start speed of ball, ball dimension, 
          start pos of ball*/ 
        Ball(SDL_Surface* BallImage, int startxSpeed, int startySpeed, 
            int ballDimension, int xScreenLocation, int yScreenLocation);
        ~Ball();
        
        int xSpeed,ySpeed,xScreenLocation,yScreenLocation;
        SDL_Surface* BallImage;
        
        //Moves the ball - y pos of players paddle*/
        int MoveBall(int paddle_y); 
        
        /*lets the ball know how big our game area is*/
        void setScreenDims(int width, int height);
        
        /*Checks to see if the ball has hit with a object. Spefified as x,y,w,h*/
        void checkObjectHit(int object_x, int object_y, int object_length);
        
        int ballDimension;  
    private:
        int width;
        int height;
                 
};

#endif	/* _Ball_H */
