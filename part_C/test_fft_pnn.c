// Code to test fft and pnn

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sndfile.h>

#define sampling_rate 44100
#define size_in_seconds 5
int size_buffer = sampling_rate*size_in_seconds;
int size_in_fft;

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

int get_length_min(Signal *signal_1, Signal *signal_2){
	if (signal_1->size > signal_2->size){
		return signal_2->size;
	}
	else if(signal_1->size < signal_2->size){
		return signal_1->size;	
	}
	else{
		return signal_1->size;	
	}
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
    correlation = correlation/normalization_part;
    //printf("\ncorrelation: %f\n", correlation);
    return correlation;
}

int main(int argc, char *argv[]){
	
	char *audio_path = argv[1];
	Signal *audio_signal = get_audio_buffer(audio_path);

    // Printing contents of buffer
    /*int i;
    for (i = 0; i < signal_audio->size; ++i){
    	printf("\n%f\n", signal_audio->data[i]);
    }*/

    Signal *audio_fft = get_fft(audio_signal);

    // Printing contents of buffer
    /*int i;
    for (i = 0; i < fft_audio->size; ++i){
    	printf("%f\n", fft_audio->data[i]);
    }*/

    //printf("%d\n", fft_audio->size);

    //printf("\n%f\n", calculate_correlation_pearson(fft_audio, fft_audio));

    int length_min;
    int i;
	// --------------- PALMA --------------------
	// Palma 1
	Signal *signal_palma_1 = get_audio_buffer("../palmas/palma_2.wav");
	
	length_min = get_length_min(audio_signal, signal_palma_1);
	signal_palma_1->size = length_min;
	Signal *audio_signal_calc = (Signal *) malloc(sizeof(Signal));
	audio_signal_calc->size = length_min;

	float *buffer_a = (float *) malloc(length_min*sizeof(float));
	float *buffer_b = (float *) malloc(length_min*sizeof(float));
	for (i = 0; i < length_min; ++i){
		buffer_a[i] = audio_signal->data[i];
		buffer_b[i] = signal_palma_1->data[i];
		printf("%f\n", buffer_a[i] - buffer_b[i]);
	}
	audio_signal_calc->data = buffer_a;
	signal_palma_1->data = buffer_b;
	Signal *cepstrum_palma_1 = get_fft(signal_palma_1);
	Signal *cepstrum_audio_signal = get_fft(audio_signal_calc);
	float palma_1 = calculate_correlation_pearson(cepstrum_palma_1, cepstrum_audio_signal);
	printf("\n%f\n", palma_1);
	  
	return 0;
}