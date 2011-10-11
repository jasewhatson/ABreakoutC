// 
// File:   GameImages.h
// Author: jason
//
// Created on January 23, 2007, 5:55 AM
//

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#ifndef _GameImages_H
#define	_GameImages_H

class GameImages{
    public:
        GameImages();
        //Takes a file name of a image. Returns the SDL surface of the image.
        static SDL_Surface *load_image(char* filename);  
};


#endif	/* _GameImages_H */

