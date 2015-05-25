clear('all');
close 'all';

figure;
hold on;

map(50, 50) = 0;
map(1, 1) = 5;
robot = {};
% N = North, S = South, L = Lest, O = Oest
direction_robot = 'N';
position_robot = [1, 1];
robot{1} = direction_robot;
robot{2} = position_robot;
while 1==1
   clf;
   % code dead reckoning
   %while position_robot(1) > 50 || position_robot(2) > 50 || ...
   	%position_robot(1) <= 0 || position_robot(2) <= 0
   	robot = move(robot);
   %end
   position_robot = robot{2};
   while position_robot(1) > 50 || position_robot(2) > 50 || ...
   		position_robot(1) <= 0 || position_robot(2) <= 0
   	    robot = move(robot);
   	    position_robot = robot{2}
   end
   position_robot = robot{2};
   map(position_robot(1), position_robot(2)) = 5;


   imagesc(map);
   drawnow;
   pause(0.01);
end