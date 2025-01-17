#include <iostream>
#include <queue>
#include <iomanip>
#include <cmath>
using namespace std;

struct customer
{
    int T;     // arrival time
    int P;     // processing time
    bool type; // true for VIP, false for normal
};
struct Windows
{
    int endTime;
    int count;
    Windows(int entTime = 0, int count = 0) : endTime(0), count(0) {}
};

int main()
{
    int N;
    cin >> N;
    queue<customer> q1, q2;
    customer c[N];
    for (int i = 0; i < N; i++)
    {
        cin >> c[i].T >> c[i].P >> c[i].type;
        if (c[i].P > 60)
            c[i].P = 60;
        /*         if (c[i].type == true)
                {
                    q1.push(c[i]);
                }
                else
                {
                    q2.push(c[i]);
                } */
    }
    int currentTime = 0;
    int K, M;
    cin >> K >> M;
    Windows windows[K];
    int sum = 0, max = 0, waitTime = 0, index = 0;
    while (index < N || !q1.empty() || !q2.empty()) // 没人的时候还不能结束
    {
        // 根据到达时间入列
        while (index < N && c[index].T <= currentTime)
        {
            if (c[index].type == 1)
                q1.push(c[index]);
            else
                q2.push(c[index]);
            index++;
        }

        if (windows[M].endTime <= currentTime && !q1.empty())
        {
            windows[M].endTime = currentTime + q1.front().P;
            waitTime = currentTime - q1.front().T;
            sum += waitTime;
            if (waitTime > max)
                max = waitTime;
            q1.pop();
            windows[M].count++;
        }
        for (int i = 0; i < K; i++)
        {
            int flag = 0;
            if (windows[i].endTime <= currentTime)
            {
                if (!q1.empty() && !q2.empty())
                {
                    if (q1.front().T <= q2.front().T)
                    {
                        flag = 1;
                    }
                    else
                    {
                        flag = 2;
                    }
                }
                else if (q1.empty() && !q2.empty())
                {
                    flag = 2;
                }
                else if (!q1.empty() && q2.empty())
                {
                    flag = 1;
                }

                if (flag == 1)
                {
                    windows[i].endTime = currentTime + q1.front().P;
                    waitTime = currentTime - q1.front().T;
                    sum += waitTime;
                    if (waitTime > max)
                        max = waitTime;
                    q1.pop();
                    windows[i].count++;
                }
                else if (flag == 2)
                {
                    windows[i].endTime = currentTime + q2.front().P;
                    waitTime = currentTime - q2.front().T;
                    sum += waitTime;
                    if (waitTime > max)
                        max = waitTime;
                    q2.pop();
                    windows[i].count++;
                }
            }
        }
        currentTime++;
    }
    int extratime = 0, max2 = 0;
    for (int i = 0; i < K; i++)
    {
        extratime = windows[i].endTime - currentTime;
        max2 = extratime > max2 ? extratime : max2;
    }
    currentTime += max2;
    double avg = 1.0 * sum / N;
    cout << fixed << setprecision(1) << avg << " " << max << " " << currentTime << endl;
    for (int i = 0; i < K; i++)
    {
        if (i != 0)
            cout << " ";
        cout << windows[i].count;
    }

    return 0;
}