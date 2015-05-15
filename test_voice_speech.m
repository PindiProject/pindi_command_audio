clear all; close all;

disp('------- TEST 1 - TONE PURE x HUMAN VOICE -------');

% -----------------------------------
% TEST 1 - TONE PURE x HUMAN VOICE
signal_tone_440_Hz = wavread('440Hz.wav');
signal_tone_60_990_Hz = wavread('60_990Hz.wav');
signal_tone_990_2000_Hz = wavread('990_2000Hz.wav');
signal_voice_1 = wavread('teste_voz.wav');
signal_voice_2 = wavread('teste_voz_2.wav');


lengths = [length(signal_tone_440_Hz) length(signal_tone_440_Hz) ...
length(signal_tone_440_Hz) length(signal_voice_1) length(signal_voice_2)];

signal_voice_1 = signal_voice_1(1:min(lengths));
signal_voice_2 = signal_voice_2(1:min(lengths));
signal_tone_440_Hz = signal_tone_440_Hz(1:min(lengths));
signal_tone_60_990_Hz = signal_tone_60_990_Hz(1:min(lengths));
signal_tone_990_2000_Hz = signal_tone_990_2000_Hz(1:min(lengths));

%figure; plot(signal_voice_1)
%figure; plot(signal_tone_440_Hz)
%figure; plot(signal_tone_60_990_Hz)
%figure; plot(signal_tone_990_2000_Hz)

cepstrum_voice_1 = rceps(signal_voice_1);
cepstrum_voice_2 = rceps(signal_voice_2);
cepstrum_tone_440_Hz = rceps(signal_tone_440_Hz);
cepstrum_tone_60_990_Hz = rceps(signal_tone_60_990_Hz);
cepstrum_tone_990_2000_Hz = rceps(signal_tone_990_2000_Hz);

%figure; plot(cepstrum_voice_1)
%figure; plot(cepstrum_tone_440_Hz)
%figure; plot(cepstrum_tone_60_990_Hz)
%figure; plot(cepstrum_tone_990_2000_Hz)

disp('-------------------------- Testing with voice 1');

voice_1_tone_440_Hz = corrcoef(cepstrum_voice_1, cepstrum_tone_440_Hz);
voice_1_tone_60_990_Hz = corrcoef(cepstrum_voice_1, cepstrum_tone_60_990_Hz);
voice_1_tone_990_2000_Hz = corrcoef(cepstrum_voice_1, cepstrum_tone_990_2000_Hz);

abs(voice_1_tone_440_Hz(1,2))
abs(voice_1_tone_60_990_Hz(1,2))
abs(voice_1_tone_990_2000_Hz(1,2))

disp('-------------------------- Testing with voice 2');

voice_2_tone_440_Hz = corrcoef(cepstrum_voice_2, cepstrum_tone_440_Hz);
voice_2_tone_60_990_Hz = corrcoef(cepstrum_voice_2, cepstrum_tone_60_990_Hz);
voice_2_tone_990_2000_Hz = corrcoef(cepstrum_voice_2, cepstrum_tone_990_2000_Hz);

abs(voice_2_tone_440_Hz(1,2))
abs(voice_2_tone_60_990_Hz(1,2))
abs(voice_2_tone_990_2000_Hz(1,2))

disp('-------------------------- Testing with voice 1 and 2 together');

voice_2_voice_1 = corrcoef(cepstrum_voice_1, cepstrum_voice_2);
abs(voice_2_voice_1(1,2))

disp('------- TEST 2 - NOISE x HUMAN VOICE -------');
% -----------------------------------
% TEST 2 - NOISE x HUMAN VOICE
clear all; close all;
signal_voice_1 = wavread('teste_voz.wav');
signal_voice_2 = wavread('teste_voz_2.wav');
signal_noise = wavread('ruido.wav');

lengths = [length(signal_voice_1) length(signal_voice_2) length(signal_noise)];

signal_voice_1 = signal_voice_1(1:min(lengths));
signal_voice_2 = signal_voice_2(1:min(lengths));
signal_noise = signal_noise(1:min(lengths));

cepstrum_voice_1 = rceps(signal_voice_1);
cepstrum_voice_2 = rceps(signal_voice_2);
cepstrum_noise = rceps(signal_noise);

disp('-------------------------- Testing with voice 1 and noise together');

voice_1_noise = corrcoef(cepstrum_voice_1, cepstrum_noise);
abs(voice_1_noise(1,2))

disp('-------------------------- Testing with voice 2 and noise together');

voice_2_noise = corrcoef(cepstrum_voice_2, cepstrum_noise);
abs(voice_2_noise(1,2))

disp('-------------------------- Testing with voice 1 and 2 together');

voice_1_voice_2 = corrcoef(cepstrum_voice_1, cepstrum_voice_2);
abs(voice_1_voice_2(1,2))

disp('------- TEST 3 - VOICE COMMANDS x OTHERS SIGNALS -------');
% -----------------------------------
% TEST 3 - VOICE COMMANDS x OTHERS SIGNALS
clear all; close all;
signal_pindi_ligar_1 = wavread('pindi_ligar.wav');
signal_pindi_desligar_1 = wavread('pindi_desligar.wav');
signal_pindi_ligar_2 = wavread('pindi_ligar_2.wav');
signal_pindi_desligar_2 = wavread('pindi_desligar_2.wav');
signal_voice_1 = wavread('teste_voz.wav');
signal_voice_2 = wavread('teste_voz_2.wav');
signal_noise = wavread('ruido.wav');

disp(length(signal_pindi_ligar_1))
disp(length(signal_pindi_desligar_1))
disp(length(signal_pindi_ligar_2))
disp(length(signal_pindi_desligar_2))
disp(length(signal_voice_1))
disp(length(signal_voice_2))
disp(length(signal_noise))

lengths = [length(signal_pindi_ligar_1) length(signal_pindi_desligar_1) length(signal_pindi_ligar_2) ...
length(signal_pindi_desligar_2) length(signal_voice_1) length(signal_voice_2) length(signal_noise)];

min(lengths)

signal_pindi_ligar_1 = signal_pindi_ligar_1(1:min(lengths));
signal_pindi_desligar_1 = signal_pindi_desligar_1(1:min(lengths));
signal_pindi_ligar_2 = signal_pindi_ligar_2(1:min(lengths));
signal_pindi_desligar_2 = signal_pindi_desligar_2(1:min(lengths));
signal_voice_1 = signal_voice_1(1:min(lengths));
signal_voice_2 = signal_voice_2(1:min(lengths));
signal_noise = signal_noise(1:min(lengths));

cepstrum_pindi_ligar_1 = rceps(signal_pindi_ligar_1);
cepstrum_pindi_desligar_1 = rceps(signal_pindi_desligar_1);
cepstrum_pindi_ligar_2 = rceps(signal_pindi_ligar_2);
cepstrum_pindi_desligar_2 = rceps(signal_pindi_desligar_2);
cepstrum_voice_1 = rceps(signal_voice_1);
cepstrum_voice_2 = rceps(signal_voice_2);
cepstrum_noise = rceps(signal_noise);

disp('-------------------------- Testing with "ligar" and others signals');
disp('----- Voice 1');
ligar_1_voice_1 = corrcoef(cepstrum_pindi_ligar_1, cepstrum_voice_1);
ligar_2_voice_1 = corrcoef(cepstrum_pindi_ligar_2, cepstrum_voice_1);
abs(ligar_1_voice_1(1,2))
abs(ligar_2_voice_1(1,2))
disp('----- Voice 2');
ligar_1_voice_2 = corrcoef(cepstrum_pindi_ligar_1, cepstrum_voice_2);
ligar_2_voice_2 = corrcoef(cepstrum_pindi_ligar_2, cepstrum_voice_2);
abs(ligar_1_voice_2(1,2))
abs(ligar_2_voice_2(1,2))
disp('----- Noise');
ligar_1_noise = corrcoef(cepstrum_pindi_ligar_1, cepstrum_noise);
ligar_2_noise = corrcoef(cepstrum_pindi_ligar_2, cepstrum_noise);
abs(ligar_1_noise(1,2))
abs(ligar_2_noise(1,2))

disp('-------------------------- Testing with "desligar" and others signals');
disp('----- Voice 1');
desligar_1_voice_1 = corrcoef(cepstrum_pindi_desligar_1, cepstrum_voice_1);
desligar_2_voice_1 = corrcoef(cepstrum_pindi_desligar_2, cepstrum_voice_1);
abs(desligar_1_voice_1(1,2))
abs(desligar_2_voice_1(1,2))
disp('----- Voice 2');
desligar_1_voice_2 = corrcoef(cepstrum_pindi_desligar_1, cepstrum_voice_2);
desligar_2_voice_2 = corrcoef(cepstrum_pindi_desligar_2, cepstrum_voice_2);
abs(desligar_1_voice_2(1,2))
abs(desligar_2_voice_2(1,2))
disp('----- Noise');
desligar_1_noise = corrcoef(cepstrum_pindi_desligar_1, cepstrum_noise);
desligar_2_noise = corrcoef(cepstrum_pindi_desligar_2, cepstrum_noise);
abs(desligar_1_noise(1,2))
abs(desligar_2_noise(1,2))

disp('-------------------------- Testing with "ligar" and "ligar"');
ligar_1_ligar_2 = corrcoef(cepstrum_pindi_ligar_1, cepstrum_pindi_ligar_2);
abs(ligar_1_ligar_2(1,2))

disp('-------------------------- Testing with "desligar" and "desligar"');
desligar_1_desligar_2 = corrcoef(cepstrum_pindi_desligar_1, cepstrum_pindi_desligar_2);
abs(desligar_1_desligar_2(1,2))