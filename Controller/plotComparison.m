num = xlsread('C:\Users\Nicolai\OneDrive\FH Mechatronik\2.Semester\Embedded Systems 2\Systems Design\Project\Controller\ReglerSprungAntwort.xlsx');
num = num';
num = num(1:200);
figure(1);
plot(linspace(0,size(num,2)*0.015,size(num,2)), num);
hold on;
plot(simout);