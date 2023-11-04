#ifndef JOANNEC_H
#define JOANNEC_H


#define startThreshold 100
#define endThreshold 100

struct Point {
    float x;
    float y;
};

struct Line {
    float rho;
    float theta;
    struct Point start;
    struct Point end;
};

struct DetectedLines{
    struct Line* lines;
    int count;
};


 struct Linegroup {
    struct Line average;
    int numlines;
} ;

struct Squares {
    struct Point topleft;
    struct Point topright;
    struct Point bottomleft;
    struct Point bottomright;
};



void printvalues(struct Line* lines, int len);

struct Squares* drawsquares(struct Line* lines, int len);


struct DetectedLines averagearray(struct Line* Line, int len);

void drawl(struct Line* line, int len,SDL_Renderer* renderer,
        SDL_Texture* texture);

struct DetectedLines performHoughTransform(SDL_Surface *surface);

#endif
