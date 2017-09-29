function [] = vertex_embedding()
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
    alpha = 0.8;
    Y_inf = (1 - alpha) * (eye(N) - alpha * L)^(-1) * Y0;
    
    % Minimum cut
    label = zeros(N, 1);
    label(Y_inf >= 0) = 1;
    label(Y_inf < 0) = -1;
    
    % Initial vertex embedding
    Z0 = rand(N, 2) / 5.0;
    Z0(sink, 1) = -1;
    Z0(sink, 2) = 0;
    Z0(source, 1) = 1;
    Z0(source, 2) = 0;
    Z = Z0;
    
    % Writing to a video
    writerObj = VideoWriter('vertex-embedding.avi');
    writerObj.FrameRate = 1;
    open(writerObj);
    
    widthImage = 640;
    heightImage = 480;
    
    % Number of iterations
    nIterations = 20;
    
    % Looping
    for iter = 1 : nIterations
        % Draw the vertex embedding
        for u = 1 : N - 1
            for v = u + 1 : N
                if W(u, v) > 0
                    plot([Z(u, 1), Z(v, 1)], [Z(u, 2), Z(v, 2)], 'g--');
                    hold on;
                end
            end
        end
        hold on;
        plot(Z(label == 1, 1), Z(label == 1, 2), 'r+');
        plot(Z(label == -1, 1), Z(label == -1, 2), 'bo');
        title(['Number of iterations: ', num2str(iter)]);
        drawnow;
        hold off;
        
        % Save to the video
        F = getframe(gcf);
        [X, ~] = frame2im(F);
        writeVideo(writerObj, imresize(X, [heightImage widthImage]));
        
        % Update the vertex embedding
        Z_next = alpha * L * Z + (1 - alpha) * Z0;
        Z = Z_next;
    end
    
    close(writerObj);
end