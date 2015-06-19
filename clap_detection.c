/*

Code to detect clap to activate Pindi.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>

int main() {
  long loops;
  int rc;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int dir;
  snd_pcm_uframes_t frames;
  float *buffer;
  int *float_buf;
  float seconds = 1;
  int size = seconds*44100;
  float *correlation_turn;
  float *correlation_do_nothing;
  int flag_turn = -1;
  int flag_old = 1;

  /* Open PCM device for recording (capture). */
  rc = snd_pcm_open(&handle, "plughw:1",
                    SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0) {
    fprintf(stderr, 
            "unable to open pcm device: %s\n",
            snd_strerror(rc));
    exit(1);
  }
  
  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params,
                      SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_FLOAT);

  /* One channels (mono) */
  snd_pcm_hw_params_set_channels(handle, params, 1);

  /* 44100 bits/second sampling rate (CD quality) */
  val = 44100;
  snd_pcm_hw_params_set_rate_near(handle, params, 
                                  &val, &dir);

  /* Set period size to 32 frames. */
  frames = 32;
  snd_pcm_hw_params_set_period_size_near(handle, 
                              params, &frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames, &dir);
  buffer = (float *) malloc(size*sizeof(float));

  /* We want to loop for 5 seconds */
  snd_pcm_hw_params_get_period_time(params, &val, &dir);

  while(1){
    rc = snd_pcm_readi(handle, buffer, size);
    if (rc == -EPIPE) {
      snd_pcm_prepare(handle);
    }

    int i;
    int window_small_size = (int) (44100*0.0025);

    int slot;
    int slot_small;
    float sum_energy = 0;
    for(slot = 0; slot < size; slot = slot + window_small_size){
      sum_energy = 0;
      for (slot_small = slot; slot_small < slot + window_small_size && 
        slot_small < size; ++slot_small){
        sum_energy = sum_energy + sqrt(buffer[slot_small]*buffer[slot_small]);
      }
      if (sum_energy > 70.0){
        flag_turn = flag_turn*(-1);
        if (flag_turn == 1){
            printf("%s\n", "YEAH YEAH to ligado");
            break;
          }else if (flag_turn == -1){
            printf("%s\n", "Ráa to desligado");
            break;
          }
      }
    }
  }

  snd_pcm_drain(handle);
  snd_pcm_close(handle);

  return 0;
}
