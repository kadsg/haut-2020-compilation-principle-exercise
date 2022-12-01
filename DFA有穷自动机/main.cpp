#include <iostream>
#include <string>

using namespace std;

// ״̬��
// S����̬
// L����ʶ�� 
// Z������0 
// N����0�������
// '!'��Ϊ������
const char Z[] = {'!', 'S', 'L', 'Z', 'N' };
const int LEN_Z = 5;

// ������E��ʾ����
const char E = 'E';
// ������I����Ƿ�
const char I = 'I';

// ��ĸ��
// '!'��Ϊ������
const char L[] = { '!', '1', '2', '3', '0', 'a','b','c' };
const int LEN_L = 8;

// ת�ƾ���
char matrix[5][8]
{
	'!', '1', '2', '3', '0', 'a', 'b', 'c',
	'S', 'N', 'N', 'N', 'Z', 'L', 'L', 'L',
	'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L',
	'Z', 'E', 'E', 'E', 'E', 'E', 'E', 'E',
	'N', 'N', 'N', 'N', 'N', 'E', 'E', 'E'
};

// ��Ϣ��
typedef struct Info
{
	// �ֱ�
	char type;
	// ֵ
	string value;
}Info;

// �����ַ�a������c�е�λ��
// �����ڣ��Ƿ����룩����-1
int location(const char* c, const char a, const int len)
{
	for (int i = 0; i < len; ++i)
	{
		if (c[i] == a)
			return i;
	}
	return -1;
}

// ״̬ת������
// state��״̬
// c��������ĸ
// ����ת�����״̬
char f(char state, char c)
{
	// �ȼ���Ƿ��ǺϷ�����
	if (location(L, c, LEN_L) == -1)
	{
		return I;
	}

	int x = location(Z, state, LEN_Z);
	int y = location(L, c, LEN_L);

	return matrix[x][y];
}

// �ʷ���������
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
	// ������
	int n;
	char type;
	string str;
	string result;
	Info info;

	cout << "202016040201" << "       " << "������" << endl << endl;

	while(1)
	{
		// ���뵥��
		cin >> str;
		
		// ����
		info = parse(str);
		
		type = info.type;
		str = info.value;

		cout << str << "��";

		if (type == E) result = "������";
		if (type == I) result = "�Ƿ�������";
		if (type == 'L') result = "��ʶ��";
		if (type == 'Z') result = "����0";
		if (type == 'N') result = "��0���������";

		cout << result << endl << endl;
	}
}

