#include <iostream>
#include <string>

using namespace std;

/*
SELECT集

产生式			该产生式对应的SELECT集
（1）E→ TM		{(, i}
（2）M→ +TM	{+}
（3）M→ ε		{), #}
（4）T→ FN		{ (, i}
（5）N→ *FN	{*}
（6）N→ ε		{+, ), #}
（7）F→ (E)	{( }
（8）F→ i		{i}
*/

// 构造一个解析栈
typedef struct Stack_Parse
{
	char stack_parse[100];
	int top; // 栈顶指针
}Stack_Parse;

Stack_Parse stack_parse;

void push(Stack_Parse &stack_parse, char c)
{
	stack_parse.stack_parse[stack_parse.top] = c;
	++stack_parse.top;
}

//char top(Stack_Parse stack_parse)
//{
//	return stack_parse.stack_parse[stack_parse.top - 1];
//}

char pop(Stack_Parse& stack_parse)
{
	--stack_parse.top;
	return stack_parse.stack_parse[stack_parse.top];
}

// 返回这个栈的所有元素（自底向上）
string getAllElement(const Stack_Parse stack_parse)
{
	string str;
	
	for (int i = 0; i < stack_parse.top; ++i)
		str += stack_parse.stack_parse[i];

	return str;
}

int isEmpty(const Stack_Parse stack_parse)
{
	return stack_parse.top == 0;
}

// 定义产生式
typedef struct Info
{
	int seq; // 产生式的序号
	char left; // 产生式左部
	string right; // 产生式右部
}Info;

// 使用最简单的else if的方式返回产生式
// left 栈顶元素
// input 匹配输入
Info getInfo(const char left, const char input)
{
	int seq = 0;
	string right;
	Info info;
	int match = 0;

	if (left == 'E')
	{
		if (input == '(' || input == 'i')
		{
			seq = 1;
			right = "TM";
			match = 1;
		}
	} 
	else if (left == 'M')
	{
		if (input == '+')
		{
			seq = 2;
			right = "+TM";
			match = 1;
		}
		else if (input == ')' || input == '#')
		{
			seq = 3;
			right = "";
			match = 1;
		}
	}

	else if (left == 'T')
	{
		if (input == '(' || input == 'i')
		{
			seq = 4;
			right = "FN";
			match = 1;
		}
	}
	else if (left == 'N')
	{
		if (input == '*')
		{
			seq = 5;
			right = "*FN";
			match = 1;
		}
		else if (input == '+' || input == ')' || input == '#')
		{
			seq = 6;
			right = "";
			match = 1;
		}
	}
	else if (left == 'F')
	{
		if (input == '(')
		{
			seq = 7;
			right = "(E)";
			match = 1;
		}
		else if (input == 'i')
		{
			seq = 8;
			right = "i";
			match = 1;
		}
	}

	// 匹配成功
	else if (left == input)
	{
		seq = 0;
		right = left;
		match = 1;
	}
	// 出错
	if (match == 0)
	{
		seq = -1;
		right = "ERROR!";
	}

	info.seq = seq;
	info.left = left;
	info.right = right;

	return info;
}

// 获得字符串str的逆序
string getReverse(const string str)
{
	string s(str.rbegin(), str.rend());

	return s;
}

// 字符串入栈
void pushStr(Stack_Parse& stack_parse, const string str)
{
	for (char c : str)
	{
		push(stack_parse, c);
	}
}

// 匹配字符串
void match(const string str)
{
 	string temp = str;
	string out = temp;
	//// 字符指针，指向串匹配的位置
	//int i = 0;
	Info info;
	int cnt = 0;
	// “所用产生式”那一列的信息
	string message;
	int ERROR = 0;
	int SUCCEED = 0;

	string allElement;

	cout << "对该输入串的LL(1)分析过程输出如下：" << endl;
	cout << "步骤" << "		" << "栈" << "		" << "剩余输入串" << "	" << "所用产生式" << endl;

	while (1)
	{
		++cnt;
		// 获取栈内元素
		allElement = getAllElement(stack_parse);
		// 栈顶出栈
		char top = pop(stack_parse);
		// 匹配并返回结果
		// 如果此时输入串为空，匹配完成
		if (temp.size() == 0)
		{
			SUCCEED = 1;
			message = "接受";
			break;
		}
		info = getInfo(top, temp.at(0));

		if (info.seq == -1)
		{
			// 出错
			out = temp;
			message = "出错";
			ERROR = 1;
			break;
		}
		else if (info.seq == 0)
		{
			// 匹配
			
			// 移除输入串头部
			out = temp;
			char head = out.at(0);
			temp.erase(0, 1);

			message = head;
			message.append("匹配");

			if (head == '#')
				message = "接受";
		}
		else
		{
			// 使用产生式
			message = to_string(info.seq);
			// 获得匹配后放入栈中的串
			string in = getReverse(info.right);
			// 入栈
			pushStr(stack_parse, in);
		}
		
		cout << cnt << "		" << allElement << "		" << out << "		" << message << endl;
	}
	
	if (SUCCEED)
	{
		cout << str << "为合法符号输入串" << endl;
	}
	else if (ERROR)
	{
		cout << cnt << "		" << allElement << "		" << out << "		" << message << endl;
		cout << str << "为非法符号输入串" << endl;
	}
}

int main()
{
	cout << "LL(1)分析程序，编制人：" << "黎铭杰，202016040201，软件2002" << endl;

	// 初始化解析栈
	stack_parse.top = 0;
	push(stack_parse, '#');
	push(stack_parse, 'E');

	cout << "输入一以#结束的符号串(可包括+*()i#等符号，如i+i#)：";
	string str;
	cin >> str;

	match(str);
	return 0;
}
