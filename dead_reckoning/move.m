function robot = move(robot)
   disp('pra frente');
   robot = go_foward(robot);
   position_robot = robot{2};
   	if position_robot(1) > 50 || position_robot(2) > 50 || ...
   		position_robot(1) <= 0 || position_robot(2) <= 0
   		disp('pra frente');
	   robot = go_rigth(robot);
	   position_robot = robot{2};

   	end

end