#include "../headers/Ball.h"

Ball::Ball(SDL_Surface* BallImage, int startxSpeed, int startySpeed, 
          int ballDimension, int xScreenLocation, int yScreenLocation){
          
   this->BallImage = BallImage;
   this->xSpeed = startxSpeed;
   this->ySpeed = startySpeed;
   this->ballDimension = ballDimension;
   this->xScreenLocation = xScreenLocation; 
   this->yScreenLocation = yScreenLocation;
   
}

//Each time this is called it moves the ball acording to current speed.
//If a ball hits the bottom of the screen 1 is returned.
int Ball::MoveBall(int paddle_y){
    
    //Moves the ball
    this->xScreenLocation += this->xSpeed;
    this->yScreenLocation += this->ySpeed;

    if (this->xScreenLocation <= 0){
        this->xScreenLocation = 0;
        this->xSpeed = -this->xSpeed;
        return 0;
    }
    
    if (this->xScreenLocation >= this->width - this->ballDimension){
        this->xScreenLocation = this->width - this->ballDimension;
        this->xSpeed = -this->xSpeed;
        return 0;
    }
    
    if (this->yScreenLocation <= 0){
        this->yScreenLocation = 0;
        this->ySpeed = -this->ySpeed;   
        return 0;
    }
    
    if (this->yScreenLocation >= paddle_y){
        this->ySpeed = -this->ySpeed;   
        return 1;
    } 
}

//Sets the size of the screen for wall bouncing
void Ball::setScreenDims(int width, int height){
     this->width = width;
     this->height = height;
}

//Checks to see if the ball has hit the paddle, if so changes the balls
//direction according to where it was hit
//TODO: Probably call this handleObjectHit. Only handles paddle and ball hit???
void Ball::checkObjectHit(int object_x, int object_y, int object_length){
    
    if (this->yScreenLocation + ballDimension >= object_y  &&
    this->xScreenLocation + ballDimension >= object_x &&
    this->xScreenLocation <= object_x + object_length){
        
        // Get center location of paddle
        int paddle_center = object_x +
        object_length / 2;
        
        int ball_center = this->xScreenLocation +
        ballDimension / 2;
        
        // Find the location on the paddle that the ball hit
        int paddle_location = ball_center - paddle_center;
        
        // Increase X speed according to distance from center of paddle.
        this->xSpeed = paddle_location / 5;
        this->ySpeed = -this->ySpeed;
        this->xScreenLocation += this->xSpeed;
        this->yScreenLocation += this->ySpeed;
    }
}

//This will need to be called should we have multiple balls ;)
Ball::~Ball(){
    SDL_FreeSurface(BallImage);
}
