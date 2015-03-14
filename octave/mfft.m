t=2*pi; 
res = 0.01;
x = 0:res:t;
N=size(x,2);

y=sin(x);
y2 = sin(x)+0.5*sin(4*x)-sin(8*x);
y2n= y2 +rand(1,N)*6;
y3=abs(fft(y2));
y3n=abs(fft(y2n));

xf=[0:N-1]*2*pi/res/N;

figure;
subplot(2,2,1)
plot(x,y2)
xlabel("t")

subplot(2,2,2)
plot(x,y2n)
xlabel("t")

subplot(2,2,3)
plot(xf(1:20),y3(1:20),"r")
xlabel('f(Hz)')

subplot(2,2,4)
plot(xf(1:20),y3n(1:20),"r")
xlabel('f(Hz)')
