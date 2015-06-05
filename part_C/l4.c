/*

Example 4 - Simple sound recording

This example reads from the default PCM device
and writes to standard output for 5 seconds of data.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>

struct Signal {
  int size;
  float *data;
};
typedef struct Signal Signal;

Signal *get_audio_buffer(char *audio_path){
  // Opening file
  SF_INFO info;
  SNDFILE *sf;
  Signal *signal_audio = (Signal *) malloc(sizeof(Signal));
  sf = sf_open(audio_path, SFM_READ, &info);
  signal_audio->size = info.frames;
  if (sf == NULL){
      printf("Failed to open the file.\n");
      exit(-1);
  }

  // Puting contents of file in array
  signal_audio->data = (float *) malloc(signal_audio->size*sizeof(float));
  sf_read_float(sf, signal_audio->data, signal_audio->size);
  sf_close(sf);
  return signal_audio;
}

Signal *get_fft(Signal *audio_signal){
  Signal *fft_audio = (Signal *) malloc(sizeof(Signal));


  int n = (int) pow(2, (int) (logf(audio_signal->size)/logf(2)));
  fft_audio->size = n;
  fft_audio->data = (float *) malloc(fft_audio->size*sizeof(float));
  int m = (int) (logf(audio_signal->size)/logf(2));


  float cosine[n / 2];
  float sine[n / 2];
  int i;
  for (i = 0; i < n/2; i++) {
    cosine[i] = (float) cos(-2*M_PI*i/n);
    sine[i] = (float) sin(-2*M_PI*i/n);
  }

  // FFT in real case
  int j, k, n1, n2, a;
  float c, s, t1, t2;

  float *x = (float *) malloc(audio_signal->size*sizeof(float));
  memcpy(x, audio_signal->data, audio_signal->size);
  float *y = (float *) malloc(audio_signal->size*sizeof(float));

  // Put zeros in imaginary part
  int l;
  for (l = 0; l < n; l++) {
    y[l] = 0;
  }
  // Bit-reverse
  j = 0;
  n2 = n / 2;
  for (i = 1; i < n - 1; i++) {
    n1 = n2;
    while (j >= n1) {
      j = j - n1;
      n1 = n1 / 2;
    }
    j = j + n1;
    if (i < j) {
      t1 = x[i];
      x[i] = x[j];
      x[j] = t1;
      t1 = y[i];
      y[i] = y[j];
      y[j] = t1;
    }
  }
  // FFT
  n1 = 0;
  n2 = 1;
  for (i = 0; i < m; i++) {
    n1 = n2;
    n2 = n2 + n2;
    a = 0;
    for (j = 0; j < n1; j++) {
      c = cosine[a];
      s = sine[a];
      a += 1 << (m - i - 1);
      //printf("%d\n", a);
      for (k = j; k < n; k = k + n2) {
        t1 = c * x[k + n1] - s * y[k + n1];
        t2 = s * x[k + n1] + c * y[k + n1];
        x[k + n1] = x[k] - t1;
        y[k + n1] = y[k] - t2;
        x[k] = x[k] + t1;
        y[k] = y[k] + t2;
      }
    }
  }

  // ABS(FFT)
  float max_fft_audio = 0;
  for (l = 0; l < n; l++) {
    fft_audio->data[l] = (float) sqrt(x[l] * x[l] + y[l] * y[l]);
    // Adding log
    fft_audio->data[l] = logf(fft_audio->data[l]);
    if (fft_audio->data[l] > max_fft_audio) {
      max_fft_audio = fft_audio->data[l];
    }
  }

  //printf("\n%f\n", max_fft_audio);

  // Equalizing FFT
  for (l = 0; l < n; l++) {
    fft_audio->data[l] = fft_audio->data[l] / max_fft_audio;
    //printf("%f\n", fft_audio[l]);
  }

  return fft_audio;
}

float calculate_mean(Signal *buffer){
    //Calculating the mean array
    //printf("%d\n", buffer->size);
    int i;
    float sum = 0;
    for (i = 0; i < buffer->size; ++i){
        sum = sum + buffer->data[i];
      //printf("\n%f\n",sum);
    }
    sum = sum/buffer->size;
    //printf("%f\n", sum);
    return sum;
}

void get_length_min(Signal *signal_1, Signal *signal_2){
  /*float *x = (float *) malloc(audio_signal->size*sizeof(float));
  memcpy(x, audio_signal->data, audio_signal->size);*/

  int length_min;
  if (signal_1->size > signal_2->size){
    length_min = signal_2->size;
  }
  else if(signal_1->size < signal_2->size){
    length_min = signal_1->size;  
  }
  else{
    length_min = signal_1->size;  
  }
  signal_1->size = length_min;
  signal_2->size = length_min;
  float *buffer_a = (float *) malloc(length_min*sizeof(float));
  float *buffer_b = (float *) malloc(length_min*sizeof(float));
  int i;
  for (i = 0; i < length_min; ++i){
    buffer_a[i] = signal_1->data[i];
    buffer_b[i] = signal_2->data[i];
  }
  //free(signal_1->data);
  //free(signal_2->data);
  signal_1->data = buffer_a;
  signal_2->data = buffer_b;
}

float calculate_covariance(Signal *bufferX, Signal *bufferY){
    float meanX  = calculate_mean(bufferX);
    float meanY  = calculate_mean(bufferY);
    int i;
    float covariance = 0;
    for (i = 0; i < bufferX->size; ++i){
        covariance = covariance + (bufferX->data[i] - meanX)*(bufferY->data[i] - meanY);
    }
    //printf("\n%f\n", covariance);
    return covariance;
}

float calculate_correlation_pearson(Signal *bufferX, Signal *bufferY){
    float standart_deviationX = sqrt(calculate_covariance(bufferX, bufferX));
    //printf("\n%f\n", standart_deviationX);
    float standart_deviationY = sqrt(calculate_covariance(bufferY, bufferY));
    //printf("\n%f\n", standart_deviationY);
    float correlation = calculate_covariance(bufferX, bufferY);
    float normalization_part = standart_deviationX*standart_deviationY;
    correlation = (correlation*100)/normalization_part;
    //printf("\ncorrelation: %f\n", correlation);
    return correlation;
}

float test_signals(Signal *signal_1, Signal *signal_2){
  Signal *signal_1_1 = (Signal *) malloc(sizeof(Signal));
  float *s_1 = (float *) malloc(signal_1->size*sizeof(float));
  memcpy(s_1, signal_1->data, signal_1->size);
  signal_1_1->data = s_1;
  signal_1_1->size = signal_1->size;

  Signal *signal_2_2 = (Signal *) malloc(sizeof(Signal));
  float *s_2 = (float *) malloc(signal_2->size*sizeof(float));
  memcpy(s_2, signal_2->data, signal_2->size);
  signal_2_2->data = s_2;
  signal_2_2->size = signal_2->size;


  get_length_min(signal_1_1, signal_2_2);
  Signal *cepstrum_signal_1 = get_fft(signal_1_1);
  free(signal_1_1);
  Signal *cepstrum_signal_2 = get_fft(signal_2_2);
  free(signal_2_2);
  float test = calculate_correlation_pearson(cepstrum_signal_1, cepstrum_signal_2);
  test = sqrt(test*test);
  free(cepstrum_signal_1);
  free(cepstrum_signal_2);
  return test;
}

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
  float seconds = 5;
  int size = seconds*44100;

  /* Open PCM device for recording (capture). */
  rc = snd_pcm_open(&handle, "default",
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

  rc = snd_pcm_readi(handle, buffer, size);
  if (rc == -EPIPE) {
    /* EPIPE means overrun */
    //fprintf(stderr, "overrun occurred\n");
    snd_pcm_prepare(handle);
  } else if (rc < 0) {
    //fprintf(stderr, "error from read: %s\n", snd_strerror(rc));
  } else if (rc != (int)frames) {
    //fprintf(stderr, "short read, read %d frames\n", rc);
  }

  snd_pcm_drain(handle);
  snd_pcm_close(handle);

  Signal *signal_audio = (Signal *) malloc(sizeof(Signal));
  signal_audio->size = size;
  signal_audio->data = (float *) malloc(size*sizeof(float));
  int i;
  for (i = 0; i < size; ++i){
    signal_audio->data[i] = buffer[i];
  }

  //Signal *audio_fft = get_fft(signal_audio);

  //printf("%d\n", audio_fft->size);
  //for (i = 0; i < audio_fft->size; ++i){
    //printf("%f\n",audio_fft->data[i]);
  //}

  //printf("%s\n", "------ PALMAS e ESTALOS");
  //printf("\n440: %f\n", test_signals(signal_audio, get_audio_buffer("440.wav")));

  
  //float c = calculate_covariance(get_audio_buffer("440_rt_2.wav"), get_audio_buffer("440_rt_2.wav"));
  //printf("%f\n", c);

  /*Signal *s1 = get_audio_buffer("../palmas/palma_1.wav");
  Signal *s2 = get_audio_buffer("440.wav");
  get_length_min(signal_audio, s1);
  float test = calculate_covariance(signal_audio, s1);
  float test_1 = calculate_covariance(signal_audio, signal_audio);
  float test_2 = calculate_covariance(s1, s1);
  printf("%f %f %f\n", test_1, test_2, test);*/

  printf("\n440: %f\n", test_signals(signal_audio, get_audio_buffer("440.wav")));
  printf("\npalma 1: %f\n", test_signals(signal_audio, get_audio_buffer("../palmas/palma_1.wav")));
  printf("\npalma 2: %f\n", test_signals(signal_audio, get_audio_buffer("../palmas/palma_2.wav")));
  printf("\npalma 3: %f\n", test_signals(signal_audio, get_audio_buffer("../palmas/palma_4.wav")));
  printf("\nestalo 1: %f\n", test_signals(signal_audio, get_audio_buffer("../palmas/palma_3.wav")));
  printf("\nestalo 2: %f\n", test_signals(signal_audio, get_audio_buffer("../palmas/estalo_2.wav")));
  printf("\nestalo 3: %f\n", test_signals(signal_audio, get_audio_buffer("../palmas/estalo_3.wav")));
  printf("%s\n", "------ RUIDOS");
  printf("\nruido 1: %f\n", test_signals(signal_audio, get_audio_buffer("../pindi_noise_2.wav")));
  printf("\nruido 2: %f\n", test_signals(signal_audio, get_audio_buffer("../pindi_noise_4.wav")));
  printf("\nruido 3: %f\n", test_signals(signal_audio, get_audio_buffer("../pindi_noise_8.wav")));
  printf("\nruido 4: %f\n", test_signals(signal_audio, get_audio_buffer("../pindi_noise_9.wav")));
  printf("\nruido 5: %f\n", test_signals(signal_audio, get_audio_buffer("../pindi_noise_10.wav")));

  return 0;
}
