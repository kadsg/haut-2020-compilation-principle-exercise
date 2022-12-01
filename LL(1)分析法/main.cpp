#include <iostream>
#include <string>

using namespace std;

/*
SELECT��

����ʽ			�ò���ʽ��Ӧ��SELECT��
��1��E�� TM		{(, i}
��2��M�� +TM	{+}
��3��M�� ��		{), #}
��4��T�� FN		{ (, i}
��5��N�� *FN	{*}
��6��N�� ��		{+, ), #}
��7��F�� (E)	{( }
��8��F�� i		{i}
*/

// ����һ������ջ
typedef struct Stack_Parse
{
	char stack_parse[100];
	int top; // ջ��ָ��
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

// �������ջ������Ԫ�أ��Ե����ϣ�
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

// �������ʽ
typedef struct Info
{
	int seq; // ����ʽ�����
	char left; // ����ʽ��
	string right; // ����ʽ�Ҳ�
}Info;

// ʹ����򵥵�else if�ķ�ʽ���ز���ʽ
// left ջ��Ԫ��
// input ƥ������
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

	// ƥ��ɹ�
	else if (left == input)
	{
		seq = 0;
		right = left;
		match = 1;
	}
	// ����
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

// ����ַ���str������
string getReverse(const string str)
{
	string s(str.rbegin(), str.rend());

	return s;
}

// �ַ�����ջ
void pushStr(Stack_Parse& stack_parse, const string str)
{
	for (char c : str)
	{
		push(stack_parse, c);
	}
}

// ƥ���ַ���
void match(const string str)
{
 	string temp = str;
	string out = temp;
	//// �ַ�ָ�룬ָ��ƥ���λ��
	//int i = 0;
	Info info;
	int cnt = 0;
	// �����ò���ʽ����һ�е���Ϣ
	string message;
	int ERROR = 0;
	int SUCCEED = 0;

	string allElement;

	cout << "�Ը����봮��LL(1)��������������£�" << endl;
	cout << "����" << "		" << "ջ" << "		" << "ʣ�����봮" << "	" << "���ò���ʽ" << endl;

	while (1)
	{
		++cnt;
		// ��ȡջ��Ԫ��
		allElement = getAllElement(stack_parse);
		// ջ����ջ
		char top = pop(stack_parse);
		// ƥ�䲢���ؽ��
		// �����ʱ���봮Ϊ�գ�ƥ�����
		if (temp.size() == 0)
		{
			SUCCEED = 1;
			message = "����";
			break;
		}
		info = getInfo(top, temp.at(0));

		if (info.seq == -1)
		{
			// ����
			out = temp;
			message = "����";
			ERROR = 1;
			break;
		}
		else if (info.seq == 0)
		{
			// ƥ��
			
			// �Ƴ����봮ͷ��
			out = temp;
			char head = out.at(0);
			temp.erase(0, 1);

			message = head;
			message.append("ƥ��");

			if (head == '#')
				message = "����";
		}
		else
		{
			// ʹ�ò���ʽ
			message = to_string(info.seq);
			// ���ƥ������ջ�еĴ�
			string in = getReverse(info.right);
			// ��ջ
			pushStr(stack_parse, in);
		}
		
		cout << cnt << "		" << allElement << "		" << out << "		" << message << endl;
	}
	
	if (SUCCEED)
	{
		cout << str << "Ϊ�Ϸ��������봮" << endl;
	}
	else if (ERROR)
	{
		cout << cnt << "		" << allElement << "		" << out << "		" << message << endl;
		cout << str << "Ϊ�Ƿ��������봮" << endl;
	}
}

int main()
{
	cout << "LL(1)�������򣬱����ˣ�" << "�����ܣ�202016040201�����2002" << endl;

	// ��ʼ������ջ
	stack_parse.top = 0;
	push(stack_parse, '#');
	push(stack_parse, 'E');

	cout << "����һ��#�����ķ��Ŵ�(�ɰ���+*()i#�ȷ��ţ���i+i#)��";
	string str;
	cin >> str;

	match(str);
	return 0;
}
