% Copyright (c) 2011 Communications Research Laboratory, Ilmenau University of Technology
% 
% This file is part of LTE-Sim
%
% LTE-Sim is free software; you can redistribute it and/or modify
% it under the terms of the GNU General Public License version 3 as
% published by the Free Software Foundation;
%
% LTE-Sim is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with LTE-Sim; if not, see <http://www.gnu.org/licenses/>.
%
% Author: Florian Schlembach <florian.schlembach@tu-ilmenau.de>
% Author2: Jianshu Zhang <jianshu.zhang@tu-ilmenau.de>
% Author3: Yao Cheng <y.cheng@tu-ilmenau.de>
%  

clc;clear all;close all;

figure(1);

fontsize = 18;

%a=0;
%b=1732.05*3;
%c=866.025*3;
%d=1500*3;
%r = 1000*3;

cell_radius=2;
a=0;
b=1732.05*cell_radius;
c=866.025*cell_radius;
d=1500*cell_radius;
r=1000*cell_radius;

% read files
pos_x = load ('output/test_mob_POS_X');
pos_y = load ('output/test_mob_POS_Y');

% create a cell circle located at origin
grey = [0.2 0.2 0.2];
[x,y,z] = cylinder(r,200);
circle = plot(x(1,:),y(1,:),'Color',grey,'LineWidth',2);hold on;
[x1,y1,z1] = cylinder(r,200);
x1=x1+a;
y1=y1+b;
circle1 = plot(x1(1,:),y1(1,:),'Color',grey,'LineWidth',2);hold on;
[x1,y1,z1] = cylinder(r,200);
x1=x1-d;
y1=y1+c;
circle1 = plot(x1(1,:),y1(1,:),'Color',grey,'LineWidth',2);hold on;
[x1,y1,z1] = cylinder(r,200);
x1=x1-d;
y1=y1-c;
circle1 = plot(x1(1,:),y1(1,:),'Color',grey,'LineWidth',2);hold on;
[x1,y1,z1] = cylinder(r,200);
x1=x1+a;
y1=y1-b;
circle1 = plot(x1(1,:),y1(1,:),'Color',grey,'LineWidth',2);hold on;
[x1,y1,z1] = cylinder(r,200);
x1=x1+d;
y1=y1-c;
circle1 = plot(x1(1,:),y1(1,:),'Color',grey,'LineWidth',2);hold on;
[x1,y1,z1] = cylinder(r,200);
x1=x1+d;
y1=y1+c;
circle1 = plot(x1(1,:),y1(1,:),'Color',grey,'LineWidth',2);hold on;
% create hexagon
t=linspace(0,2*pi,7);
ahx=a+r*cos(t);
bhx=a+r*sin(t);
plot(ahx,bhx,'-.','Color',grey,'LineWidth',2); hold on;
ahx=a+r*cos(t);
bhx=b+r*sin(t);
plot(ahx,bhx,'-.','Color',grey,'LineWidth',2); hold on;
ahx=-d+r*cos(t);
bhx=c+r*sin(t);
plot(ahx,bhx,'-.','Color',grey,'LineWidth',2); hold on;
ahx=-d+r*cos(t);
bhx=-c+r*sin(t);
plot(ahx,bhx,'-.','Color',grey,'LineWidth',2); hold on;
ahx=a+r*cos(t);
bhx=-b+r*sin(t);
plot(ahx,bhx,'-.','Color',grey,'LineWidth',2); hold on;
ahx=d+r*cos(t);
bhx=-c+r*sin(t);
plot(ahx,bhx,'-.','Color',grey,'LineWidth',2); hold on;
ahx=d+r*cos(t);
bhx=c+r*sin(t);
plot(ahx,bhx,'-.','Color',grey,'LineWidth',2); hold on;



% plot paths
paths = plot (pos_x, pos_y,'LineWidth',2);
start = plot(pos_x(1,:),pos_y(1,:),'g>','LineWidth',3); % plot starting points
stop = plot(pos_x(end,:),pos_y(end,:),'rO','LineWidth',3); % plot end points

% plot eNB position
enb = plot ([0 0],'kx','LineWidth',14); 
%enb = plot ([0 , 1732.05],'kx','LineWidth',14);
%enb = plot ([-1500.0 ,866.025],'kx','LineWidth',14);
%enb = plot ([-1500.0 ,-866.025],'kx','LineWidth',14);
%enb = plot ([0 ,-1732.05],'kx','LineWidth',14);
%enb = plot ([1500.0 ,-866.025],'kx','LineWidth',14);
%enb = plot ([1500.0 ,866.025],'kx','LineWidth',14);

axis([-r*3 r*3 -r*3 r*3],'equal');
title('Manhattan Mobility-model, single-cell scenario','FontSize',fontsize);
xlabel('X-coordinate','FontSize',fontsize);ylabel('Y-coordinate','FontSize',fontsize);set(gca,'FontSize',fontsize);
legend([circle(1) enb(1) paths(1) start(1) stop(1)],{'cell edge','eNB position','realizations paths','starting point','end point'},4);
grid on;

% remove margin around plot
% set(gca,'units','centimeters')
% pos = get(gca,'Position');
% ti = get(gca,'TightInset');

% set(gcf, 'PaperUnits','centimeters');
% set(gcf, 'PaperSize', [pos(3)+ti(1)+ti(3) pos(4)+ti(2)+ti(4)]);
% set(gcf, 'PaperPositionMode', 'manual');
% set(gcf, 'PaperPosition',[0 0 pos(3)+ti(1)+ti(3) pos(4)+ti(2)+ti(4)]);


% print all this stuff to an eps figure
% set(gcf, 'renderer', 'painters');
% print(gcf, '-depsc2', 'mob.eps');


