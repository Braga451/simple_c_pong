#include <game.h>
#include <terminal_handler.h>
#include <stdio.h>

int main(int argc, char **argv) {
  enableRawMode();
  play(&draw, &inputHandler);
}
