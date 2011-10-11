#include "../headers/Sprite.h"
#include <iostream>

Sprite::Sprite(){
    
}

Sprite::Sprite(SDL_Surface* spritesheet, int spritelength , int spriteheight,
int noframes, long timelength) : spritesheet(spritesheet){
    
    //Sprite atributes : Some are already assigned from above. Kinna pointless. Who knows?
    this->noFrames = noframes;
    this->currentFrame = 0;
    this->frameLength = timelength / noframes;
    this->timeAnimationStarted = 0;
    this->pausedTime = 0;
    this->spritesheet = spritesheet;
    paused = false;
    
    this->status = this->IDEL;//Set the sprites status to idel.
    
    SDL_Rect rr;
    
    int x = 0;
    
    //Loop through all the frames and make rects out of them. Add these rects to a vector.
    for (int i = 0; i < noframes; i++) {
        
        rr.x = x;
        rr.y = 0;
        
        rr.w = spritelength;
        rr.h = spriteheight;
        
        this->animation.push_back(rr);
        
        x+= spritelength;      
    }
}
               
void Sprite::animate(int noAnimationLoops){
    //TODO: Implement/get rid of noAnimation loop
    
    //it might be nice to do something with threading or call backs or interupts 
    //instead of this constantly being called.
    
    /*Basically if the sprite isnt paused we check to see if the ammount of time
      each frame goes for has passed since last frame change. 
      If so we move to the next frame. If its a looping frame we set its frame 
      back to the first frame if the current frame is at the end. If its not a 
      looping sprite we set its status to finish.*/
    if(!paused){
        if (this->timeAnimationStarted == 0){
            this->timeAnimationStarted = SDL_GetTicks();
        }
        
        if (timeAnimationStarted + this->frameLength <= SDL_GetTicks()){
            
            timeAnimationStarted += this->frameLength;
            
            
            if (this->currentFrame >= this->noFrames - 1){
                if (noAnimationLoops == -1){
                    this->status = this->FINISHED;
                    return;
                }
                currentFrame = 0;
            }
            
            this->currentFrame++;
        }
    }
}

//resets animation to the starting frame
void Sprite::restart(){
    currentFrame = 0;
    this->status = this->IDEL;
}

//Pauses the animation. Sets the time of pause and sprite status to paused
void Sprite::pause(){
    paused = true;
    timeAtStartOfPause = SDL_GetTicks();
    this->status = this->PAUSED;
}

//Resumes the animation. Adjusts our time so it seems like the animation hasnt changes since the time we paused
void Sprite::resume(){
    paused = false;
    timeAnimationStarted += SDL_GetTicks() - timeAtStartOfPause;
    this->status = this->ANIMATING;
}

//Returns a frame or gets the current frame in the animation
SDL_Rect *Sprite::getFrame(int frame){
    
    if (frame != -1){
        return &this->animation[frame];
    }else{
        return &this->animation[this->currentFrame];
    }
}