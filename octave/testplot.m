

x = 0:0.001:0.5;
y = rand(size(x,2),1);
y2 = rand(size(x,2),1);
figure;
subplot(2,2,1);
plot (x,y,"-r");
subplot(2,2,2);
plot (x,y2);
subplot(2,2,3);
plot (y,y2,"xr","markersize",10);
subplot(2,2,4);
hist(y,20);

