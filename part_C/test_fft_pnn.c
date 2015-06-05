// Code to test fft and pnn

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sndfile.h>

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
	float *x = audio_signal->data;
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
		fft_audio->data[l] = log(fft_audio->data[l]);
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
	free(signal_1->data);
	free(signal_2->data);
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
        //printf("\n%f\n", covariance);
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
    correlation = (correlation*1000000)/normalization_part;
    //printf("\ncorrelation: %f\n", correlation);
    return correlation;
}

float test_signals(char *path_signal_1, char *path_signal_2){
	Signal *signal_1 = get_audio_buffer(path_signal_1);
	Signal *signal_2 = get_audio_buffer(path_signal_2);
	get_length_min(signal_1, signal_2);
	Signal *cepstrum_signal_1 = get_fft(signal_1);
	free(signal_1);
	Signal *cepstrum_signal_2 = get_fft(signal_2);
	free(signal_2);
	float test = calculate_correlation_pearson(cepstrum_signal_1, cepstrum_signal_2);
	free(cepstrum_signal_1);
	free(cepstrum_signal_2);
	return test;

}

int main(int argc, char *argv[]){
	
	char *audio_path = argv[1];
	/*Signal *audio_signal = get_audio_buffer(audio_path);*/

    // Printing contents of buffer
    /*int i;
    for (i = 0; i < 10; ++i){
    	printf("%f\n", audio_signal->data[i]);
    }*/

    //Signal *audio_fft = get_fft(audio_signal);

    // Printing contents of buffer
    /*int i;
    for (i = 0; i < fft_audio->size; ++i){
    	printf("%f\n", fft_audio->data[i]);
    }*/

    //printf("%d\n", fft_audio->size);

    //printf("\n%f\n", calculate_correlation_pearson(fft_audio, fft_audio));


	// --------------- TESTE PALMA --------------------
	// Palma 1
	printf("palma 1:\n%f\n", test_signals("../palmas/palma_1.wav", "../palmas/palma_2.wav"));
	// Palma 2
	printf("palma 2:\n%f\n", test_signals("../palmas/palma_1.wav", "../palmas/palma_4.wav"));
	// --------------- ESTALO --------------------
	// Estalo 1
	printf("estalo 3:\n%f\n", test_signals("../palmas/palma_1.wav", "../palmas/palma_3.wav"));
	// Estalo 2
	printf("estalo 4:\n%f\n", test_signals("../palmas/palma_1.wav", "../palmas/estalo_2.wav"));
	// --------------- RUIDO --------------------
	// Ruido 1
	printf("ruido 5:\n%f\n", test_signals("../palmas/palma_1.wav", "../pindi_noise_2.wav"));
	// Ruido 2
	printf("ruido 6:\n%f\n", test_signals("../palmas/palma_1.wav", "../pindi_noise_4.wav"));
	// Ruido 3
	printf("ruido 7:\n%f\n", test_signals("../palmas/palma_1.wav", "../pindi_noise_8.wav"));
	// Ruido 4
	printf("ruido 8:\n%f\n", test_signals("../palmas/palma_1.wav", "../pindi_noise_9.wav"));
	// Ruido 5
	printf("ruido 9:\n%f\n", test_signals("../palmas/palma_1.wav", "../pindi_noise_10.wav"));

	printf("%s\n","---------------------------------");
	// --------------- TESTE ESTALO --------------------
	// Palma 1
	printf("palma 1: \n%f\n", test_signals("../palmas/estalo_3.wav", "../palmas/palma_2.wav"));
	// Palma 2
	printf("palma 2: \n%f\n", test_signals("../palmas/estalo_3.wav", "../palmas/palma_4.wav"));
	// --------------- ESTALO --------------------
	// Estalo 1
	printf("estalo 1: \n%f\n", test_signals("../palmas/estalo_3.wav", "../palmas/palma_3.wav"));
	// Estalo 2
	printf("estalo 2: \n%f\n", test_signals("../palmas/estalo_3.wav", "../palmas/estalo_2.wav"));
	// --------------- RUIDO --------------------
	// Ruido 1
	printf("ruido 1:\n%f\n", test_signals("../palmas/estalo_3.wav", "../pindi_noise_2.wav"));
	// Ruido 2
	printf("ruido 2:\n%f\n", test_signals("../palmas/estalo_3.wav", "../pindi_noise_4.wav"));
	// Ruido 3
	printf("ruido 3:\n%f\n", test_signals("../palmas/estalo_3.wav", "../pindi_noise_8.wav"));
	// Ruido 4
	printf("ruido 4:\n%f\n", test_signals("../palmas/estalo_3.wav", "../pindi_noise_9.wav"));
	// Ruido 5
	printf("ruido 5:\n%f\n", test_signals("../palmas/estalo_3.wav", "../pindi_noise_10.wav"));
	// --------------- RUIDO --------------------
	// Ruido 1
	printf("%s\n","---------------------------------");
	// --------------- TESTE ESTALO --------------------
	// Palma 1
	printf("%s\n","--------------- TESTE PALMA ------------------");
	//printf("\n%f\n", test_signals("6735/sound.wav", "440.wav"));
	printf("\n%f\n", test_signals("6735/sound.wav", "../palmas/palma_2.wav"));
	// Palma 2
	printf("\n%f\n", test_signals("6735/sound.wav", "../palmas/palma_4.wav"));
	// --------------- ESTALO --------------------
	// Estalo 1
	printf("%s\n","--------------- TESTE ESTALO ------------------");
	printf("\n%f\n", test_signals("6735/sound.wav", "../palmas/palma_3.wav"));
	// Estalo 2
	printf("\n%f\n", test_signals("6735/sound.wav", "../palmas/estalo_2.wav"));
	// --------------- RUIDO --------------------
	// Ruido 1
	printf("%s\n","--------------- TESTE RUIDO ------------------");
	printf("\n%f\n", test_signals("6735/sound.wav", "../pindi_noise_2.wav"));
	// Ruido 2
	printf("\n%f\n", test_signals("6735/sound.wav", "../pindi_noise_4.wav"));
	// Ruido 3
	printf("\n%f\n", test_signals("6735/sound.wav", "../pindi_noise_8.wav"));
	// Ruido 4
	printf("\n%f\n", test_signals("6735/sound.wav", "../pindi_noise_9.wav"));
	// Ruido 5
	printf("\n%f\n", test_signals("6735/sound.wav", "../pindi_noise_10.wav"));

  /*Signal *s1 = get_audio_buffer("../palmas/palma_1.wav");
  Signal *s2 = get_audio_buffer("440.wav");
  get_length_min(s2, s1);
  float test = calculate_covariance(s2, s1);
  float test_1 = calculate_covariance(s2, s2);
  float test_2 = calculate_covariance(s1, s1);
  printf("%f %f %f\n", test_1, test_2, test);*/

	return 0;
}