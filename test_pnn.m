clear('all');
close 'all';

%audio_signal = wavread('part_C/6735/sound.wav');
audio_signal = dlmread('part_C/output','\n');
% --------------- PALMA --------------------
% Palma 1
signal_palma_1 = wavread('part_C/440.wav');
lengths = [length(signal_palma_1) length(audio_signal)];
signal_palma_1 = signal_palma_1(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_palma_1 = log(abs(fft(signal_palma_1)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
palma_1 = corrcoef(cepstrum_palma_1, cepstrum_audio_signal);
palma_1 = abs(palma_1(1,2));
disp('Palma 1');
disp(palma_1);

% Palma 1
signal_palma_1 = wavread('part_C/440.wav');
lengths = [length(signal_palma_1) length(audio_signal)];
signal_palma_1 = signal_palma_1(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_palma_1 = log(abs(fft(signal_palma_1)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
palma_1 = corrcoef(cepstrum_palma_1, cepstrum_audio_signal);
palma_1 = abs(palma_1(1,2));
disp('Palma 1');
disp(palma_1);

% --------------- PALMA --------------------
% Palma 1
signal_palma_1 = wavread('palmas/palma_2.wav');
lengths = [length(signal_palma_1) length(audio_signal)];
signal_palma_1 = signal_palma_1(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_palma_1 = log(abs(fft(signal_palma_1)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
palma_1 = corrcoef(cepstrum_palma_1, cepstrum_audio_signal);
palma_1 = abs(palma_1(1,2));
disp('Palma 1');
disp(palma_1);

% Estalo
signal_estalo = wavread('palmas/palma_3.wav');
lengths = [length(signal_estalo) length(audio_signal)];
signal_estalo = signal_estalo(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_estalo = log(abs(fft(signal_estalo)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
estalo = corrcoef(cepstrum_estalo, cepstrum_audio_signal);
estalo = abs(estalo(1,2));
disp('Estalo');
disp(estalo);

% Estalo_2
signal_estalo_2 = wavread('palmas/estalo_2.wav');
lengths = [length(signal_estalo_2) length(audio_signal)];
signal_estalo_2 = signal_estalo_2(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_estalo_2 = log(abs(fft(signal_estalo_2)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
estalo_2 = corrcoef(cepstrum_estalo_2, cepstrum_audio_signal);
estalo_2 = abs(estalo_2(1,2));
disp('Estalo 2');
disp(estalo_2);

% Palma 2
signal_palma_2 = wavread('palmas/palma_4.wav');
lengths = [length(signal_palma_2) length(audio_signal)];
signal_palma_2 = signal_palma_2(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_palma_2 = log(abs(fft(signal_palma_2)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
palma_2 = corrcoef(cepstrum_palma_2, cepstrum_audio_signal);
palma_2 = abs(palma_2(1,2));
disp('Palma 2');
disp(palma_2);

% --------------- RUÍDOS --------------------
% noise 2
signal_noise_2 = wavread('pindi_noise_2.wav');
lengths = [length(signal_noise_2) length(audio_signal)];
signal_noise_2 = signal_noise_2(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_noise_2 = log(abs(fft(signal_noise_2)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
noise_2 = corrcoef(cepstrum_noise_2, cepstrum_audio_signal);
noise_2 = abs(noise_2(1,2));
disp('noise 2');
disp(noise_2);

% noise 4
signal_noise_4 = wavread('pindi_noise_4.wav');
lengths = [length(signal_noise_4) length(audio_signal)];
signal_noise_4 = signal_noise_4(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_noise_4 = log(abs(fft(signal_noise_4)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
noise_4 = corrcoef(cepstrum_noise_4, cepstrum_audio_signal);
noise_4 = abs(noise_4(1,2));
disp('noise 4');
disp(noise_4);

% noise 8
signal_noise_8 = wavread('pindi_noise_8.wav');
lengths = [length(signal_noise_8) length(audio_signal)];
signal_noise_8 = signal_noise_8(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_noise_8 = log(abs(fft(signal_noise_8)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
noise_8 = corrcoef(cepstrum_noise_8, cepstrum_audio_signal);
noise_8 = abs(noise_8(1,2));
disp('noise 8');
disp(noise_8);

% noise 9
signal_noise_9 = wavread('pindi_noise_9.wav');
lengths = [length(signal_noise_9) length(audio_signal)];
signal_noise_9 = signal_noise_9(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_noise_9 = log(abs(fft(signal_noise_9)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
noise_9 = corrcoef(cepstrum_noise_9, cepstrum_audio_signal);
noise_9 = abs(noise_9(1,2));
disp('noise 9');
disp(noise_9);

% noise 10
signal_noise_10 = wavread('pindi_noise_10.wav');
lengths = [length(signal_noise_10) length(audio_signal)];
signal_noise_10 = signal_noise_10(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_noise_10 = log(abs(fft(signal_noise_10)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
noise_10 = corrcoef(cepstrum_noise_10, cepstrum_audio_signal);
noise_10 = abs(noise_10(1,2));
disp('noise 10');
disp(noise_10);

% noise 11
signal_noise_11 = wavread('pindi_noise_11.wav');
lengths = [length(signal_noise_11) length(audio_signal)];
signal_noise_11 = signal_noise_11(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_noise_11 = log(abs(fft(signal_noise_11)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
noise_11 = corrcoef(cepstrum_noise_11, cepstrum_audio_signal);
noise_11 = abs(noise_11(1,2));
disp('noise 11');
disp(noise_11);

% noise 12
signal_noise_12 = wavread('pindi_noise_12.wav');
lengths = [length(signal_noise_12) length(audio_signal)];
signal_noise_12 = signal_noise_12(1:min(lengths));
audio_signal_calc = audio_signal(1:min(lengths));
cepstrum_noise_12 = log(abs(fft(signal_noise_12)));
cepstrum_audio_signal = log(abs(fft(audio_signal_calc)));
noise_12 = corrcoef(cepstrum_noise_12, cepstrum_audio_signal);
noise_12 = abs(noise_12(1,2));
disp('noise 12');
disp(noise_12);

