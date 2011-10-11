#include "../headers/GameScreen.h"

void GameScreen::apply_surface( int x, int y, SDL_Surface *source, 
                                                   SDL_Rect *rec){
    
    //Rectangle to hold the offsets
    SDL_Rect offset;
    
    //Get offsets
    offset.x = x;
    offset.y = y;
    
    //Blit the surface
    SDL_BlitSurface( source, rec, this->screen, &offset );
    
}

bool GameScreen::init(int screen_width,int screen_height,int screen_bpp){
    
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ){
        return false;    
    }
    
    screen = NULL;
    
    //Set up the screen
    screen = SDL_SetVideoMode(screen_width, screen_height, screen_bpp, 
                              SDL_SWSURFACE);
    
    //If there was in error in setting up the screen
    if( screen == NULL ){
        return false;    
    }
    
    //If everything initialized fine
    return true;    
}

int GameScreen::CalculateFrameRate(){
    
    static float framesPerSecond    = 0.0f;
    
    static float lastTime			= 0.0f;
    static char strFrameRate[50] = {0};
    float currentTime = SDL_GetTicks() * 0.001f;
    int fps;
    
    ++framesPerSecond;
    
    if( currentTime - lastTime > 1.0f ){
        lastTime = currentTime;
        sprintf(strFrameRate, "Current Frames Per Second: %d", int(framesPerSecond));
        
        if(debug)
            SDL_WM_SetCaption(strFrameRate,0);
        
        fps = int(framesPerSecond);
        
        framesPerSecond = 0;
        
        return fps;
    }
}


