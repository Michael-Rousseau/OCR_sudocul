#include "square_detection.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
//#include "detection.c"

struct Square {
  struct Point topleft;
  struct Point topright;
  struct Point bottomleft;
  struct Point bottomright;
};

void horizontal_vertical_lines(struct Line *lines, int len,
                               struct Line *horizon, struct Line *vertical) {
  // thtea en degree
  int j = 0, k = 0;
  for (int i = 0; i < len; i++) {
    float t = lines[i].theta;
    if ((t >= -10 && t <= 10) || (t >= 170 && t <= 190)) {
      vertical[j] = lines[i];
      j++;
    } else {
      horizon[k] = lines[i];
      k++;
    }
  }
  // return (j+1,k+1);
}

void sort_horizontal_lines(struct Line *horizon, int len) {
  int i, j;
  struct Line compare;

  for (i = 1; i < len; i++) {
    // printf("(%f,%f)",horizon[i].start.x,horizon[i].start.y);
    compare = horizon[i];
    j = i - 1;

    while (j >= 0 && horizon[j].start.y > compare.start.y) {
      horizon[j + 1] = horizon[j];
      j = j - 1;
    }
    horizon[j + 1] = compare;
  }
}

void sort_vertical_lines(struct Line *vertical, int len) {
  int i, j;
  struct Line compare;

  for (i = 1; i < len; i++) {
    compare = vertical[i];
    j = i - 1;

    // Move elements that are greater than key to one position
    // ahead of their current position
    while (j >= 0 && vertical[j].start.x > compare.start.x) {
      vertical[j + 1] = vertical[j];
      j = j - 1;
    }
    vertical[j + 1] = compare;
  }

  /*	for(int k = 0; k <len; k++)
                  printf("(%f,%f)",vertical[k].start.x,vertical[k].start.y);
          printf("\n");*/
}

void fillsquares(struct Line *vertical, struct Line *horizon,
                 struct Square *squares) {
  int len = 8; // As there are 8 vertical and 8 horizontal lines
  int nx = 0;

  for (int i = 0; i < len - 1; i++) {
    for (int j = 0; j < len - 1; j++) {
      squares[nx].topleft.y = horizon[i].start.y;
      squares[nx].topleft.x = vertical[j].start.x;

      squares[nx].topright.y = horizon[i].start.y;
      squares[nx].topright.x = vertical[j + 1].start.x;

      squares[nx].bottomleft.y = horizon[i + 1].start.y;
      squares[nx].bottomleft.x = vertical[j].start.x;

      squares[nx].bottomright.y = horizon[i + 1].start.y;
      squares[nx].bottomright.x = vertical[j + 1].start.x;

      nx++;
    }
  }
}

void printvalues(struct Line *lines, int len) {
  struct Line *horizon = calloc(8, sizeof(struct Line));
  struct Line *vertical = calloc(8, sizeof(struct Line));

  horizontal_vertical_lines(lines, len, horizon, vertical);

  sort_horizontal_lines(horizon, 8);
  for (int i = 0; i < 8; i++) {
    // printf("x %f", horizon[i].start.x);
    printf("y %f \n", horizon[i].start.y);
  }

  sort_vertical_lines(vertical, 8);

  for (int i = 0; i < 8; i++) {
    // printf("x %f", horizon[i].start.x);
    printf("y %f \n", vertical[i].start.x);
  }

  struct Square *squares = calloc(49, sizeof(struct Square));
  fillsquares(vertical, horizon, squares);

  for (int i = 0; i < 49; i++) {
    printf("%i \n", i);
    printf("Top Left: (%f, %f)\n", squares[i].topleft.x, squares[i].topleft.y);
    printf("Top Right: (%f, %f)\n", squares[i].topright.x,
           squares[i].topright.y);
    printf("Bottom Right: (%f, %f)\n", squares[i].bottomright.x,
           squares[i].bottomright.y);
    printf("Bottom Left: (%f, %f)\n", squares[i].bottomleft.x,
           squares[i].bottomleft.y);
    printf("\n");
  }
  free(horizon);
  free(vertical);
  free(squares);
}
