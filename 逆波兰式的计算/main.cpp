#include <iostream>
#include <string>

#define MAXLEN 50

using namespace std;

// 分析栈
class Stack_Parse
{
public:
	char stack[MAXLEN];
	int top_pointer;

	Stack_Parse()
	{
		top_pointer = 0;
	}

	Stack_Parse(char c)
	{
		top_pointer = 0;
		stack[top_pointer++] = c;
	}

	void push(const char c)
	{
		stack[top_pointer++] = c;
	}

	char pop()
	{
		char c = stack[--top_pointer];
		stack[top_pointer] = '\0';
		return c;
	}

	char top()
	{
		int index = top_pointer - 1;

		return stack[index];
	}

	// 自底部向上地返回栈内元素
	string getStackElement()
	{
		string str;

		for (int i = 0; i < top_pointer; ++i)
			str += stack[i];

		return str;
	}

	bool isEmpty()
	{
		string str = getStackElement();
		
		if (str.length() != 1 || str[0] != '#')
			return false;
		return true;
	}
};

class Parse
{
private:
	string str;
	string castStr;
	Stack_Parse op_stack, result_stack;

	// 获得算符优先级
	int priority(char c)
	{
		int p;

		if (c == '#')
			p = 0;
		else if (c == '(')
			p = 1;
		else if (c == '+' || c == '-')
			p = 2;
		else if (c == '*' || c == '/')
			p = 3;

		return p;
	}

	// 比较a与b的优先级，a高于b返回true，否则返回false
	bool isHighterAfter(char a, char b)
	{
		int p_a = priority(a);
		int p_b = priority(b);

		return p_a > p_b;
	}

	/*
		stack_parse为分析栈
		stack_parse为结果栈
		str为剩余符号串
	*/
	void displayCast(Stack_Parse parse, 
					Stack_Parse result,
					string str)
	{
		string str_parse = parse.getStackElement();
		string str_result = result.getStackElement();

		cout << str_parse << "\t\t" << str << "\t\t\t" << str_result << endl;
	}

	void cast(string str)
	{
		string parse_str = str;
		bool lastIsDigit = false;


		bool hasError = false;
		string message;

		cout << "分析栈" << "\t\t" << "剩余符号串" << "\t\t" << "结果栈" << endl;

		while (parse_str.size() > 1)
		{
			char first = parse_str[0];

			// 如果是数字
			if (isdigit(first))
			{
				lastIsDigit = true;
				result_stack.push(first);
				parse_str.erase(0, 1);
				displayCast(op_stack, result_stack, parse_str);
				continue;
			}

			//if (parse_str.length() > 1)
			//{
			//	char next = parse_str[1];
			//	if (lastIsDigit && isdigit(next))
			//	{
			//		lastIsDigit = false;
			//		result_stack.push('&');
			//	}
			//}

			char topChar;
			bool isHighter;

			if (first != ')')
			{
				topChar = op_stack.top();
				isHighter = isHighterAfter(first, topChar);

				if (lastIsDigit && isHighter)
				{
					lastIsDigit = false;
					result_stack.push('&');
				}
			}

			// 如果是返括号，进行匹配
			if (first == ')')
			{
				while (1)
				{
					char topChar = op_stack.pop();

					if (topChar != '(')
						result_stack.push(topChar);
					else
					{
						parse_str.erase(0, 1);
						displayCast(op_stack, result_stack, parse_str);
						break; // 括号匹配成功
					}

					if (op_stack.isEmpty())
					{
						message = "括号匹配失败";
						hasError = true;
						break;
					}
				}
				continue;
			}

			//char topChar = op_stack.top();
			//bool isHighter = isHighterAfter(first, topChar);
			
			// 如果优先级高于栈顶，执行push操作
			if (isHighter)
			{
				op_stack.push(first);
				parse_str.erase(0, 1);
			}
			// 如果优先级低于栈顶
			else
			{
				while (1)
				{
					/*if (op_stack.isEmpty() && op_stack.p)
					{
						hasError = true;
						message = "字符栈已空，匹配失败";
						break;
					}*/

					topChar = op_stack.top();
					isHighter = isHighterAfter(first, topChar);
					
					if (isHighter)
					{
						op_stack.push(first);
						parse_str.erase(0, 1);
						break;
					}

					op_stack.pop();
					result_stack.push(topChar);
				}
			}
			displayCast(op_stack, result_stack, parse_str);
		}
		
		
		if (hasError)
			cout << message << endl;
		else
		{
			while (!op_stack.isEmpty())
			{
				char topChar = op_stack.pop();
				result_stack.push(topChar);
			}

			displayCast(op_stack, result_stack, parse_str);

			castStr = result_stack.getStackElement();
			cout << "逆波兰式为" << castStr << endl;
		}
	}

	void count()
	{
		Stack_Parse count_stack;
		string countStr = castStr;
		int result = 0;

		int point = 0;
		int len = countStr.length();

		cout << "运算栈" << "\t\t" << "剩余串" << endl;

		while (countStr.length() > 0)
		{
			char c = countStr.at(0);
			countStr.erase(0, 1);

			// 数字
			if (isdigit(c) || c == '&')
				count_stack.push(c);
			// 操作符
			else
			{
				string a_str, b_str;
				char temp_c;

				// 读取右操作数
				while (1)
				{
					temp_c = count_stack.top();
					
					if (temp_c == '&')
						break;

					b_str.insert(0, 1, temp_c);
					count_stack.pop();
				}

				// 读取左操作数
				// 先将栈顶的'&'出栈
				count_stack.pop();
				while (1)
				{
					if (count_stack.getStackElement() == "")
						break;

					temp_c = count_stack.top();

					if (temp_c == '&')
						break;

					a_str.insert(0, 1, temp_c);
					count_stack.pop();
				}

				// 开始运算
				int a = stoi(a_str);
				int b = stoi(b_str);

				if (c == '+')
					result = a + b;
				else if (c == '-')
					result = a - b;
				else if (c == '*')
					result = a * b;
				else if (c == '/')
					result = a / b;

				string tempStr = to_string(result);

				for (int i = 0; i < tempStr.length(); ++i)
					count_stack.push(tempStr.at(i));
			}

			++point;

			string out_stack = count_stack.getStackElement();
			string out_str = countStr;

			cout << out_stack << "\t\t" << out_str << endl;
		}

		cout << "逆波兰式" << castStr << "的计算机结果为" << result << endl;
	}

public:
	Parse() :op_stack('#') {}

	void parse(string str)
	{
		cout << "-----------------转换--------------------" << endl;
		// 转换
		cast(str);
		cout << "-----------------计算--------------------" << endl;
		// 计算
		count();

	}
};

int main()
{
	string str;
	Parse parse;

	cout << "(1) 逆波兰式的计算程序，编制人：黎铭杰 202016040201 软件2002" << endl;
	cout << "输入一以#结束的中缀表达式(可包括+ -*/()数字#)：在此位置输入符号串如(28+68)*2#：";
	cin >> str;

	parse.parse(str);

	return 0;
}
