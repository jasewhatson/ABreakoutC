// 
// File:   MapReader.h
// Author: jason
//
// Created on January 22, 2007, 4:53 AM
//

#ifndef _MapReader_H
#define	_MapReader_H
#include <fstream>
#include <vector>

class MapReader{
    public:
        MapReader();
        std::vector <int> map;
        std::vector <int> *loadMap(int level,char* LevelsLocation);
        int mapLength;
        int mapHeight;
        bool readMapExtra(std::vector<std::string> *strv, int level, char* LevelsLocation);
    private:
        std::ifstream fin;
};



#endif	/* _MapReader_H */

