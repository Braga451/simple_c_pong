#include "terminal_handler.h"
#include <termio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct termios defaultTerminal;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &defaultTerminal);
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &defaultTerminal);
  atexit(disableRawMode);

  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_lflag &= ~(ECHO | ICANON);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void draw(char ** matrix, int width, int height) {
  printf("\033[2J\033[H");
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      char to_print;

      switch (matrix[i][j]) {
        case 0:
          to_print = ' ';
          break;

        case 1:
          to_print = '|';
          break;

        case 2:
          to_print = '-';
          break;

        case 3:
          to_print = '.';
          break;
      }

      printf("%c ", to_print);
    }
    puts("");
  }
}

char inputHandler(void) {
  char input;
  read(STDIN_FILENO, &input, 1);
  
  if (iscntrl(input)) {
    read(STDIN_FILENO, NULL, 1);
    
    char key;
    read(STDIN_FILENO, &key, 1);

    switch (key) {
      case 'D':
        return 1;

      case 'C':
        return 2;
    }
  }

  return 0;
}
