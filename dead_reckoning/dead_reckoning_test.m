clear 'all';
close 'all';

%set map
map(50, 50) = 0;

% set car
map(25,25) = 5;

% set walls
map(25,27) = 1;
map(24,26) = 1;
map(25,27) = 1;
map(26,26) = 1;

%disp(go_foward([0,0]));
%disp(go_rigth([0,0]));
%disp(go_left([0,0]));
%disp(go_down([0,0]));

direction_up = 0;
direction_down = 1;
direction_rigth = 2;
direction_left = 3;
direction_car = direction_up;

surf(map);
tic

%init navigation
initial_time = clock;
delta_time = 0;
initial_position = [25 25];
while delta_time <= 10
	previous_position = initial_position;
	position = go_foward(initial_position);
	
	map(position) = 5;
	if map(position(1), position(2)) == 1
		position = go_down(position);
		position = go_rigth(position);
		if map(position(1), position(2)) == 1
			position = go_left(position);
			position = go_left(position);
			if map(position(1), position(2)) == 1
				position = go_rigth(position);
				position = go_down(position);
			end
		end
	end
	delta_time = etime(clock, initial_time);
end

toc