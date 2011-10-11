#include "../headers/Paddle.h"
#include "../headers/GameImages.h"

Paddle::Paddle(SDL_Surface* PaddleImage, int width,
              int xScreenLocation, int yScreenLocation){

      this->PaddleImage = PaddleImage;
      this->paddleWidth = width;
      this->xScreenLocation = xScreenLocation;
      this->yScreenLocation = yScreenLocation;
}

void Paddle::movePaddle(bool left,int amount){
        //If left = false. Asume right
        if(left == true){
                if (this->xScreenLocation - amount > 0){ 
                   this->xScreenLocation -= amount;
                }else{
                  this->xScreenLocation = 0;    
                }      
        }else{
              if (this->xScreenLocation + amount + this->paddleWidth < this->width){
                 this->xScreenLocation += amount;
              }else{
                 this->xScreenLocation = this->width - this->paddleWidth;
              }
        }              
} 

void Paddle::setScreenDims(int width, int height){
     this->width = width;
     this->height = height;
}
                    
                     


                   
