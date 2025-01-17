#include <iostream>
#include <stack>
#include <cstdlib>
#include <string>
#include <algorithm>

using namespace std;

string OPSET = "+-*/()#";

char Prior[7][7] = {
    '<', '<', '<', '<', '<', '>', '>',
    '<', '<', '<', '<', '<', '>', '>',
    '>', '>', '<', '>', '<', '>', '>',
    '>', '>', '>', '<', '<', '>', '>',
    '<', '<', '<', '<', '<', '=', ' ',
    '>', '>', '>', '>', ' ', '>', '>',
    '<', '<', '<', '<', '<', ' ', '='};
char Prior2[7][7] = {
    '>', '>', '<', '<', '<', '>', '>',
    '>', '>', '<', '<', '<', '>', '>',
    '>', '>', '>', '>', '<', '>', '>',
    '>', '>', '>', '>', '<', '>', '>',
    '<', '<', '<', '<', '<', '=', ' ',
    '>', '>', '>', '>', ' ', '>', '>',
    '<', '<', '<', '<', '<', ' ', '='};

char precede(char Atop, char Btop)
{
    return Prior[OPSET.find(Atop)][OPSET.find(Btop)];
}
char precede2(char Atop, char Btop)
{
    return Prior2[OPSET.find(Atop)][OPSET.find(Btop)];
}
string Bolan(string expression)
{
    reverse(expression.begin(), expression.end());
    for (int i = 0; i < expression.length(); i++)
    {
        if (expression[i] == '(')
            expression[i] = ')';
        else if (expression[i] == ')')
            expression[i] = '(';
    }
    expression += "#";
    int i = 0;
    string tempdata;
    char ch = expression[i];
    stack<string> operand;
    stack<char> operatorstack; // 运算符
    operatorstack.push('#');
    while (ch != '#' || operatorstack.top() != '#')
    {

        if (isdigit(ch) || ch == '.') // 30.1245-> 5421.03
        {
            tempdata += ch;
            ch = expression[++i];
            if (!isdigit(ch) && ch != '.')
            {
                reverse(tempdata.begin(), tempdata.end());
                operand.push(" " + tempdata);
                tempdata.clear();
            }
        }
        else
        {
            switch (precede(operatorstack.top(), ch))
            {
            case '<':
                operatorstack.push(ch);
                ch = expression[++i];
                break;
            case '=':
                operatorstack.pop();
                ch = expression[++i];
                break;
            case '>':
                tempdata += operatorstack.top();
                operatorstack.pop();
                tempdata += operand.top();
                operand.pop();
                tempdata += operand.top();
                operand.pop();
                operand.push(" " + tempdata);
                tempdata.clear();
                break;
            }
        }
    }
    return operand.top();
}

string nibolan(string expression)
{
    string output;
    stack<char> A;
    int i = 0;
    string tempdata;
    expression += "#";
    char ch = expression[i];
    A.push('#');
    while (ch != '#' || A.top() != '#')
    {

        if (isdigit(ch))
        {
            tempdata += ch;
            ch = expression[++i];
            if (!isdigit(ch))
            {
                output += tempdata;
                output += " ";
                tempdata.clear();
            }
        }
        else
        {
            switch (precede2(A.top(), ch))
            {
            case '<':
                A.push(ch);
                ch = expression[++i];
                break;
            case '=':
                A.pop();
                ch = expression[++i];
                break;
            case '>':
                output += A.top();
                output += " ";
                A.pop();
                break;
            }
        }
    }
    return output;
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        string ch;
        cin >> ch;
        string A = Bolan(ch);
        /*      for (int i = 0; i < A.length(); i++) {
             if (i != 0 || !(isdigit(A[i])&&isdigit(A[i-1])))
                 cout << " ";
             cout << A[i];
         }*/
        A.erase(0, 1);
        cout << A << endl;
        string B = nibolan(ch);
        B.erase(B.length() - 1, 1);
        cout << B << endl;
        if (t != 0)
            cout << endl;
    }

    return 0;
}