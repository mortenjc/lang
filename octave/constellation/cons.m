clear; close all; clc

A=0.4
S=99

V=-15.0*(2*3.1415)/360
rot = [cos(V), -sin(V); sin(V), cos(V)]

a = [A*randn(S,1)+1, A*randn(S,1)+3]*rot;
b = [A*randn(S,1)+1, A*randn(S,1)+1]*rot;
c = [A*randn(S,1)+3, A*randn(S,1)+1]*rot;
d = [A*randn(S,1)+3, A*randn(S,1)+3]*rot;
all_data = [a;b;c;d];

ps = 4;

plot(a(:,1), a(:,2),'bo', ...
     'MarkerSize', ps, 'LineWidth', 2.5);
hold on;
plot(b(:,1), b(:,2), 'go', ...
     'MarkerSize', ps, 'LineWidth', 2.5);
plot(c(:,1), c(:,2),'ro',...
     'MarkerSize', ps, 'LineWidth', 2.5);
plot(d(:,1), d(:,2),'yo', ...
     'MarkerSize', ps, 'LineWidth', 2.5);



max_iters=5
initial_centroids = [1,3; 1,1; 3,1; 3,3];
[centroids, idx] = runkMeans(all_data, initial_centroids, max_iters, true);

scatter(centroids(:,1),centroids(:,2), 22.0)
