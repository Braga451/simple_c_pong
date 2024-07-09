#ifndef TERMINAL_HANDLER_H
  #define TERMINAL_HANDLER_H
  void enableRawMode();
  void draw(char ** matrix, int width, int height);
  char inputHandler(void);
#endif
