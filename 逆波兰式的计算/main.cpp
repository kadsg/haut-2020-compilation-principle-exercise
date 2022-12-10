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

			if (lastIsDigit)
			{
				lastIsDigit = false;
				result_stack.push('&');
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

			char topChar = op_stack.top();
			bool isHighter = isHighterAfter(first, topChar);
			
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
		string parse_str = castStr;
	}

public:
	Parse() :op_stack('#') {}

	void parse(string str)
	{
		// ת��
		cast(str);
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
