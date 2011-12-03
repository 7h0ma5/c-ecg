#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "audio.h"
#include "analyze.h"
#include "graphics.h"

#define SAMPLE_RATE 44100
#define SAMPLE_TYPE
#define FRAMES_PER_BUFFER 512

PaStream* stream = 0;

static int callback(const void* inputBuffer, void* outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlags, void* userData) {

   int* out = (int*)outputBuffer;
   const int* in = (const int*)inputBuffer;

   int freq = find_frequency(in, FRAMES_PER_BUFFER);
   graphics_frequency(freq);

   memset(out, 0, sizeof(int)*512);

   return paContinue;
}

void audio_init() {
  PaStreamParameters inputP, outputP;
  PaError err;

  err = Pa_Initialize();
  if (err != paNoError) goto error;

  inputP.device = Pa_GetDefaultInputDevice();
  inputP.channelCount = 2;
  inputP.sampleFormat = paInt32;
  inputP.suggestedLatency = Pa_GetDeviceInfo(inputP.device)->defaultLowInputLatency;
  inputP.hostApiSpecificStreamInfo = NULL;

  outputP.device = Pa_GetDefaultOutputDevice();
  outputP.channelCount = 2;
  outputP.sampleFormat = paInt32;
  outputP.suggestedLatency = Pa_GetDeviceInfo(outputP.device)->defaultLowOutputLatency;
  outputP.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream(&stream, &inputP, &outputP,
                      SAMPLE_RATE, FRAMES_PER_BUFFER, 0,
                      callback, NULL);

  if (err != paNoError) goto error;

  err = Pa_StartStream(stream);
  if(err == paNoError) return;

 error:
  Pa_Terminate();
  fprintf(stderr, "audio initialization failed:\n");
  fprintf(stderr, "  %s\n", Pa_GetErrorText(err));
  exit(0);
}

void audio_close() {
  Pa_CloseStream(stream);
  Pa_Terminate();
}
