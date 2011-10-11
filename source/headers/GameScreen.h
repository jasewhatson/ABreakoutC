// 
// File:   GameScreen.h
// Author: jason
//
// Created on January 23, 2007, 8:22 PM
//
#include "SDL/SDL.h"

#ifndef _GameScreen_H
#define	_GameScreen_H

/*Represents the screen we draw to*/

class GameScreen{
    public:
        
        //Draws a SDL surface at the x, y location. Can also draw part of a 
        //image specified by a SDL_rect (x,y,h,w)
        void apply_surface( int x, int y, SDL_Surface* source, SDL_Rect *rec = NULL);
        
        //Sets up a screen for use to draw to
        bool init(int screen_width,int screen_height,int screen_bpp);
        void clean_up();
        
        //Returns refrence to the screen
        SDL_Surface *getScreen(){ return this->screen;}
        
        //Calculates Frames per second that we a drawing to the screen
        int CalculateFrameRate();
        bool debug;
        
    private:
        SDL_Surface *screen;
};


#endif	/* _GameScreen_H */
