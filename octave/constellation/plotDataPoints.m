function plotDataPoints(X, idx, K)
% Create palette
palette = hsv(K+1);
colors = palette(idx, :);

printf("before scatter command\n");
fflush(stdout);

% Plot the data
scatter(X(:,1), X(:,2), 3, colors);
%scatter(X(:,1), X(:,2), 15);

end
