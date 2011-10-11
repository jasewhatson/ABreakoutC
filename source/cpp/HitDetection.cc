#include "../headers/HitDetection.h"

// Check to see if a point is within a rectangle 
bool HitDetection::checkPointInRect(int x, int y, int block_x, int block_y, 
                                    int block_w, int block_h){
	
        if ((x >= block_x) && (x <= block_x + block_w) && 
           (y >= block_y) && (y <= block_y + block_h) ){
		return true;
	}

	return false;
}
