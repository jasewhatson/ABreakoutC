// 
// File:   HitDetection.h
// Author: jason
//
// Created on February 5, 2007, 12:37 AM
//

#ifndef _HitDetection_H
#define	_HitDetection_H

/*Very simple object hit detection. So simple...*/
class HitDetection{
    public:
        
        /*Checks to see if a location is inside a given area (block)*/
        static bool checkPointInRect(int x, int y, int block_x, int block_y, 
                         int block_w, int block_h);
};

#endif	/* _HitDetection_H */

