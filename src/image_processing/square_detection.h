#ifndef SQUARE_DETECTION_H
#define SQUARE_DETECTION_H

#include "detection.h"
struct Squares {
    struct Point topleft;
    struct Point topright;
    struct Point bottomleft;
    struct Point bottomright;
};



void printvalues(struct Line* lines, int len);
#endif
