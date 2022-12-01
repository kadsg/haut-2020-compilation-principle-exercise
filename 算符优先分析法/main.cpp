#include <iostream>
#include <string>
#include <vector>
#include <cstdbool>

# define MAXLEN 100

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

	// �Եײ����ϵط���ջ��Ԫ��
	string getStackElement()
	{
		string str;

		for (int i = 0; i < top_pointer; ++i)
			str += stack[i];

		return str;
	}

	// ���ջ�ڻ����ڳ�'#'��c������ַ���������
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
	// �����ַ���
	// �ַ�������ռ�±�0
	const vector<char>V_N = { '!','+','-','*','/','i','(',')','#' };
	// ����������ȹ�ϵ��
	// ���ȼ���1Ϊ���ڣ� 2ΪС�ڣ� 3Ϊ���ڣ�0Ϊ������
	const int op[9][9] =
	{
		0,0,0,0,0,0,0,0,0,// ��������
		0,1,1,2,2,2,2,1,1,
		0,1,1,2,2,2,2,1,1,
		0,1,1,1,1,2,2,1,1,
		0,1,1,1,1,2,2,1,1,
		0,1,1,1,1,0,0,1,1,
		0,2,2,2,2,2,2,3,0,
		0,1,1,1,1,0,0,1,1,
		0,2,2,2,2,2,2,0,3
		// ��������
	};
	// ������ȹ�ϵ������
	string parse_str = "#";

	Stack_Parse stack_parse;

	// ��÷��ս���������±�
	int location(const char c)
	{
		int index = 0;

		for (int i = 1; i < V_N.size(); ++i)
			if (c == V_N.at(i))
				index = i;
		return index;
	}

	// ���������ȼ�
	int priority(char a, char b)
	{
		int index_a = location(a);
		int index_b = location(b);

		int p = 0;

		if (index_a != 0 && index_b != 0)
			p = op[index_a][index_b];

		return p;
	}

	// �ַ�ת���������ȼ������ֻ�ԭ�ɷ�����ʽ
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
	// ���
	void parse(string str)
	{
		string tempStr = str;
		int cnt = 1;
		int p = 1;
		// ״̬����
		string state_stack, state_str, state_priority, action;
		bool isReduce = true;
		bool isFirst = true;

		cout << "����" << "\t" << "ջ" << "\t" << "ʣ�����봮" << "\t" << "���ȹ�ϵ" << "\t\t" << "����" << endl;

		while (str.size() > 0 && p != 0)
		{
			state_stack = stack_parse.getStackElement();

			// �����һ�������˹�Լ������Ҫ�ƽ����ȹ�ϵ
			// û�й�Լ���ƽ�
			// �״����У��ƽ�
			if (!isReduce || isFirst) 
			{
				isFirst = false;
				// ��ȡ�������ȹ�ϵ��������β�˵��ַ�
				char last = parse_str.back();
				// ��ȡʣ�മ�ĵ�һ���ַ�
				char input = str.at(0);

				// ������ȼ�
				p = priority(last, input);
				if (p == 0)
					break;

				// ���������
				// �������ȼ�
				parse_str.push_back(p + '0');
				// ����ʣ�മ�ĵ�һ���ַ�
				parse_str.push_back(input);
			}
			isReduce = false;

			state_str = str;
			state_priority = parse_str;

			int index_left, index_right;
			int len;
			int i, j;

			// ɨ��
			for (i = 1; i < parse_str.size(); ++i)
			{
				// ��������ɨ�赽�˵�һ�� >
				if (parse_str.at(i) == '1')
				{
					index_right = i;
					// ����
					for (j = i - 1; j >= 1; --j)
					{
						// ����ʱ�ҵ��˵�һ�� <
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

			// ɨ����ֿɹ�Լ��
			if (isReduce) {
				// ��Լ����֮��Ĵ�
				len = index_right - index_left + 1;

				int count = 1;
				// ���ջ���Ƿ��ս������ջ��������Ԫ�ش�����������Ϊ F/�ս�����ս����F��Ӧ������ȫ��ȡ��
				if (stack_parse.top() == 'F')
				{
					char top_char = stack_parse.pop();
					char second_char = stack_parse.top();

					// ����ڶ���Ԫ��Ϊ�ս��
					if (second_char != 'F' && second_char != '#')
					{
						stack_parse.pop();
						char third_char = stack_parse.top();
						// ���������Ԫ���Ƿ��ս��
						if(third_char=='F')
							count = 3;
						stack_parse.push(second_char);
					}
					// ����ڶ���Ԫ��Ҳ���ս��������
					else
						p = 0;

					stack_parse.push(top_char);
				}
				// ���ջ�����ս��
				else
				{
					char top_char = stack_parse.pop();
					char second_char = stack_parse.top();
					
					// ���������(F)�����
					if (top_char == ')' && second_char == 'F')
					{
						stack_parse.pop();
						char third_char = stack_parse.top();

						if (third_char == '(')
							count = 3;
						stack_parse.push(second_char);
					}
					// �����������(F)������ҵڶ���Ԫ���Ƿ��ս��������
					else if (second_char == 'F')
						p = 0;

					stack_parse.push(top_char);
				}

				// ջ����ջ
				while (count >= 1, count--)
					stack_parse.pop();

				// �ս����ջ
				stack_parse.push('F');

				action = "��Լ";
				string message(parse_str.at(index_left), len);
				action.append(message);

				// ɾ���������еĿɹ�Լ�Ӵ�[index_left, index_right]
				parse_str.erase(index_left, len);
				int temp_left = index_left - 1;
				int temp_right = index_left;

				// ���뱻ɾ���ִ������������ս��֮������ȼ�
				int temp_p = priority(parse_str.at(temp_left), parse_str.at(temp_right));
				parse_str.insert(temp_right, 1, temp_p + '0');

				// ���������������û�����ȼ������P��ʹѭ����ֹ��
				if (temp_p == 0)
					p = 0;
			}
			// ɨ����Ϻ����޷���Լ���ƽ�
			else
			{
				// ʣ�����봮������'#'���ĵ�һ���ַ���ջ
				char c = str.at(0);
				if (c != '#')
				{
					stack_parse.push(c);
					// ɾ��ʣ�����봮�ĵ�һ���ַ�
					str.erase(0, 1);
					action = "�ƽ�";
					action.push_back(stack_parse.top());
				} 
				// ���ʣ�����봮��ʣ��'#'
				else
				{
					p = 0; // ��ֹѭ��
					break; // ��ֹ����ѭ��
				}
			}
			// ������
			change(state_priority);
			cout << cnt << "\t" << state_stack << "\t" << state_str << "\t\t" << state_priority << "\t\t\t" << action << endl;

			// ��β����
			++cnt;
			state_stack = stack_parse.getStackElement();
			state_str = str;
			state_priority = parse_str;

		} // �㷨����

		// �������
		if (p == 0)
			action = "����";
		// ��֤��ʱ���봮ֻʣ��'#'���������봮ֻʣ�¡�#'
		if (!stack_parse.hasOtherChar('F') && str == "#")
			action = "�ɹ�";

		// ������һ�μ�¼
		change(state_priority);
		cout << cnt << "\t" << state_stack << "\t" << state_str << "\t\t" << state_priority << "\t\t\t" << action << endl;

		if (action == "�ɹ�")
			cout << "�ж������" << tempStr << "Ϊ�Ϸ����Ŵ�" << endl;
		else
		{
			cout << "�ж������" << tempStr << "Ϊ�Ƿ����Ŵ�" << endl;
			cout << "ջ��ֻ�С�������ĳ���ս�������봮ʣ�¡���������ʾ�����ɹ����������봮�д�" << endl;
		}
	}
};

int main()
{
	string str;
	V_N_Parse v_n_parse;
	
	cout << "������ȷ������򣬱����ˣ�������		202016040201	���2002" << endl;
	cout << "�ս��֮���������ȼ���ʹ�÷���'!'��ʾ" << endl;
	cout << "����һ��#�����ķ��Ŵ�(�ɰ���+-*/()i#�ȷ��ţ���i+i*i#)��";
	
	cin >> str;
	cout << "�Ը����봮��������ȷ�������������£�" << endl;
	
	v_n_parse.parse(str);

	return 0;
}