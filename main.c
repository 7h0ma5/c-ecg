#include <stdio.h>
#include "audio.h"
#include "graphics.h"

int main(int argc, char** argv) {
  audio_init();
  graphics_init(argc, argv);
  graphics_loop();
  return 0;
}
