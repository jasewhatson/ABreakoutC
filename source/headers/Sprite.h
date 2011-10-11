// 
// File:   Sprite.h
// Author: jason
//
// Created on February 7, 2007, 1:47 AM
//

#ifndef _Sprite_H
#define	_Sprite_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <vector>

/*Sprite - A simple frame by frame animation*/

class Sprite{
    public:
        Sprite();//Does nothing ATM
        
        /*A sprite sheet image, lenght of sprite sheet lenght and height,
          the number of frames in the sheet and the lenght of the entire animation*/
        Sprite(SDL_Surface* spritesheet, int spritelength ,int spriteheight, 
        int noframes,long timelength);
                
        void addFrame(SDL_Rect, int frameno); //No implementation yet
        
        /*Begins the animation*/
        void startAnimate(int loopamount = 1); 
        
        /*Animates the frame. Needs to be constantly called by game loop to 
          check if we need to move to the next frame*/
        void animate(int noAnimationLoops = -1);
        
        /*Sets the animation to the first frame*/
        void restart();
        
        /*Self explanatory*/
        void pause();
        void resume();
        
        /*Gets the current frame or pass a frame number to ge a specific frame*/
        SDL_Rect *getFrame(int frame = -1);
        
        SDL_Surface* spritesheet;
        
        std::vector<SDL_Rect> animation; 
        
        int noFrames;
        int currentFrame;
        int status;
        
        long frameLength;
        
        Uint32 timeAnimationStarted;
        Uint32 pausedTime;
        Uint32 timeAtStartOfPause;
        bool paused;       
        
        //Defines the states the sprite can be in*/
        static const int PAUSED = 1;
        static const int ANIMATING = 2;
        static const int FINISHED = 3;
        static const int IDEL = 4;
        
        
};

#endif	/* _Sprite_H */

