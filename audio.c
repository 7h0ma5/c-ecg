#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include "audio.h"
#include "analyze.h"

#define SAMPLE_RATE 44100
#define FRAME_SIZE 512

int frequency;
short* buffer;
snd_pcm_t* handle;

void audio_init() {
  // Open PCM device for recording (capture).
  int rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0) {
    fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
    exit(1);
  }

  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_t* params;
  snd_pcm_hw_params_malloc(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* noninterleaved mode */
  snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

  /* One channel (mono) */
  snd_pcm_hw_params_set_channels(handle, params, 1);

  /* 44100 bits/second sampling rate (CD quality) */
  int dir;
  unsigned int sample_rate = SAMPLE_RATE;
  snd_pcm_hw_params_set_rate_near(handle, params, &sample_rate, &dir);

  /* Set period size to 512 frames. */
  snd_pcm_uframes_t frames = FRAME_SIZE;
  snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames, &dir);
  buffer = (short*)malloc(frames * 2); /* 2 bytes/sample */
}

int audio_read() {
  int rc = snd_pcm_readi(handle, buffer, FRAME_SIZE);
  if (rc == -EPIPE) {
    fprintf(stderr, "overrun occurred\n");
    snd_pcm_prepare(handle);
  }
  else if (rc < 0) {
    fprintf(stderr, "error from read: %s\n", snd_strerror(rc));
  }
  else if (rc != FRAME_SIZE) {
    fprintf(stderr, "short read, read %d frames\n", rc);
  }

  return find_frequency(buffer, FRAME_SIZE);
}

void audio_close() {
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);
}
