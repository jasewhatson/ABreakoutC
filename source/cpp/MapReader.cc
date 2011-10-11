#include <iostream>
#include <sstream>

#include "SDL/SDL_image.h"

#include "../headers/MapReader.h"

MapReader::MapReader() : mapHeight(0), mapLength(0){
}

std::vector <int> *MapReader::loadMap(int level, char* LevelsLocation){
    
    std::ostringstream outs;  // Declare an output string stream.
    char c; //Holds the read char from the file
    
    outs << LevelsLocation << "/Level" << level << ".map";  // Convert integer into a string. (got itoa?)
    
    std::cout << outs.str() << std::endl;//Deguggin...
    
    std::ifstream fin(outs.str().c_str());//Open out map file
    
    //If the file existed read all the chars in it. Else return NULL. No map loaded
    if (fin.is_open()){
        while (! fin.eof() ){
            
            fin.get(c);//Read the next char
            
            //If we find a newline char increase map height else inc map lenght
            if (c == '\n'){
                this->mapHeight++;
            }else{
                map.push_back(c);
                this->mapLength++;
            }   
        }
        
        fin.close();
        
    }else{
        return NULL;
    }
    
    /*Ive been debugging all the live long day!
     for (int i = 0; i < map.size(); i++) {
           std::cout << (char) map[i] << std::endl;
    }*/

    return &map; //Return the map
}

bool MapReader::readMapExtra(std::vector<std::string> *strv, int level, char* LevelsLocation){
    
    std::ostringstream outs;  // Declare an output string stream.
    
    outs << LevelsLocation << "/Level" << level << ".sup";  // Convert value into a string.
    
    std::cout << outs.str() << std::endl;
    
    this->fin.open(outs.str().c_str());
    //Puts the lines of the file into a string vectror.
    if (fin.is_open()){
        
        std::string temp;
        while (! fin.eof() ){
            getline(fin, temp);
            std::cout << "Temp -" << temp << std::endl;
            strv->push_back(temp);
        }
        
        this->fin.close();
        return true;
    }
    
    return false;
}