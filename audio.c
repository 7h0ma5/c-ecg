#ifdef __APPLE__
  #include "audio-mac.c"
#else
  #include "audio-alsa.c"
#endif
