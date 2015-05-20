function command = get_voice_command(audio_signal)

	signal_pindi_ligar_1 = wavread('pindi_ligar.wav');
	signal_pindi_desligar_1 = wavread('pindi_desligar.wav');
	signal_pindi_ligar_2 = wavread('pindi_ligar_2.wav');
	signal_pindi_ligar_3 = wavread('pindi_ligar_3.wav');
	signal_pindi_desligar_2 = wavread('pindi_desligar_2.wav');
	signal_pindi_desligar_3 = wavread('pindi_desligar_3.wav');
	signal_voice_1 = wavread('teste_voz.wav');
	signal_voice_2 = wavread('teste_voz_2.wav');
	signal_noise = wavread('ruido.wav');
	signal_noise_2 = wavread('ruido_2.wav');
	signal_noise_3 = wavread('ruido_3.wav');
	signal_noise_4 = wavread('ruido_4.wav');
	signal_noise_5 = wavread('ruido_5.wav');

	lengths = [length(signal_pindi_ligar_1) length(signal_pindi_desligar_1) length(signal_pindi_ligar_2) ...
length(signal_pindi_desligar_2) length(signal_voice_1) length(signal_voice_2) length(signal_noise) ...
 length(signal_noise_2) length(signal_noise_3) length(signal_pindi_ligar_3) length(signal_pindi_desligar_3) ...
 length(signal_noise_4) length(signal_noise_5)];

	audio_signal = audio_signal(1:min(lengths));
	signal_pindi_ligar_1 = signal_pindi_ligar_1(1:min(lengths));
	signal_pindi_desligar_1 = signal_pindi_desligar_1(1:min(lengths));
	signal_pindi_ligar_2 = signal_pindi_ligar_2(1:min(lengths));
	signal_pindi_ligar_3 = signal_pindi_ligar_3(1:min(lengths));
	signal_pindi_desligar_2 = signal_pindi_desligar_2(1:min(lengths));
	signal_pindi_desligar_3 = signal_pindi_desligar_3(1:min(lengths));
	signal_voice_1 = signal_voice_1(1:min(lengths));
	signal_voice_2 = signal_voice_2(1:min(lengths));
	signal_noise = signal_noise(1:min(lengths));
	signal_noise_2 = signal_noise_2(1:min(lengths));
	signal_noise_3 = signal_noise_3(1:min(lengths));
	signal_noise_4 = signal_noise_4(1:min(lengths));
	signal_noise_5 = signal_noise_5(1:min(lengths));

	cepstrum_audio_signal = log(abs(fft(audio_signal)));
	cepstrum_pindi_ligar_1 = log(abs(fft(signal_pindi_ligar_1)));
	cepstrum_pindi_desligar_1 = log(abs(fft(signal_pindi_desligar_1)));
	cepstrum_pindi_ligar_2 = log(abs(fft(signal_pindi_ligar_2)));
	cepstrum_pindi_ligar_3 = log(abs(fft(signal_pindi_ligar_3)));
	cepstrum_pindi_desligar_2 = log(abs(fft(signal_pindi_desligar_2)));
	cepstrum_pindi_desligar_3 = log(abs(fft(signal_pindi_desligar_3)));
	cepstrum_voice_1 = log(abs(fft(signal_voice_1)));
	cepstrum_voice_2 = log(abs(fft(signal_voice_2)));
	cepstrum_noise = log(abs(fft(signal_noise)));
	cepstrum_noise_2 = log(abs(fft(signal_noise_2)));
	cepstrum_noise_3 = log(abs(fft(signal_noise_3)));
	cepstrum_noise_4 = log(abs(fft(signal_noise_4)));
	cepstrum_noise_5 = log(abs(fft(signal_noise_5)));

	% comandos para ligar
	ligar_1 = corrcoef(cepstrum_pindi_ligar_1, cepstrum_audio_signal);
	ligar_1 = abs(ligar_1(1,2));
	ligar_2 = corrcoef(cepstrum_pindi_ligar_2, cepstrum_audio_signal);
	ligar_2 = abs(ligar_2(1,2));
	ligar_3 = corrcoef(cepstrum_pindi_ligar_3, cepstrum_audio_signal);
	ligar_3 = abs(ligar_3(1,2));

	% comandos para desligar
	desligar_1 = corrcoef(cepstrum_pindi_desligar_1, cepstrum_audio_signal);
	desligar_1 = abs(desligar_1(1,2));
	desligar_2 = corrcoef(cepstrum_pindi_desligar_2, cepstrum_audio_signal);
	desligar_2 = abs(desligar_2(1,2));
	desligar_3 = corrcoef(cepstrum_pindi_desligar_3, cepstrum_audio_signal);
	desligar_3 = abs(desligar_3(1,2));

	% comandos para nada
	voice_1 = corrcoef(cepstrum_voice_1, cepstrum_audio_signal);
	voice_1 = abs(voice_1(1,2));
	voice_2 = corrcoef(cepstrum_voice_2, cepstrum_audio_signal);
	voice_2 = abs(voice_2(1,2));
	noise = corrcoef(cepstrum_noise, cepstrum_audio_signal);
	noise = abs(noise(1,2));
	noise_2 = corrcoef(cepstrum_noise_2, cepstrum_audio_signal);
	noise_2 = abs(noise_2(1,2));
	noise_3 = corrcoef(cepstrum_noise_3, cepstrum_audio_signal);
	noise_3 = abs(noise_3(1,2));
	noise_4 = corrcoef(cepstrum_noise_4, cepstrum_audio_signal);
	noise_4 = abs(noise_4(1,2));
	noise_5 = corrcoef(cepstrum_noise_5, cepstrum_audio_signal);
	noise_5 = abs(noise_5(1,2));

	commands = [ligar_1 ligar_2 desligar_1 desligar_2 voice_1 voice_2 noise noise_2 noise_3 ligar_3 desligar_3 ...
	noise_4 noise_5];

	command = 999;
	if find(commands == max(commands)) == 1 || find(commands == max(commands)) == 2 ...
		|| find(commands == max(commands)) == 10
		command = 1;
	elseif find(commands == max(commands)) == 3 || find(commands == max(commands)) == 4 ...
		|| find(commands == max(commands)) == 11
		command = -1;
	elseif find(commands == max(commands)) == 5 || find(commands == max(commands)) == 6 ... 
		|| find(commands == max(commands)) == 7 || find(commands == max(commands)) == 8 ...
		|| find(commands == max(commands)) == 9 || find(commands == max(commands)) == 12 ...
		|| find(commands == max(commands)) == 13
		command = 0;
	end
end