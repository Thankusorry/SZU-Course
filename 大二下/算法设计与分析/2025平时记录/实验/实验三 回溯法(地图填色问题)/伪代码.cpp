bool csp_check(int current, int c, RemovedColors* rc):
    // 初始化队列
    queue ← array of size n+1
    front ← 0, rear ← 0

    // Step 1: 向前探测删除颜色
    for each neighbor in graph[current]:
        // 未涂色 + 可涂节点颜色
        if neighbor is uncolored and color c exists in its domain:
            // 删除元素
            Remove_Color();
// 分支1：无颜色可选
if neighbor
    's domain becomes empty: free queue return False // Conflict detected
        // 分治2:只剩一种颜色
        if neighbor's domain becomes single-value: enqueue neighbor to queue

        // Step 2: 处理单色队列
        while queue not empty : node ← dequeue from queue

                                    // 找到仅存的颜色
                                    single_color ← find remaining color in node's domain
                                // 单色探测删除颜色
                                Remove_Color1();
    free queue
    return True  // 向前探测成功

csp(int current):
    
    // 涂色完成
    if current > n :
        cnt++
        return

    // 填色
    for c from 1 to colornum:
        if !isValid : continue
        
        color[current] ← c
          
        //向前探测检查
        success ← csp_check(current, c, &rc)
        
        if success:
            next_node ++;
    csp(next_node) // Recursive call

        // 回溯1:恢复所有删除颜色
        Restore_Color();

    // 回溯2:单色队列设置为未填色
    Set_Color1();
