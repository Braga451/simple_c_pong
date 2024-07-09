#ifndef GAME_H
  #define GAME_H
  enum Entities {
    EMPTY = 0,
    WALL ,
    ROOF_FLOOR ,
    BALL
  };
  void play( void(*draw)(char ** matrix, int width, int height), char(*inputHandler)(void));

#endif
