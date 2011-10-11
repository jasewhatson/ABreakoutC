#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#ifndef _Item_H
#define	_Item_H

/*A powerup which adds a special effect to the game*/

class Powerup{
    public:
        
        /*Give the powerup a name, location, image, and callback method to turn the powerup on/off*/
        Powerup(char* name,int xpos,int ypos,SDL_Surface* itemImage,void (*powerUpEvent)(bool) = NULL);
        
        /*Check if the power has been hit. Namely by the paddle. Which means the player collected the item*/
        bool checkObjectHit(int object_x, int object_y, int object_length);
        
        /*Move the paddle down*/
        void drop();
        
        /*Returns true if a powerup has been collected*/
        bool checkIfPowerupActive();
        
        /*Starts timming how long the powerup has been on*/
        void startTimer();
        
        int xpos,ypos;
        
        SDL_Surface* sprite;
        Uint32 startTime; 
        Uint32 length;
        
        bool visable;
        bool itemRecived;
        bool puOnSent;
        
        void (*powerUpEvent)(bool);
        
    private:    
        char* name;
        bool active;
        
        bool playerHasItem;
        
};

#endif	/* _Item_H */

