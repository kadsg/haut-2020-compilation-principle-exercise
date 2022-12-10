#include <iostream>
#include <string>

#define MAXLEN 50

using namespace std;

// ����ջ
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

	// �Եײ����ϵط���ջ��Ԫ��
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

	// ���������ȼ�
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

	// �Ƚ�a��b�����ȼ���a����b����true�����򷵻�false
	bool isHighterAfter(char a, char b)
	{
		int p_a = priority(a);
		int p_b = priority(b);

		return p_a > p_b;
	}

	/*
		stack_parseΪ����ջ
		stack_parseΪ���ջ
		strΪʣ����Ŵ�
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

		cout << "����ջ" << "\t\t" << "ʣ����Ŵ�" << "\t\t" << "���ջ" << endl;

		while (parse_str.size() > 1)
		{
			char first = parse_str[0];

			// ���������
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

			// ����Ƿ����ţ�����ƥ��
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
						break; // ����ƥ��ɹ�
					}

					if (op_stack.isEmpty())
					{
						message = "����ƥ��ʧ��";
						hasError = true;
						break;
					}
				}
				continue;
			}

			//char topChar = op_stack.top();
			//bool isHighter = isHighterAfter(first, topChar);
			
			// ������ȼ�����ջ����ִ��push����
			if (isHighter)
			{
				op_stack.push(first);
				parse_str.erase(0, 1);
			}
			// ������ȼ�����ջ��
			else
			{
				while (1)
				{
					/*if (op_stack.isEmpty() && op_stack.p)
					{
						hasError = true;
						message = "�ַ�ջ�ѿգ�ƥ��ʧ��";
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
			cout << "�沨��ʽΪ" << castStr << endl;
		}
	}

	void count()
	{
		Stack_Parse count_stack;
		string countStr = castStr;
		int result = 0;

		int point = 0;
		int len = countStr.length();

		cout << "����ջ" << "\t\t" << "ʣ�മ" << endl;

		while (countStr.length() > 0)
		{
			char c = countStr.at(0);
			countStr.erase(0, 1);

			// ����
			if (isdigit(c) || c == '&')
				count_stack.push(c);
			// ������
			else
			{
				string a_str, b_str;
				char temp_c;

				// ��ȡ�Ҳ�����
				while (1)
				{
					temp_c = count_stack.top();
					
					if (temp_c == '&')
						break;

					b_str.insert(0, 1, temp_c);
					count_stack.pop();
				}

				// ��ȡ�������
				// �Ƚ�ջ����'&'��ջ
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

				// ��ʼ����
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

		cout << "�沨��ʽ" << castStr << "�ļ�������Ϊ" << result << endl;
	}

public:
	Parse() :op_stack('#') {}

	void parse(string str)
	{
		cout << "-----------------ת��--------------------" << endl;
		// ת��
		cast(str);
		cout << "-----------------����--------------------" << endl;
		// ����
		count();

	}
};

int main()
{
	string str;
	Parse parse;

	cout << "(1) �沨��ʽ�ļ�����򣬱����ˣ������� 202016040201 ���2002" << endl;
	cout << "����һ��#��������׺���ʽ(�ɰ���+ -*/()����#)���ڴ�λ��������Ŵ���(28+68)*2#��";
	cin >> str;

	parse.parse(str);

	return 0;
}
