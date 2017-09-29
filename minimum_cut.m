function [] = minimum_cut() 
    % Load the adjacency matrix
    W = load('karate-network.adj');
    
    % Diagonal degree matrix
    D = diag(sum(W));
    
    % Normalized Laplacian matrix
    L = D^(-1/2) * W * D^(-1/2);
    
    % Number of vertices
    N = size(W, 1);
    
    % Source vertex with label 1
    source = 1;
    
    % Sink vertex with label -1
    sink = N;
    
    % Initial labels
    Y0 = zeros(N, 1);
    Y0(source) = 1;
    Y0(sink) = -1;
    
    % Propagation
    alpha = 0.5;
    Y_inf = (1 - alpha) * (eye(N) - alpha * L)^(-1) * Y0;
    
    % Minimum cut
    label = zeros(N, 1);
    label(Y_inf >= 0) = 1;
    label(Y_inf < 0) = -1;
    
    A = (label == 1);
    B = (label == -1);
    min_cut = sum(sum(W(A, B)));
    
    fprintf('Minimum cut: %d\n', min_cut);
end