#ifndef SQUARE_DETECTION_H
#define SQUARE_DETECTION_H

/*struct Point {
	
    float x;
    float y;
};

struct Line {
	    float rho ;
	    float theta;
	    struct Point start;
	    struct Point end;
};
*/

struct Squares {
    struct Point topleft;
    struct Point topright;
    struct Point bottomleft;
    struct Point bottomright;
};



void printvalues(struct Line* lines, int len);
#endif
