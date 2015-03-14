function [centroids, idx] = runkMeans(X, initial_centroids, ...
                                      max_iters, plot_progress)

if ~exist('plot_progress', 'var') || isempty(plot_progress)
    plot_progress = false;
end

% Plot the data if we are plotting progress
hold on;

% Initialize values
[m n] = size(X);
K = size(initial_centroids, 1);
centroids = initial_centroids;
previous_centroids = centroids;
idx = zeros(m, 1);

% Run K-Means
for i=1:max_iters
    
    % Output progress
    printf('K-Means iteration %d/%d...\n', i, max_iters);
    fflush(stdout); 
    
    % For each example in X, assign it to the closest centroid
    idx = findClosestCentroids(X, centroids);

    % Optionally, plot progress here
    if plot_progress
        plotProgresskMeans(X, centroids, previous_centroids, idx, K, i);
        previous_centroids = centroids;
        printf("Press enter to continue\n");
        fflush(stdout);
        pause
    end
    
    % Given the memberships, compute new centroids
    centroids = computeCentroids(X, idx, K);
end
end
