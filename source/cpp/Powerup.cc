#include "../headers/Powerup.h"

Powerup::Powerup(char* name,int xpos,int ypos,SDL_Surface* itemImage,void (*powerUpEvent)(bool)) : itemRecived(false), length(30000), visable(true), startTime(0){
    this->name = name;
    this->xpos = xpos;
    this->ypos = ypos;
    this->sprite = itemImage;
    this->length = 30000; //Defaul value
    this->powerUpEvent = powerUpEvent;
    this->puOnSent = false;
}

void Powerup::drop(){
    this->ypos += 5; //needs to be a constant called drop speed
}


bool Powerup::checkObjectHit(int object_x, int object_y, int object_length){
    
    if (this->ypos + 15 >= object_y  && this->ypos <= object_y + 5 &&
    this->xpos + 15 >= object_x &&
    this->xpos <= object_x + object_length){
        return true;        
    }
    
    return false;
}

void Powerup::startTimer(){
    this->startTime = SDL_GetTicks();
}

bool Powerup::checkIfPowerupActive(){
    
    //The powerup has been recived
    if(itemRecived == false){
        return true;
    }
    
    //If the length of time the powerup is active for is over. Turn off the powerup
    if (this->startTime + this->length >= SDL_GetTicks()){
        if (puOnSent == false){
            this->powerUpEvent(true); //Call the callback method
            puOnSent = true;
            printf("%d",this->length);
        }
        return true;
    }else{
        this->powerUpEvent(false); //Call the callback method
        return false;
    }
}