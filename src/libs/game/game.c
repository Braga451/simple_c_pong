#include "game.h"
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 24
#define WIDTH 24
#define LIFES 3
#define PLATAFORM_SIZE 4
#define EMPTY 0
#define WALL 1
#define ROOF_FLOOR 2
#define BALL 3
#define LEFT_ARROW 1
#define RIGHT_ARROW 2
#define PLATAFORM_I (HEIGHT - 4)
#define PLATAFORM_INITIAL_J ( (WIDTH/2) - 1) 
#define BALL_INITIAL_I ( (WIDTH/2) ) 
#define BALL_INITIAL_J ( (HEIGHT/2) - 1)

typedef struct ball {
  int i;
  int j;
  char is_up;
  char is_left;
}Ball;

typedef struct plataform {
  int first_j;
}Plataform;

char ** returnGameMatrix() {
  char ** matrix = (char **) malloc(sizeof(char *) * HEIGHT);

  for (int i = 0; i < HEIGHT; i++) {
    matrix[i] = (char *) malloc(sizeof(char) * WIDTH); 
    matrix[i][0] = WALL;
    matrix[i][WIDTH - 1] = WALL;

    if (i == 0 || i == HEIGHT - 1) {
      for (int j = 1; j < WIDTH - 1; j++) {
        matrix[i][j] = ROOF_FLOOR;
      }
    }
  }

  // Set ball initial position
  matrix[BALL_INITIAL_I][BALL_INITIAL_J] = BALL;
  
  // Set plataform initial position
  for (int j = PLATAFORM_INITIAL_J; j < PLATAFORM_INITIAL_J + PLATAFORM_SIZE; j++) {
    matrix[PLATAFORM_I][j] = ROOF_FLOOR;
  }

  return matrix;
}

void moveBall(char ** matrix, Ball * b, Plataform * p, char * l) {
  matrix[b->i][b->j] = EMPTY;

  int new_i = b->is_up? b->i - 1 : b->i + 1,
      new_j = b->is_left ? b->j - 1 : b->j + 1;
  
  if (new_i == HEIGHT - 1) {
    (*l)--; 
    b->is_up = 0;
    b->is_left = 0;
    b->i = BALL_INITIAL_I;
    b->j = BALL_INITIAL_J;
    
    for (int j = p->first_j; j < p->first_j + PLATAFORM_SIZE; j++) {
      matrix[PLATAFORM_I][j] = EMPTY;
    }

    p->first_j = PLATAFORM_INITIAL_J;

    sleep(1);
    return;
  }

  if (matrix[new_i][new_j] != EMPTY) {
    switch (matrix[new_i][new_j]) {
      case ROOF_FLOOR:
        b->is_up = !(b->is_up);
        new_i = b->is_up ? new_i - 1 : new_i + 1;
        break;

      case WALL:
        b->is_left = !(b->is_left);
        new_j = b->is_left ? new_j - 1 : new_j + 1;
        break;
    }
  }
  
  b->i = new_i;
  b->j = new_j;

  matrix[b->i][b->j] = BALL;
}

void movePlataform(char ** matrix, Plataform * p, char arrow) {
  int new_j = arrow == LEFT_ARROW ? p->first_j - 1: p->first_j + 1;

  if ( (arrow == LEFT_ARROW && matrix[PLATAFORM_I][new_j] == EMPTY ) || (arrow == RIGHT_ARROW && matrix[PLATAFORM_I][new_j + PLATAFORM_SIZE - 1] == EMPTY)) {
    for (int j = new_j; j < new_j + PLATAFORM_SIZE; j++) {
      matrix[PLATAFORM_I][j] = ROOF_FLOOR;
    }

    if (arrow == LEFT_ARROW) {
      matrix[PLATAFORM_I][p->first_j + PLATAFORM_SIZE - 1] = EMPTY;
    }
    else {
      matrix[PLATAFORM_I][p->first_j] = EMPTY;
    }
    
    p->first_j = new_j;
  }
}

void play( void(*draw)(char ** matrix, int width, int height), char(*inputHandler)(void)) {
  char ** gameMatrix = returnGameMatrix(),
       lifes = LIFES;
  
  Ball * b = (Ball *) malloc(sizeof(Ball));
  b->is_up = 0;
  b->is_left = 0;
  b->i = BALL_INITIAL_I;
  b->j = BALL_INITIAL_J;
  
  Plataform * p = (Plataform *) malloc(sizeof(Plataform));
  p->first_j = PLATAFORM_INITIAL_J;

  while (lifes > 0) {
    moveBall(gameMatrix, b, p, &lifes);

    if (draw != NULL) {
      draw(gameMatrix, WIDTH, HEIGHT);
    }
    
    char pressedKey = inputHandler();

    if (pressedKey != 0) {
      movePlataform(gameMatrix, p, pressedKey);
    }
    usleep(99000);
  }
}
