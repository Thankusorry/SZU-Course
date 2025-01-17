

double computer(string s)
{
    string numstr;
    string op = "+-*/()#";
    for (int i = 0; i < s.size();)
    {
        if (isdigit(s[i] || s[i] == '.'))
        {
            numstr += s[i];
        }
        else
        {
            if (numstr.size())
            {
                ds.push(stod(numstr)); // string to double
                numstr.clear();
            }
            char leftop = os.top();
            s[i];
            int index = op.find(leftop);
            int rightindex = op.find(s[i]);
            if (Prior[leftindex][rightindex] == '>')
            {
            }
            else if (Prior[leftindex][rightindex] == '=')
            {
                os.pop();
                i++;
            }
            else
            {
                os.push(s[i++]);
            }
        }
    }
}

void solve(int mesh[][30])
{
    curr = point(0, 0, 0);
    do
        ()
        {
            if (mesh[curr.x][curr.y] == 0)
            {
                mesh[curr.x][curr.y] = 2;
                s.push(curr);
                if (curr.x == n - 1 && curr.y == n - 1)
                    break;
                else
                {

                    curr = getnextpoint(curr);
                }
            }
            else
            {
                if (!s.empty())
                {
                    curr.s.top();
                    s.pop();
                    if (curr.dir < 3)
                    {
                        curr.dir++;
                        s.push(curr);
                    }
                    curr = getnextpoint(curr);
                }
            }
        }
    while (!path.empty())
}