组队列


cin >> n;
queue<string> q[n];
map<string, int> membermap;

// 先确定映射关系—— map容器
for (int i = 0; i < n; i++)
{
    cin >> m;
    for (int j = 0; j < m; j++)
    {
        string member;
        cin >> member;
        membermap[member] = i; // 第i组
    }
}

if ()
{
    cin >> member;
    q[membermap[member]].push(member);
    // 还需要一个队列记录谁先来
    // 出队列的时候从存储队列取对头元素 q[order.front()].pop()
    // remember —— order.pop() 前提是队列为空
    // 即order.front()不为空，遇到出队就一直出队
}

int kmp(string s,string p,int pos){
    getnext(p, next);
    for (int i = pos, j = 0; i < s.size() && j < p.size()){
        if(j==-1 ||s[i]==p[j]){
            i++;
            j++;
        }
        else{
            j = next[j]; 
        }
    }
}