


FUNCTION BuildTree(root: int):
    DECLARE stack[300000]  // Static stack array
    SET top = 0            // Stack pointer
    // Push root node
    stack[top] = root
    INCREMENT top
    SET visited[root] = true
    WHILE top > 0:
        u = stack[top-1]    // Peek top element (no pop)
        processed = false
        // Process unvisited neighbors
        FOR i FROM 0 TO graphSizes[u]-1:
            v = graph[u][i]
            // Skip parent node
            IF v == father[u]:
                CONTINUE  
            IF NOT visited[v]:
                // Tree edge processing
                visited[v] = true
                father[v] = u
                depth[v] = depth[u] + 1
                // Push to stack for DFS
                stack[top] = v
                INCREMENT top
                SET processed = true
                BREAK  // Process one child per iteration
            ELSE IF u < v:
                // Back edge detection
                ADD Edge(u, v) TO back_edges
                INCREMENT bcnt
        // Pop current node if no children to process
        IF NOT processed:
            DECREMENT top  // Pop current node
