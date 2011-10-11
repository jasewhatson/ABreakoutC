#include "../headers/GameImages.h"

GameImages::GameImages(){} 


 SDL_Surface *GameImages::load_image(char* filename){    //The image that's loaded
    
    SDL_Surface* loadedImage = NULL;
      
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image using SDL_image
    loadedImage = IMG_Load(filename);
       
    //If the image loaded
    if( loadedImage != NULL ){
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //Free the old image
        SDL_FreeSurface( loadedImage );
    }
    
    //If the image was optimized just fine 
    if( optimizedImage != NULL ) { 
        //Map the color key 
        Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 00, 00, 00);
        //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent 
        SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey ); 
    }
    
    //Return the optimized image
    return optimizedImage;
}
