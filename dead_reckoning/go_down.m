function robot = go_down(robot)
	position_robot = robot{2};
	direction_robot = robot{1};
	if direction_robot == 'N'
		direction_robot = 'S';
		position_robot = position_robot + [0, -1];
	elseif direction_robot == 'S'
		direction_robot = 'N';
		position_robot = position_robot + [0, 1];
	elseif direction_robot == 'L'
		direction_robot = 'O';
		position_robot = position_robot + [-1, 0];
	elseif direction_robot == 'O'
		direction_robot = 'L';
		position_robot = position_robot + [1, 0];
	end
	robot{1} = direction_robot;
	robot{2} = position_robot;
end