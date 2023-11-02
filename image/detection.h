#ifndef DETECTION_H
#define DETECTION_H

#define startThreshold 15
#define endThreshold 15

struct Point {
    float x;
    float y;
};

struct Line {
	    float rho ;
	    float theta;
	    struct Point start;
	    struct Point end;
};

 struct Linegroup {
    struct Line average;  
    int numlines; 
} ;


void drawl(struct Line* line, int len,SDL_Renderer* renderer);
#endif
