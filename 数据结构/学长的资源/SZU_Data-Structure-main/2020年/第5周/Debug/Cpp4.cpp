#include <iostream>							//输入/输出函数
using namespace std;

int main()
{
	char string[128];

	cout << "请输入一个字符串: ";
	cin >> string;
	cout << endl;
	cout << "the input is: " << string << endl << endl;
	return 1;
}