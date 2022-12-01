#include <iostream>
#include <string>

using namespace std;

// 状态集
// S：初态
// L：标识符 
// Z：数字0 
// N：除0外的整数
// '!'仅为填充符号
const char Z[] = {'!', 'S', 'L', 'Z', 'N' };
const int LEN_Z = 5;

// 单独用E表示出错
const char E = 'E';
// 单独用I代表非法
const char I = 'I';

// 字母表
// '!'仅为填充符号
const char L[] = { '!', '1', '2', '3', '0', 'a','b','c' };
const int LEN_L = 8;

// 转移矩阵
char matrix[5][8]
{
	'!', '1', '2', '3', '0', 'a', 'b', 'c',
	'S', 'N', 'N', 'N', 'Z', 'L', 'L', 'L',
	'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L',
	'Z', 'E', 'E', 'E', 'E', 'E', 'E', 'E',
	'N', 'N', 'N', 'N', 'N', 'E', 'E', 'E'
};

// 信息集
typedef struct Info
{
	// 种别
	char type;
	// 值
	string value;
}Info;

// 返回字符a在数组c中的位置
// 不存在（非法输入）返回-1
int location(const char* c, const char a, const int len)
{
	for (int i = 0; i < len; ++i)
	{
		if (c[i] == a)
			return i;
	}
	return -1;
}

// 状态转换函数
// state：状态
// c：输入字母
// 返回转换后的状态
char f(char state, char c)
{
	// 先检查是否是合法输入
	if (location(L, c, LEN_L) == -1)
	{
		return I;
	}

	int x = location(Z, state, LEN_Z);
	int y = location(L, c, LEN_L);

	return matrix[x][y];
}

// 词法分析函数
Info parse(const string str)
{
	int len = str.size();
	char state;
	Info info;

	info.value = str;
	state = f('S', str[0]);
	info.type = state;

	if (state == E || state == I)
	{
		return info;
	}

	for (int i = 1; i < len; ++i)
	{
		state = f(state, str[i]);
		info.type = state;

		if (state == E || state == I)
		{
			return info;
		}
	}

	return info;
}

int main() 
{
	// 单词数
	int n;
	char type;
	string str;
	string result;
	Info info;

	cout << "202016040201" << "       " << "黎铭杰" << endl << endl;

	while(1)
	{
		// 输入单词
		cin >> str;
		
		// 解析
		info = parse(str);
		
		type = info.type;
		str = info.value;

		cout << str << "：";

		if (type == E) result = "不接受";
		if (type == I) result = "非法的输入";
		if (type == 'L') result = "标识符";
		if (type == 'Z') result = "数字0";
		if (type == 'N') result = "除0以外的整数";

		cout << result << endl << endl;
	}
}

