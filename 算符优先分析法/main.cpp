#include <iostream>
#include <string>
#include <vector>
#include <cstdbool>

# define MAXLEN 100

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
		stack[top_pointer++] = '#';
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

	// 如果栈内还存在除'#'和c以外的字符，返回真
	bool hasOtherChar(char c)
	{
		string str = getStackElement();

		for (char s : str)
		{
			if (s == '#' || s == c);
			else
				return true;
		}

		return false;
	}
};

class V_N_Parse
{
private:
	// 构造字符表
	// 字符！仅挤占下标0
	const vector<char>V_N = { '!','+','-','*','/','i','(',')','#' };
	// 构造算符优先关系表
	// 优先级：1为大于， 2为小于， 3为等于，0为不存在
	const int op[9][9] =
	{
		0,0,0,0,0,0,0,0,0,// 首行无用
		0,1,1,2,2,2,2,1,1,
		0,1,1,2,2,2,2,1,1,
		0,1,1,1,1,2,2,1,1,
		0,1,1,1,1,2,2,1,1,
		0,1,1,1,1,0,0,1,1,
		0,2,2,2,2,2,2,3,0,
		0,1,1,1,1,0,0,1,1,
		0,2,2,2,2,2,2,0,3
		// 首列无用
	};
	// 算符优先关系分析串
	string parse_str = "#";

	Stack_Parse stack_parse;

	// 获得非终结符的数组下标
	int location(const char c)
	{
		int index = 0;

		for (int i = 1; i < V_N.size(); ++i)
			if (c == V_N.at(i))
				index = i;
		return index;
	}

	// 获得算符优先级
	int priority(char a, char b)
	{
		int index_a = location(a);
		int index_b = location(b);

		int p = 0;

		if (index_a != 0 && index_b != 0)
			p = op[index_a][index_b];

		return p;
	}

	// 字符转换，将优先级从数字还原成符号形式
	void change(string& str)
	{
		for (char& c : str)
		{
			if (c == '1')
				c = '>';
			else if (c == '2')
				c = '<';
			else if (c == '3')
				c = '=';
			else if (c == '0')
				c = '!';
		}
	}

public:
	// 入口
	void parse(string str)
	{
		string tempStr = str;
		int cnt = 1;
		int p = 1;
		// 状态保留
		string state_stack, state_str, state_priority, action;
		bool isReduce = true;
		bool isFirst = true;

		cout << "步骤" << "\t" << "栈" << "\t" << "剩余输入串" << "\t" << "优先关系" << "\t\t" << "动作" << endl;

		while (str.size() > 0 && p != 0)
		{
			state_stack = stack_parse.getStackElement();

			// 如果上一步进行了归约，不需要移进优先关系
			// 没有归约，移进
			// 首次运行，移进
			if (!isReduce || isFirst) 
			{
				isFirst = false;
				// 获取算术优先关系分析串最尾端的字符
				char last = parse_str.back();
				// 获取剩余串的第一个字符
				char input = str.at(0);

				// 获得优先级
				p = priority(last, input);
				if (p == 0)
					break;

				// 构造分析串
				// 放入优先级
				parse_str.push_back(p + '0');
				// 放入剩余串的第一个字符
				parse_str.push_back(input);
			}
			isReduce = false;

			state_str = str;
			state_priority = parse_str;

			int index_left, index_right;
			int len;
			int i, j;

			// 扫描
			for (i = 1; i < parse_str.size(); ++i)
			{
				// 从左往右扫描到了第一个 >
				if (parse_str.at(i) == '1')
				{
					index_right = i;
					// 回溯
					for (j = i - 1; j >= 1; --j)
					{
						// 回溯时找到了第一个 <
						if (parse_str.at(j) == '2')
						{
							index_left = j;
							isReduce = true;
							break;
						}
					}
				}
				if (isReduce)
					break;
			}

			// 扫描后发现可归约串
			if (isReduce) {
				// 归约两者之间的串
				len = index_right - index_left + 1;

				int count = 1;
				// 如果栈顶是非终结符，且栈顶的三个元素从上往下依次为 F/终结符，终结符，F，应将它们全部取出
				if (stack_parse.top() == 'F')
				{
					char top_char = stack_parse.pop();
					char second_char = stack_parse.top();

					// 如果第二个元素为终结符
					if (second_char != 'F' && second_char != '#')
					{
						stack_parse.pop();
						char third_char = stack_parse.top();
						// 如果第三个元素是非终结符
						if(third_char=='F')
							count = 3;
						stack_parse.push(second_char);
					}
					// 如果第二个元素也是终结符，出错
					else
						p = 0;

					stack_parse.push(top_char);
				}
				// 如果栈顶是终结符
				else
				{
					char top_char = stack_parse.pop();
					char second_char = stack_parse.top();
					
					// 如果是形如(F)的情况
					if (top_char == ')' && second_char == 'F')
					{
						stack_parse.pop();
						char third_char = stack_parse.top();

						if (third_char == '(')
							count = 3;
						stack_parse.push(second_char);
					}
					// 如果不是形如(F)的情况且第二个元素是非终结符，出错
					else if (second_char == 'F')
						p = 0;

					stack_parse.push(top_char);
				}

				// 栈顶出栈
				while (count >= 1, count--)
					stack_parse.pop();

				// 终结符入栈
				stack_parse.push('F');

				action = "归约";
				string message(parse_str.at(index_left), len);
				action.append(message);

				// 删除分析串中的可归约子串[index_left, index_right]
				parse_str.erase(index_left, len);
				int temp_left = index_left - 1;
				int temp_right = index_left;

				// 插入被删除字串左右两个非终结符之间的优先级
				int temp_p = priority(parse_str.at(temp_left), parse_str.at(temp_right));
				parse_str.insert(temp_right, 1, temp_p + '0');

				// 如果出错（上述二者没有优先级，标记P，使循环终止）
				if (temp_p == 0)
					p = 0;
			}
			// 扫描完毕后发现无法归约，移进
			else
			{
				// 剩余输入串（不是'#'）的第一个字符入栈
				char c = str.at(0);
				if (c != '#')
				{
					stack_parse.push(c);
					// 删除剩余输入串的第一个字符
					str.erase(0, 1);
					action = "移进";
					action.push_back(stack_parse.top());
				} 
				// 如果剩余输入串还剩下'#'
				else
				{
					p = 0; // 终止循环
					break; // 终止本次循环
				}
			}
			// 输出结果
			change(state_priority);
			cout << cnt << "\t" << state_stack << "\t" << state_str << "\t\t" << state_priority << "\t\t\t" << action << endl;

			// 收尾工作
			++cnt;
			state_stack = stack_parse.getStackElement();
			state_str = str;
			state_priority = parse_str;

		} // 算法结束

		// 如果出错
		if (p == 0)
			action = "出错";
		// 保证此时输入串只剩下'#'，并且输入串只剩下’#'
		if (!stack_parse.hasOtherChar('F') && str == "#")
			action = "成功";

		// 输出最后一次记录
		change(state_priority);
		cout << cnt << "\t" << state_stack << "\t" << state_str << "\t\t" << state_priority << "\t\t\t" << action << endl;

		if (action == "成功")
			cout << "判定结果：" << tempStr << "为合法符号串" << endl;
		else
		{
			cout << "判定结果：" << tempStr << "为非法符号串" << endl;
			cout << "栈内只有“＃”和某非终结符，输入串剩下“＃”，表示分析成功；否则输入串有错" << endl;
		}
	}
};

int main()
{
	string str;
	V_N_Parse v_n_parse;
	
	cout << "算符优先分析程序，编制人：黎铭杰		202016040201	软件2002" << endl;
	cout << "终结符之间若无优先级，使用符号'!'表示" << endl;
	cout << "输入一以#结束的符号串(可包括+-*/()i#等符号，如i+i*i#)：";
	
	cin >> str;
	cout << "对该输入串的算符优先分析过程输出如下：" << endl;
	
	v_n_parse.parse(str);

	return 0;
}