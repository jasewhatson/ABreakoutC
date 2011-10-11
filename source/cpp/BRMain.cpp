/*STD Librarys*/
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>

/*SDL librarys*/
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL.h"

/*My objects*/
#include "../headers/GameScreen.h"
#include "../headers/GameImages.h"
#include "../headers/Ball.h"
#include "../headers/Paddle.h"
#include "../headers/MapReader.h"
#include "../headers/Blocks.h"
#include "../headers/Powerup.h"
#include "../headers/Sprite.h"
#include "../headers/HitDetection.h"

/*CONSTANTS
 ----------*/

//Specifies how many FPS to run at
const int FPS = 60;

//The attributes of the screen
const int SCREEN_WIDTH = 520;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Ball Consts
const int BALL_DIMENSION = 15;

const int START_BALL_X_SPEED = -2;
const int START_BALL_Y_SPEED = 5;

const int START_BALL_X_LOCATION = SCREEN_WIDTH / 2 - BALL_DIMENSION / 2;
const int START_BALL_Y_LOCATION = 200;

//Paddle Consts 
const int PADDLE_WIDTH = 70;
const int START_PADDLE_X_LOCATION = 0;
const int START_PADDLE_Y_LOCATION = 440;

//Block Consts
const int BLOCK_WIDTH = 40;
const int BLOCK_HEIGHT = 15;

//Items/Powerups Consts
const int ITEM_DROP_RATE = 3;
const int NO_DIFFRENT_POWERUPS = 3;

//Powerup item codes
const int DOUBLE_SPEED = 1;
const int ONE_HIT_BREAK = 2;
const int PADDLE_WIDER = 3;

/*image resource locations
  ------------------------*/
char* RL_BALL_IMAGE = "Resources/Images/Ball.png"; 
char* RL_Paddle_IMAGE = "Resources/Images/Paddle.png";
char* RL_Paddle_IMAGE_LONG = "Resources/Images/PaddleLong.png";

char* RL_BLOCK_FULL = "Resources/Images/Blue.png";     
char* RL_BLOCK_MEDIUM = "Resources/Images/PurpleBlock.png";
char* RL_BLOCK_LOW = "Resources/Images/RedBlock.png";
char* RL_BLOCK_NBBLOCK = "Resources/Images/NonBreakBlock.png";

char* PU_SLOWPADDLE = "Resources/Images/Powerup.png";

char* PU_DOUBLESPEED = "Resources/Images/PUAllYellow.png";
char* PU_ALLBLOCKRED = "Resources/Images/PUAllRed.png";
char* PU_ORANGEBLOCK = "Resources/Images/PUOrange.png";

char* HEART_IMAGE_LOCATION = "Resources/Images/heart.png";

//Sprites locations
char* RL_BLOCKDESTROY = "Resources/Sprites/BlockBreak/BlockSprite.png";

//Level locations 
char* LVL_DIR_LOCATION = "Resources/Levels";

/*Sprite objects
------------------*/

/*A structure to define the location we need to animate the blockDestroySprite
 and a vector of DestroySprite structs
 ----------------------------------------------------------------------------*/
struct DestroySprite{
    int location_X;
    int location_Y;
    Sprite* blockDestroySprite1;
};

std::vector<DestroySprite*> ds; 

/*Game engine related objects
 ----------------------------*/
//Used to load images
GameImages* gi = new GameImages();

//Renders and sets up a window to play our game
GameScreen* gs = new GameScreen();

//An instance of the map reader, reads in map files from disk
MapReader* mr = new MapReader();

/*Various strings which are drawn on the screen
 ---------------------------------------------*/
std::string strLevel = "Level ";

bool showWin = false;

char s[2]; //Holds the level number after its converted from integer to ascii

char* credits = "Credits - Programming and Graphics: Jason Whatson\0";

int PaddelSpeed = 5;//TODO: Make this constant and held in the paddle class

//The of the text
SDL_Color textColor = { 255, 255, 255 }; //White

//The font that's going to be used 
TTF_Font *font = NULL;

const char* FONT_LOCATION = "Resources/Fonts/obloquys.ttf";

/*Holds which power ups are on
  ----------------------------*/
bool oneHitBreakSwitch = false;
bool fastModeSwitch = false;
bool widerPaddleSwitch = false;

/*Game objects
  -------------*/
Ball* b1; 
Paddle* p1;

/*Graphics
 ---------------*/
SDL_Surface* playerHeart;
SDL_Surface* text = NULL;
SDL_Surface* blockDestroyFrameSheet;
SDL_Surface* redBlock;

SDL_Surface* paddle;
SDL_Surface* paddleLong;

SDL_Surface* pu_DoubleSpeed = NULL;
SDL_Surface* pu_Random = NULL;
SDL_Surface* pu_AllBlockDestroy = NULL;
SDL_Surface* pu_WiderPaddle = NULL;

/*Key and mouse event related
-----------------------------*/
//Used for keyboard events
SDL_Event event;

//Holds if and which keys are being held
bool keysHeld[323] = {false}; // everything will be initialized to false

/*Misc vars
 ---------------------------*/
//Indicates where game is over and paused
bool GameOver, Paused;

//Holds which level the player is on
int Level = 1;

//Holds the number of lives a player has left
int playerLives = 3;

bool ballStateIdel = true; //???

//An array of block pointers representing our map
std::vector<Block*> blockMap; 

//Holds the falling powerups
std::vector<Powerup*> puv; 

/*Global Functions declearations
 ---------------------------------*/
void checkForEvents(); //Checks for key events
void clearScreen(); //Clears the screen to render a new frame
void drawGame(); //Draws the game
void init(); //Loads resources and sets up the game for play
void runGame(); //The game loop
void handleBlockHit(); //Checks and handles the ball when a block is hit.
//Applys a effect to a block when its hit
void blockHit(Block* hitblock,int vectorLocation);
void loadLevel(int Level); //Loads a level
void freeMemory();//This is pretty pointless
//Loads the powerups into a level
int getBlockExtras(std::vector<std::string> *strv,int r,int c);
void resetGame();//Restarts a game as if it was new
void handleItems();//Handles a item drop
void clearPowerups();//Clears any falling powerups from the screen or active

//Turns on or off the effect of a powerup
void doubleSpeed(bool on); //Doubles speed of ball and paddle
void oneHitBreak(bool on); //Makes all blocks red (one hit to break it)
void widerPaddle(bool on);//Increases paddle lenght

//Powerup* cpu; delete
           
//Entry to the game
int main(int argc, char** argv) {
    
    if(argc > 1){
        Level = atoi(argv[1]);
    }
    
    init(); //Sets up game to start
    runGame();  //runs the game
    freeMemory(); //Frees and exits game (dosnt do anything because memory is free'd on exit anyway)
    return (EXIT_SUCCESS);
}

//The game loop
void runGame(){
    
    //Vars so we can get our avaliable sleeps lenght
    int startTime = 0;
    int endTime = 0;
    int timeToProcessOneFrame = 0;
    
    //Loop the game loop while its not game over
    while(!GameOver){
        
        startTime = SDL_GetTicks(); //Get the time before we process a frame
        
        //If not paused move the ball.
        if (!Paused){
            
            //Move the ball, if 1 is returned in means the player has missed the ball.
            //MoveBall also handles wall hits
            if (b1->MoveBall(p1->yScreenLocation) == 1){
                
                //TODO: Make the following a method called handleBallMiss()
                b1->xScreenLocation = START_BALL_X_LOCATION;
                b1->yScreenLocation = START_BALL_Y_LOCATION;
                playerLives--;
                b1->xSpeed = 0;
                b1->ySpeed = 0;
                ballStateIdel = true;
                doubleSpeed(false);
                oneHitBreak(false);
                widerPaddle(false);
                clearPowerups();
            }
            
            handleBlockHit();//Check for hits with blocks
            
            //Check for hits with paddle
            b1->checkObjectHit(p1->xScreenLocation, p1->yScreenLocation, p1->paddleWidth);
            
            //Handle the status of any items that may of been droped
            handleItems();
            
            //Basically checks to see 
            //blockDestroySprite->animate();//need
            
            drawGame(); //Draws the game to the sceen 
            
            //Show FPS
            gs->CalculateFrameRate();
        }
        
        //Check for things like key events  (outside of paused condition so unpaused will work)
        checkForEvents();
        
        endTime = SDL_GetTicks(); //Get the time after 1 frame has been processed
        
        timeToProcessOneFrame = endTime - startTime; //Find how long it took to process one frame
        
        //Find how long its going to take to process frames at the frame rate we want.
        int timeToProcess60Frames = timeToProcessOneFrame * FPS;
        
        //Work out how much free time in the second we have if we were to only process the frame rate ammount of frames per second.
        int freeMLS =  1000 - timeToProcess60Frames;
        
        //Divide our free time between the ammount of frames we are going to process in the second.
        int timeToSleepPerFrame = freeMLS / FPS;
        
        //After every frame if we do have time to spare wait for that length of spare time.
        //This spare time allows the CPU to be used for other applications.
        if (timeToSleepPerFrame > 0){
            SDL_Delay(timeToSleepPerFrame);
        }
    }
}

//Sets up things like our graphics resources, map resources and game objects
void init(){
    
    //Sets up a SDL screen, such as a xwindow
    gs->init(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP);
    
    gs->debug = true;//Set debugging on
    
    //Initialize SDL_ttf so we can write to the screen. 
    TTF_Init();
    
    //Open the font 
    font = TTF_OpenFont(FONT_LOCATION,15);      
    
    //Load heart sprite
    playerHeart = gi->load_image(HEART_IMAGE_LOCATION);
    
    //Explosion animation... 
    blockDestroyFrameSheet =  gi->load_image(RL_BLOCKDESTROY);
        
   // blockDestroySprite = new Sprite(blockDestroyFrameSheet,40,15,11,500);
    
    redBlock = GameImages::load_image(RL_BLOCK_LOW);
    
    paddle = GameImages::load_image(RL_Paddle_IMAGE);
    paddleLong = GameImages::load_image(RL_Paddle_IMAGE_LONG);

    //Sets up our block gfx    
    Block::load(GameImages::load_image(RL_BLOCK_FULL),
                GameImages::load_image(RL_BLOCK_MEDIUM),
                redBlock,
                GameImages::load_image(RL_BLOCK_NBBLOCK),
                BLOCK_HEIGHT, BLOCK_WIDTH);
    
    //Create a new ball object
    b1 = new Ball(GameImages::load_image(RL_BALL_IMAGE), 0,
    0, BALL_DIMENSION, START_BALL_X_LOCATION,
    START_BALL_Y_LOCATION);
    
    //Give the ball the bounderies which it can bounce
    b1->setScreenDims(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    //Create a new paddle object
    p1 = new Paddle(paddle, PADDLE_WIDTH,
    START_PADDLE_X_LOCATION, START_PADDLE_Y_LOCATION);
    
    //Give the paddle the boundies which it can move
    p1->setScreenDims(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    //Load the first level
    loadLevel(Level);
   
}

//Draws our game graphics to the screen
void drawGame(){
     
    clearScreen(); //Clear all gfx on the screen
    
    //Draw the destroy sprites
    for (int i = 0; i < ds.size(); i++) {
        
        //TODO: This shouldnt be in drawGame. 
        if(ds[i]->blockDestroySprite1->status == Sprite::FINISHED){
            delete ds[i]->blockDestroySprite1;
            ds.erase(ds.begin() + i);
        }
         
         //We need this to animate or sprite.
         ds[i]->blockDestroySprite1->animate();
         
         gs->apply_surface(ds[i]->location_X,ds[i]->location_Y,
         ds[i]->blockDestroySprite1->spritesheet,ds[i]->blockDestroySprite1->getFrame()); 
    }
    
    //Draw the blocks in the game to the screen at their positions
    for (int i=0;i < blockMap.size();i++){
        //If the oneHitBreakMode is on we draw all the blocks as red
        if (oneHitBreakSwitch && blockMap[i]->NoHits != 5){
            gs->apply_surface(blockMap[i]->BlockXLocation,
            blockMap[i]->BlockYLocation,redBlock);
        }else{
            gs->apply_surface(blockMap[i]->BlockXLocation,
            blockMap[i]->BlockYLocation,blockMap[i]->getBlockImage());
        }
    }
    
    //Draw the paddle
    gs->apply_surface(p1->xScreenLocation,p1->yScreenLocation,p1->PaddleImage);
    //Draw the ball
    gs->apply_surface(b1->xScreenLocation,b1->yScreenLocation,b1->BallImage);
    
    //Draw how many lives the player has left
    int startx = 490;
    for (int i = 0; i < playerLives; i++) {
        gs->apply_surface(startx,455,playerHeart);
        startx -= 21;
    }
   
    //Convert integer to ascii, store result in 's'
    sprintf(s,"%d", Level);
    
    //Render the text for the 'Level no'
    text = TTF_RenderText_Blended(font, (strLevel + s).c_str() ,textColor);
    
    gs->apply_surface(5,460,text);//Draw the level text
    
    //If the game is over show credits and restart game.
    if (playerLives <= 0){
        
        text = TTF_RenderText_Blended(font, "Game Over!" ,textColor);
        gs->apply_surface(195,380,text);
        
        font = TTF_OpenFont(FONT_LOCATION,10);  
        text = TTF_RenderText_Blended(font, credits ,textColor);
        gs->apply_surface(40,400,text);
        resetGame();
        Paused = true;
    }

    //Show you win! If player has won
    if (showWin == true){    
        font = TTF_OpenFont(FONT_LOCATION,30);  
        text = TTF_RenderText_Blended(font, "You win!" ,textColor);
        gs->apply_surface(175,380,text);
        resetGame();
        Paused = true;
    }
    
    //Draw the powerups if the powerup is on 
    if(fastModeSwitch){
        gs->apply_surface(105,455,pu_DoubleSpeed);
    }
    
    if(oneHitBreakSwitch){
        gs->apply_surface(132,455,pu_AllBlockDestroy);
    }
    
    if(widerPaddleSwitch){
         gs->apply_surface(159,455,pu_WiderPaddle);
    }
    
    //Draw the power ups falling
    for (int i = 0; i < puv.size(); i++) {
        if (puv[i]->visable){
            gs->apply_surface(puv[i]->xpos,puv[i]->ypos,puv[i]->sprite);
        }
    }

    //Update the screen   
    SDL_Flip(gs->getScreen());
    
    SDL_FreeSurface(text);//Needed or we get a memory leek from creating font objects and lossing refrence
    text = NULL;
}

//Checks and handles the ball when a block is hit.
void handleBlockHit(){

    // Calculate collision points on the ball
    
    /**
     Hit points = 0
    
    0----0----0
    |         |
    0         0
    |         |
    0----0----0
     
     */
    
    //Defines locations of the hit points
    int left_x   = b1->xScreenLocation;
    int left_y   = b1->yScreenLocation + b1->ballDimension/2; 
    int right_x  = b1->xScreenLocation + b1->ballDimension; 
    int right_y  = b1->yScreenLocation + b1->ballDimension/2; 
    int top_x    = b1->xScreenLocation + b1->ballDimension/2;
    int top_y    = b1->yScreenLocation;
    int bottom_x = b1->xScreenLocation+ b1->ballDimension/2;
    int bottom_y = b1->yScreenLocation + b1->ballDimension;
    
    /*Not used. May be used some day...
    int north_west_x = b1->xScreenLocation;
    int north_west_y = b1->yScreenLocation;
    
    int north_east_x = b1->xScreenLocation + b1->ballDimension;
    int north_east_y = b1->yScreenLocation;
    
    int south_east_x = b1->xScreenLocation + b1->ballDimension;
    int south_east_y = b1->yScreenLocation + b1->ballDimension;
    
    int east_west_x = b1->xScreenLocation;
    int east_west_y = b1->yScreenLocation + b1->ballDimension;
    */
    
    //Hold which points have been hit
    bool top = false;
    bool bottom = false;
    bool left = false;
    bool right = false;
    
    /*Not used again. May be used some day
    bool north_west = false;
    bool north_east = false;
    bool south_east = false;
    bool east_west = false;
    */
    
    //Holds wheather a block is hit
    bool b_blockHit = false;
    
    //Loop though all the blocks in the game and check for contact
    for (int i=0;i < blockMap.size();i++){
        
        b_blockHit = false;//Reset block hit
        
        // Top 
        if (HitDetection::checkPointInRect(top_x, top_y, blockMap[i]->BlockXLocation,
        blockMap[i]->BlockYLocation, Block::BlockWidth, Block::BlockHeight)){
            top = true;
            b_blockHit = true;
        }
        
        // Bottom //
        if (HitDetection::checkPointInRect(bottom_x, bottom_y, blockMap[i]->BlockXLocation, 
        blockMap[i]->BlockYLocation, Block::BlockWidth, Block::BlockHeight)){
            bottom = true;
            b_blockHit = true;
        }
        
        // left 
        if (HitDetection::checkPointInRect(left_x, left_y, blockMap[i]->BlockXLocation, 
        blockMap[i]->BlockYLocation,Block::BlockWidth, Block::BlockHeight)){
            left = true;
            b_blockHit = true;
        }
        
        // right
        if (HitDetection::checkPointInRect(right_x, right_y, blockMap[i]->BlockXLocation, 
        blockMap[i]->BlockYLocation, Block::BlockWidth, Block::BlockHeight)){
            right = true;
            b_blockHit = true;
        }
        
        //If the block was hit. Handle the hit.
        if(b_blockHit == true){
            blockHit(blockMap[i], i);
        }
        
        /*Not used. May be used some day
        //north_east
        if (HitDetection::checkPointInRect(north_east_x, north_east_y, blockMap[i]->BlockXLocation,
        blockMap[i]->BlockYLocation, Block::BlockWidth, Block::BlockHeight)){
            north_east = true;
            blockHit(blockMap[i], i);
        }
        
        //north_west
        if (HitDetection::checkPointInRect(north_west_x, north_west_y, blockMap[i]->BlockXLocation,
        blockMap[i]->BlockYLocation, Block::BlockWidth, Block::BlockHeight)){
            north_west = true;
            blockHit(blockMap[i], i);
        }
        
         //south_east
        if (HitDetection::checkPointInRect(south_east_x, south_east_y, blockMap[i]->BlockXLocation,
        blockMap[i]->BlockYLocation, Block::BlockWidth, Block::BlockHeight)){
            south_east = true;
            blockHit(blockMap[i], i);
        }
        
        //east_west
        if (HitDetection::checkPointInRect(east_west_x, east_west_y, blockMap[i]->BlockXLocation,
        blockMap[i]->BlockYLocation, Block::BlockWidth, Block::BlockHeight)){
            east_west = true;
            blockHit(blockMap[i], i);
        }
        */
    }
    
    /*Not used. May be used some day
    if (north_west == true && top == false && bottom == false && left  == false && right == false){
        b1->ySpeed = -b1->ySpeed;
        b1->xSpeed = -b1->xSpeed;
        
        b1->yScreenLocation += 2;
        b1->xScreenLocation += 2;
        return;
    }
    
    if (north_east == true && top == false && bottom == false && left  == false && right == false){
        b1->ySpeed = -b1->ySpeed;
        b1->xSpeed = -b1->xSpeed;
        
        b1->yScreenLocation += 2;
        b1->xScreenLocation -= 2;
        return;
    }
    
    if (south_east == true && top == false && bottom == false && left  == false && right == false){
        b1->ySpeed = -b1->ySpeed;
        b1->xSpeed = -b1->xSpeed;
        
        b1->yScreenLocation -= 2;
        b1->xScreenLocation -= 2;
        return;
    }
    
    if (east_west == true && top == false && bottom == false && left  == false && right == false){
        b1->ySpeed = -b1->ySpeed;
        b1->xSpeed = -b1->xSpeed;
        
        b1->yScreenLocation -= 2;
        b1->xScreenLocation += 2;
        return;
    }
    */
    
    //Adjust ball so we dont hit up multiply hits. Adjust direction accordingly
    if (top){
        b1->ySpeed = -b1->ySpeed;
        b1->yScreenLocation += 2;
    }
    if (bottom){
        b1->ySpeed = -b1->ySpeed;
        b1->yScreenLocation -= 2;
    }
    if (left){
        b1->xSpeed = -b1->xSpeed;
        b1->xScreenLocation += 2;
    }
    if (right){
        b1->xSpeed = -b1->xSpeed;
        b1->xScreenLocation -= 2;
    }
}

//Handles all types of events 
//(We are mainly concerned with key events)
void checkForEvents(){
    
    static bool left_pressed  = false;
    static bool right_pressed = false;

    // Fill our event structure with event information. //
    if ( SDL_PollEvent(&event) ){
        // Handle user manually closing game window //
        if (event.type == SDL_QUIT){			
            GameOver = true;
            
        // Handle keyboard input here //
        }else if (event.type == SDL_KEYDOWN){

            if (event.key.keysym.sym == SDLK_LEFT){
                    left_pressed = true;
            }else if (event.key.keysym.sym == SDLK_RIGHT){
                    right_pressed = true;
            }else if (event.key.keysym.sym == SDLK_p){
                Paused = !Paused;
                if(showWin == true){
                    showWin = false;
                }
            }else if (event.key.keysym.sym == SDLK_SPACE){
                if (ballStateIdel == true){
                    b1->xSpeed = START_BALL_X_SPEED;
                    b1->ySpeed = START_BALL_Y_SPEED;
                    ballStateIdel = false;
                }
                
            }
            
        }else if (event.type == SDL_KEYUP){
            
            if (event.key.keysym.sym == SDLK_LEFT){
                left_pressed = false;
            }else if (event.key.keysym.sym == SDLK_RIGHT){
                right_pressed = false;
            }
        }		
    }

    // This is where we actually move the paddle //
    if (left_pressed){
        p1->movePaddle(true,PaddelSpeed);
                
    }
    if (right_pressed){
        p1->movePaddle(false,PaddelSpeed);
    }    
}

// This function simply clears the back buffer to black. //
void clearScreen(){
	
        // This function just fills a surface with a given color. The
	// first 0 tells SDL to fill the whole surface. The second 0 
	// is for black. 
	SDL_FillRect(gs->getScreen(), 0, 0);
}

//Applys a effect to a block when its hit
void blockHit(Block* hitblock,int vectorLocation){
    
    //If a block is not a non breakable one reduce its hit health
    if (hitblock->NoHits != 5){ //TODO: Make 5 a constant called NON_BREAKABLE_BLOCK
         hitblock->NoHits--;
        
        //If the block is now down
        if (hitblock->NoHits <= 0 || oneHitBreakSwitch == true){
            
            //Make a new destroyed block sprite. Add it to our vector
            struct DestroySprite *s = new DestroySprite;
            
            s->blockDestroySprite1 = new Sprite(blockDestroyFrameSheet,40,15,11,500);
            s->location_X = hitblock->BlockXLocation;
            s->location_Y = hitblock->BlockYLocation;
            
            ds.push_back(s);
            
            //If the destroyed block has a item
            if (hitblock->Extra != -1){ //if (true){ 
                
                //Define start location of dropping powerup
                int powerupxloc = (hitblock->BlockXLocation + (hitblock->BlockWidth / 2)) - 11;
                        
                //TODO: Make constant 
                if(hitblock->Extra == DOUBLE_SPEED){
                    
                    if(pu_DoubleSpeed == NULL){
                        pu_DoubleSpeed = gi->load_image(PU_DOUBLESPEED);
                    }
                    
                    //Add a new power up the power up aray
                    Powerup* cpu = new Powerup("Double speed", powerupxloc,
                    hitblock->BlockYLocation, pu_DoubleSpeed, &doubleSpeed);
                    puv.push_back(cpu);
                }else if(hitblock->Extra == ONE_HIT_BREAK){
                    
                    if(pu_AllBlockDestroy == NULL){
                        pu_AllBlockDestroy = gi->load_image(PU_ALLBLOCKRED);
                    }
                    
                    //Add a new power up the power up aray
                    Powerup* cpu = new Powerup("One hit break", powerupxloc,
                    hitblock->BlockYLocation, pu_AllBlockDestroy, &oneHitBreak);
                    puv.push_back(cpu);
                }else if(hitblock->Extra == PADDLE_WIDER){
                    
                    if(pu_WiderPaddle == NULL){
                        pu_WiderPaddle = gi->load_image(PU_ORANGEBLOCK);
                    }
                    
                    //Add a new power up the power up aray
                    Powerup* cpu = new Powerup("Powerup", powerupxloc,
                            hitblock->BlockYLocation, pu_WiderPaddle, &widerPaddle);
                    puv.push_back(cpu);
                }else{
                    
                    srand ( time(NULL) );//Seed the randomizer
                    
                    if(pu_Random == NULL){
                        pu_Random = gi->load_image(PU_SLOWPADDLE);
                    }
                    
                    //Generate the random number. Corresponding to a powerup
                    int rn = rand() % NO_DIFFRENT_POWERUPS + 1;
                    
                    Powerup* cpu;
                    
                    switch(rn){
                        
                        case DOUBLE_SPEED:
                            
                            if(pu_DoubleSpeed == NULL){
                                pu_DoubleSpeed = gi->load_image(PU_DOUBLESPEED);
                            }
                            
                            //Add a new power up the power up aray
                            cpu = new Powerup("Powerup", powerupxloc,
                            hitblock->BlockYLocation, pu_Random, &doubleSpeed);//doubleSpeed //oneHitBreak
                            break;
                        case ONE_HIT_BREAK:
                            
                            if(pu_AllBlockDestroy == NULL){
                                pu_AllBlockDestroy = gi->load_image(PU_ALLBLOCKRED);
                            }
                            
                            //Add a new power up the power up aray
                            cpu = new Powerup("Powerup", powerupxloc,
                            hitblock->BlockYLocation, pu_Random, &oneHitBreak);//doubleSpeed //oneHitBreak
                            break;
                        case PADDLE_WIDER:
                            
                            if(pu_WiderPaddle == NULL){
                                pu_WiderPaddle = gi->load_image(PU_ORANGEBLOCK);
                            }
                            
                            //Add a new power up the power up aray
                            cpu = new Powerup("Powerup", powerupxloc,
                            hitblock->BlockYLocation, pu_Random, &widerPaddle);//doubleSpeed //oneHitBreak
                            break;
                    }

                    puv.push_back(cpu);//Add out powerup to our vector of power ups
                }

            }
            
            //Remove the block
            blockMap.erase(blockMap.begin() + vectorLocation);
            Block::BlocksLeft--;
            
        }
    }
     
    //If there are no block left move to the next level
    if (Block::BlocksLeft <= 0){
        b1->xScreenLocation = START_BALL_X_LOCATION;
        b1->yScreenLocation = START_BALL_Y_LOCATION;
        b1->xSpeed = 0;
        b1->ySpeed = 0;
        ballStateIdel = true;
        
        Level++;
        clearPowerups();
        loadLevel(Level);
    }
}

//Load a level
void loadLevel(int Level){
    
    bool mapExtraFound = false;
    
    //Clear our array of blocks which represnt our maps
    blockMap.clear();
    
    //Make a new instance of a map reader
    mr = new MapReader();
    
    //Load map extra infomation 
    std::vector<std::string> strv;
    mapExtraFound = mr->readMapExtra(&strv,Level,LVL_DIR_LOCATION);
    
    //Load the actual map data
    std::vector<int>* map = mr->loadMap(Level,LVL_DIR_LOCATION);
    
    //If map is null. The map couldnt be loaded. Assume the player has beaten the game
    if (map == NULL){std::cout << "Error loading map" << std::endl; showWin = true;}
    
    //Holds the x and y positions of the blocks. These get incremented 
    //depending on which row or col we are on
    int blockposy =0,blockposx =0, extras = -1;
    
    //Calcs the number of columns in the map
    int mapCols =  mr->mapLength / (mr->mapHeight - 1);
    
    Block::BlocksLeft = 0;//Resuts block count. This is recalculated below
    
    int k = 0;
    
    //Loops through the contents of map to create a vector of the map blocks
    //and the position which the block is at and the ammount of hits the block has
    for (int j=0;j < mr->mapHeight - 1;j++){
        for(int i=0;i < mapCols;i++){
            
            if(j == 11 && i == 11){
                int ff = 0;
            }
             
            if(mapExtraFound == true){
                //Retrieve extra infomation about the current block
                extras = getBlockExtras(&strv,j+1,i+1);
            }
            
           //Debugging - Delete
           if (extras != -1){
                std::cout << "j=" << j << "i=" << i << std::endl;
           }
            
           if(k == 15){
               int ff = 0;
           }
            
             //Dereferences the map vector and converts its current element to a char
             switch((char) (*map)[j * mapCols + i]){

                 //Depending on the ammount of hits a block has we create new
                 //block instances and add these to our map vector
                  case '1':
                       blockMap.push_back(new Block(extras,1,blockposy,blockposx));
                       Block::BlocksLeft++;
                       k++;
                       break;         
                  case '2':
                       blockMap.push_back(new Block(extras,2,blockposy,blockposx));
                       Block::BlocksLeft++;
                       k++;
                       break;
                  case '3':
                       blockMap.push_back(new Block(extras,3,blockposy,blockposx));
                       Block::BlocksLeft++;
                       k++;
                       break;
                  case '5':
                       blockMap.push_back(new Block(extras,5,blockposy,blockposx));
                       k++;
                       break;
             }
             
             extras = -1;
            
             //Increment our column pos the ammount of the block width
             blockposx += Block::BlockWidth;  
        }
        
        //After each row move the column pos back to the start and increment our row pos
        blockposx = 0;
        blockposy += Block::BlockHeight;
     }
}

//Retrives extra infomation about a block
//TODO: Kill c++, Remove curse words. 
int getBlockExtras(std::vector<std::string> *strv,int r,int c){
    std::string temp;
    
    for (int i=0;i < strv->size() -1;i++){
    
        temp = (*strv)[i];

        int fuck = atoi(temp.c_str());

        //std::cout << "fuck=" << fuck << std::endl;
        int one = (int) floor(fuck / 10000);
        //std::cout << "one=" << one << std::endl;

        int two = (int) fmod(fuck, 10000);
        two = (int) floor(two / 100);
        
        //std::cout << "two=" << two << std::endl;

        int three = (int) fmod(fuck, 100);
        //std::cout << "three=" << three << std::endl;
        
        if (one == r && c == two){
            return three;
        }
    }
    
    return -1;
}

//Resets a game
void resetGame(){
    Level = 1;
    playerLives = 3;
    loadLevel(Level);
    //Open the font 
    font = TTF_OpenFont("Resources/Fonts/obloquys.ttf",15);  
}

//Deletes some objects
void freeMemory(){
   delete gi;
   delete gs;
   delete mr;
   delete p1;
   delete b1;
   Block::freeSDLResources();
}

//Handles a item drop
void handleItems(){
    int val = puv.size();
    
    for(int i=0;i < val;i++){
        
        if (puv[i]->ypos > SCREEN_HEIGHT){
            puv.erase(puv.begin() + i);
        }
        
        //Only move the item if it hasnt already been picked up
        if (puv[i]->itemRecived == false){
            puv[i]->ypos += ITEM_DROP_RATE;
        }
        
        //Check to see if a object is collected
        if (puv[i]->checkObjectHit(p1->xScreenLocation, p1->yScreenLocation, p1->paddleWidth) && puv[i]->visable == true){
            puv[i]->length = 8000;
            puv[i]->itemRecived = true;
            puv[i]->startTimer();
            puv[i]->visable = false;
        }
        
        //Check to see if after a object is collected it is no longer active
        if (puv[i]->checkIfPowerupActive() == false){
            puv.erase(puv.begin() + i);
        }
    }
}

void oneHitBreak(bool on){
    if(on){
        oneHitBreakSwitch = true;
    }else{
        oneHitBreakSwitch = false;
    }
}

void doubleSpeed(bool on){
    if(on){
        if (fastModeSwitch == false){
            b1->xSpeed *= 2;
            b1->ySpeed *= 2;
            PaddelSpeed *= 2;
            std::cout << "\nPowerup on" << std::endl;
            fastModeSwitch = true;
        }
    }else{
        if(fastModeSwitch == true){
            b1->xSpeed /= 2;
            b1->ySpeed /= 2;
            PaddelSpeed /= 2;
            std::cout << "\nPowerup off" << std::endl;
            fastModeSwitch = false;
        }
    }
}

void widerPaddle(bool on){
    if(on){
        if (widerPaddleSwitch == false){
            p1->paddleWidth += 15;
            p1->PaddleImage = paddleLong;
            p1->xScreenLocation -= 7;
            widerPaddleSwitch = true;
        }
    }else{
        if (widerPaddleSwitch == true){
            p1->paddleWidth -= 15;
            p1->PaddleImage = paddle;
            p1->xScreenLocation += 7;
            widerPaddleSwitch = false;
        }
    }
}

void clearPowerups(){
    int val = puv.size();
    
    for(int i=0;i < val;i++){
        puv[i]->visable = false;
    }
}



