#include <iostream>
#include <stack>

using namespace std;

struct Point
{
    int x;
    int y;
    int dir; // 0 right 1 down 2 left 3 up
    Point(int x, int y, int dir = 0) : x(x), y(y), dir(dir) {}
};

/* typedef struct
{
    int ord;
    Point seat ;
    int di;
}; */
void getnextpoint(Point &curr)
{
    if (curr.dir == 0)
        curr.y++;
    else if (curr.dir == 1)
        curr.x++;
    else if (curr.dir == 2)
        curr.y--;
    else
        curr.x--;
}
void printpath(stack<Point> s)
{
    if (!s.empty())
    {
        int i = 0;
        stack<Point> s2;
        while (!s.empty())
        {
            s2.push(s.top());
            s.pop();
        }
        while (!s2.empty())
        {
            Point cpos = s2.top();
            int row = cpos.x - 1, col = cpos.y - 1;
            if ((++i) % 4 == 0)
                cout << "[" << row << "," << col << "]--\n";
            else
                cout << "[" << row << "," << col << "]--";
            s2.pop();
        }
        cout << "END\n";
    }
    else
    {
        cout << "no path\n";
    }
}
void MeshPath(int mesh[][100], int n)
{
    stack<Point> s;
    Point curr = Point(1, 1, 0);
    do
    {
        if (mesh[curr.x][curr.y] == 0)
        {
            mesh[curr.x][curr.y] = 2;
            curr.dir = 0;
            s.push(curr);
            if (curr.x == n && curr.y == n)
                break;
            else
            {

                getnextpoint(curr);
            }
        }
        else
        {
            if (!s.empty())
            {
                curr = s.top();
                s.pop();
                if (curr.dir < 3)
                {               // 还有位置可以走
                    curr.dir++; // 设定新位置
                    s.push(curr);
                    getnextpoint(curr);
                }
                else
                {
                    while (!s.empty())
                    {
                        curr = s.top();
                        s.pop();
                        if (curr.dir < 3)
                        {
                            curr.dir++;
                            s.push(curr);
                            getnextpoint(curr);
                            break;
                        }
                    }
                }
            }
        }
    } while (!s.empty());

    printpath(s);
}

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        int mesh[100][100] = {0};
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
                cin >> mesh[i][j];
        }
        for (int i = 0; i <= n + 1; i++)
            mesh[0][i] = mesh[i][0] = mesh[n + 1][i] = mesh[i][n + 1] = 1;
        MeshPath(mesh, n);
    }
    return 0;
}
