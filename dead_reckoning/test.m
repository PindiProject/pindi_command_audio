clear('all');
close 'all';

figure;
hold on;

map(50, 50) = 0;
robot = {};
% N = North, S = South, L = Lest, O = Oest
direction_robot = 'N';
position_robot = [1, 1];
robot{1} = direction_robot;
robot{2} = position_robot;
walk = round(rand*3);
while 1==1
   clf;
   
   if walk == 0
      disp('pra frente');
      
   elseif walk == 1
      disp('pra baixo');
   elseif walk == 2
      disp('pra esquerda');
   elseif walk == 3
      disp('pra direita');
   end
   
   position_robot = robot{2};
   map(position_robot(1), position_robot(2)) = 5;


   imagesc(map);
   drawnow;
   pause(0.01);
   walk = round(rand*10);
end