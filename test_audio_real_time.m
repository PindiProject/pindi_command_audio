clear('all');
close 'all';

while 1 == 1
	object_to_recorder = audiorecorder(44100,16,1);

	time_initial = clock;
	time_diferential = 0;
	time_final = 0;
	record(object_to_recorder);
	while time_diferential < 1.5 
		time_final = clock;
		time_diferential = etime(time_final, time_initial);
	end
	stop(object_to_recorder);
	command = get_voice_command(object_to_recorder.getaudiodata);
	if command == 1
		disp('LIGUEI');
	elseif command == -1
		disp('DESLIGUEI');
	elseif command == 0
		disp('Nenhum comando');
	elseif command == 999
		disp('Nem rodou o código');
	end

	
end