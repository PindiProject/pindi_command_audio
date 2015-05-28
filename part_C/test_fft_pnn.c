// Code to test fft and pnn

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sndfile.h>

#define sampling_rate 44100
#define size_in_seconds 5
int size_buffer = sampling_rate*size_in_seconds;
int size_in_fft;

float *get_audio_buffer(char *audio_path){
	// Opening file
	SF_INFO info;
    SNDFILE *sf;
    sf = sf_open(audio_path, SFM_READ, &info);
    if (sf == NULL){
        printf("Failed to open the file.\n");
        exit(-1);
    }

    // Puting contents of file in array
    float *buffer = (float *) malloc(size_buffer*sizeof(float));
    sf_read_float(sf, buffer, size_buffer);
    sf_close(sf);
	return buffer;
}

float *get_fft(float *audio_signal){
	int n = (int) pow(2, (int) (logf(size_buffer)/logf(2)));
	size_in_fft = n;
	int m = (int) (logf(size_buffer)/logf(2));


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
	float *x = audio_signal;
	float y[size_buffer];

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
	float fft_audio[n];
	for (l = 0; l < n; l++) {
		fft_audio[l] = (float) sqrt(x[l] * x[l] + y[l] * y[l]);
		// Adding log
		fft_audio[l] = log(fft_audio[l]);
		if (fft_audio[l] > max_fft_audio) {
			max_fft_audio = fft_audio[l];
		}
	}

	//printf("\n%f\n", max_fft_audio);

	// Equalizing FFT
	for (l = 0; l < n; l++) {
		fft_audio[l] = fft_audio[l] / max_fft_audio;
		//printf("%f\n", fft_audio[l]);
	}

	return fft_audio;
}

float calculate_mean(float *buffer){
    //Calculating the mean array
    int i;
    float sum = 0;
    for (i = 0; i < size_in_fft; ++i){
        sum = sum + buffer[i];
      //printf("\n%f\n",sum);
    }
    sum = sum/size_in_fft;
    //printf("%f\n", sum);
    return sum;
}

float calculate_covariance(float *bufferX, float *bufferY){
    float meanX  = calculate_mean(bufferX);
    float meanY  = calculate_mean(bufferY);
    int i;
    float covariance = 0;
    for (i = 0; i < size_in_fft; ++i){
        covariance = covariance + (bufferX[i] - meanX)*(bufferY[i] - meanY);
    }
    //printf("\n%f\n", covariance);
    return covariance;
}

float calculate_correlation_pearson(float *bufferX, float *bufferY){
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
	float *buffer = get_audio_buffer(audio_path);

    /*// Printing contents of buffer
    int i;
    for (i = 0; i < size_buffer; ++i){
    	printf("\n%f\n", buffer[i]);
    }*/

    float *fft_audio = get_fft(buffer);

    // Printing contents of buffer
    /*int i;
    for (i = 0; i < size_in_fft; ++i){
    	printf("%f\n", fft_audio[i]);
    }*/

    printf("\n%f\n", calculate_correlation_pearson(fft_audio, fft_audio));
	  
	return 0;
}