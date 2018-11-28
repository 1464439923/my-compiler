// compiler_v0.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "string"
#include "fstream"
#include"cstring"

#define stacksize 500
#define uMax 30000
class cCounter
{
	unsigned int cnt;
public:
	cCounter(void) { cnt = 0; }
	cCounter operator ++()
	{
		if (cnt < uMax)
			cnt++;
		return(*this);
	}
	cCounter operator ++(int dummy)
	{
		cCounter  x = *this;
		if (cnt < uMax)
			cnt++;
		return (x);
	}
	cCounter operator--()
	{//ע������ֵ��˵��Ϊ����&
		if (cnt > 0)
			cnt--;
		return (*this);
	}
	cCounter operator --(int dummy)
	{ //ע������ֵ����˵��Ϊ����&
		cCounter x = *this;
		if (cnt > 0)
			cnt--;
		return (x);
	}
	int operator !()
	{
		return (cnt == 0);
	}
	operator unsigned int()
	{
		return(cnt);
	}
};
ifstream keywordfile;
cCounter counter;
extern int getsym(char & the_single_char_readed_now, int &);
char fname[20];
string keyword = "";
fstream sym_tab;
fstream osym_tab;
fstream iint_tab;
fstream oint_tab;
ifstream sourcefile;
int space_used;
int iaddress;
int insert(string type, string var_name);
int lookint(int integer);
int lookup(string id);
string NEWT(void);
string _add(string y, string z, string t);
string _divided(string y, string z, string t);
string _mult(string y, string z, string t);
string _sub(string y, string z, string t);
int S(char&ch, int &valueofsymbol, string &returned, int &ireturned);
int L(char&ch, int&valueofsymbol, string &returned, int &ireturned);
int LL(char&ch, int&valueofsymbol, string &returned, int &ireturned);
int B(char&ch, int&valueofsymbol, string &returned, int &ireturned);
int T(char &ch, int &valueofsymbol, string &returned, int &ireturned);
int E(char&ch, int&valueofsymbol, string &returned, int &ireturned);
int F(char& ch, int &valueofsymbol, string &returned, int &ireturned);
int TT(char &ch, int&valueofsymbol, string &returned, int &ireturned);
int FF(char &ch, int&valueofsymbol, string &returned, int &ireturned);
int F(char& ch, int &valueofsymbol, string &returned, int &ireturned)
{
	if (valueofsymbol == 40)// (
	{
		getsym(ch, valueofsymbol);
		if (!E(ch, valueofsymbol, returned, ireturned))return 0;
		if (valueofsymbol != 41)
		{
			cout << "Error17: equation���ұ��ڴ����֣�";
			return 0;
		}
		getsym(ch, valueofsymbol);
		return 1;
	}
	if (valueofsymbol == 50)//NUM
	{
		getsym(ch, valueofsymbol);
		return 1;
	}
	if (valueofsymbol == 49)//ID
	{
		getsym(ch, valueofsymbol);
		return 1;
	}
	cout << "Error16: �˴��ڴ����֣� NUM ID ";
	return 0;
}
int LL(char&ch, int&valueofsymbol, string &returned, int &ireturned)
{
	if (valueofsymbol == 59)
	{
		getsym(ch, valueofsymbol);
		if (!L(ch, valueofsymbol, returned, iret)) return 0;
		return 1;
	}
	if (valueofsymbol != 125)
	{
		cout << "Error14: �������ȱ'}'";
		return 0;
	}
	//�˴�����Ҫ��ǰ����������symbol��Ϊ����δ���룬ֻ�ǳ�ǰ�۲�
	return 1;
}
int D(char &ch, int &valueofsymbol, string &returned, int &ireturned)
{
	if (valueofsymbol == 65 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)//int char float double long short signed
	{
		insert(keyword, NEWT());//���뵽���ű�
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error5: �������ȱ�ٱ�����";//����0 ����������
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 59)//��
		{
			cout << "Error5: �������ȱ�١�����";//����0 ����������
			return 0;
		}
		getsym(ch, valueofsymbol);
		while (valueofsymbol == 65 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)
		{
			insert(keyword, NEWT());//���뵽���ű�
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 49)
			{
				cout << "Error6:�������ȱ�ٱ�����";
				return 0;
			}
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 59)//��
			{
				cout << "Error5: �������ȱ�١�����";//����0 ����������
				return 0;
			}
			getsym(ch, valueofsymbol);
		}
		cout << "������������ȷ��" << endl;
		return 1;
	}
	else
	{
		cout << "Error4: �������δ��int char float double long short signed��ʼ";
		return 0;
	}
}
int T(char &ch, int &valueofsymbol, string &returned, int &ireturned)
{
	if (!F(ch, valueofsymbol))return 0;
	while (valueofsymbol == 42 || valueofsymbol == 47)//* /
	{//if valueo == 42 �ǳˡ�����
		getsym(ch, valueofsymbol);
		if (!F(ch, valueofsymbol))return 0;
	}//�Ƿ��ǼȲ���42 �ֲ���47�����ֲ���follow��������
	return 1;
}
int E(char&ch, int&valueofsymbol, string &returned, int &ireturned)
{
	if (!T(ch, valueofsymbol))return 0;
	while (valueofsymbol == 43 || valueofsymbol == 45)//+ -
	{
		getsym(ch, valueofsymbol);
		if (!T(ch, valueofsymbol))return 0;
	}
	return 1;//���û��+����-Ҳ�ǿ��Ե�
}
int L(char&ch, int&valueofsymbol, string &returned, int &ireturned)
{
	if (!S(ch, valueofsymbol))return 0;
	if (!LL(ch, valueofsymbol))return 0;
	return 1;
}
int FF(char &ch, int&valueofsymbol, string &returned, int &ireturned)
{
	if (valueofsymbol != 49)//ID
	{
		cout << "Error17: �߼����������ǰȱ�ٱ���";
		return 0;
	}
	getsym(ch, valueofsymbol);
	if (valueofsymbol == 23 || valueofsymbol == 24 || valueofsymbol == 25 || valueofsymbol == 26)//��ʵ���еȺ� ���в��Ⱥ�
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error18: �߼��������������ȱ�ٱ���";
			return 0;
		}
		getsym(ch, valueofsymbol);
		return 1;
	}
}
int TT(char &ch, int &valueofsymbol, string &returned, int &ireturned)
{
	if (!FF(ch, valueofsymbol)) return 0;
	while (valueofsymbol == 17)//and
	{
		getsym(ch, valueofsymbol);
		if (!FF(ch, valueofsymbol))return 0;
	}
	return 1;
}
int B(char&ch, int&valueofsymbol, string &returned, int &ireturned)
{
	if (!TT(ch, valueofsymbol)) return 0;
	while (valueofsymbol == 18)//or
	{
		getsym(ch, valueofsymbol);
		if (!TT(ch, valueofsymbol))return 0;
	}
	return 1;
}
int S(char&ch, int &valueofsymbol, string &returned, int &ireturned)
{
	if (valueofsymbol == 49)
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 61)
		{
			cout << "Error7: ��ֵ���ȱ�١�=��";
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!E(ch, valueofsymbol)) return 0;
		counter++;
		cout << "���" << counter << "������ȷ" << endl;//��дһ��������������x��
		return 1;
	}
	if (valueofsymbol == 123)//{
	{
		getsym(ch, valueofsymbol);
		if (!L(ch, valueofsymbol))return 0;
		if (valueofsymbol != 125)
		{
			cout << "Error8: �������ȱ�٣�" << endl;
			return 0;
		}
		getsym(ch, valueofsymbol);
		counter++;
		string x = "���";
		string y = "������ȷ";
		cout << x + char(counter + 0x30) + y << endl;
		return 1;
	}
	if (valueofsymbol == 88)//while
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 40)//(
		{
			cout << "Error9: while����ȱ�١�����";
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!B(ch, valueofsymbol))return 0;
		if (valueofsymbol != 41)//)
		{
			cout << "Error10: while ����ж���������ȱ�١�����" << endl;
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (valueofsymbol == 87)//do
		{
			getsym(ch, valueofsymbol);
			if (!S(ch, valueofsymbol)) return 0;
			cout << "while������ɹ�" << endl;
			return 1;
		}
		else
		{
			cout << "Error11�� while���ȱ��do" << endl;
			return 0;
		}
	}
	if (valueofsymbol == 68)
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 40)//(
		{
			cout << "Error19: if����ȱ�١�����";
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!B(ch, valueofsymbol))return 0;
		if (valueofsymbol != 41)//)
		{
			cout << "Error20: if ����ж���������ȱ�١�����" << endl;
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (valueofsymbol == 19)//then
		{
			getsym(ch, valueofsymbol);
			if (!S(ch, valueofsymbol)) return 0;
			if (valueofsymbol == 71)//else
			{
				getsym(ch, valueofsymbol);
				if (!S(ch, valueofsymbol)) return 0;
			}
			cout << "if������ɹ�" << endl;
			return 1;
		}
	}
	{//�������Ķ�û�г���
		cout << "Error12: ���������������У�,ID,while,if�е�һ��" << endl;
		return 0;
	}
}
int P(char &ch, int &valueofsymbol, string &returned, int &ireturned)//��ʼʹ�ó�ǰ���� ��ǰ����һ��symbol
{
	getsym(ch, valueofsymbol);
	if (valueofsymbol == 123)//{
	{
		getsym(ch, valueofsymbol);
		if (!D(ch, valueofsymbol)) return 0;///////ready to be changed
		if (!S(ch, valueofsymbol)) return 0;

		if (valueofsymbol != 125)//}
		{
			cout << "Error3: �������ȱ�٣�";
			return 0;
		}
		getsym(ch, valueofsymbol);
		//#����
		return 1;
	}
	else
	{
		cout << "Error2: ����ʼȱ�٣�";
		return 0;
	}
}
string NEWT(void)
{
	static cCounter name_count;
	static const string tname = "t";
	string temporary_var;
	temporary_var = tname + char((name_count++) + 0x30);
	return temporary_var;
}
string _add(string y, string z, string t)
{
	return "(+," + y + "," + z + "," + t + ")";
}
string _mult(string y, string z, string t)
{
	return "(*," + y + "," + z + "," + t + ")";
}
string _sub(string y, string z, string t)
{
	return "(-," + y + "," + z + "," + t + ")";
}
string _divided(string y, string z, string t)
{
	return "(/," + y + "," + z + "," + t + ")";
}
int lookup(string id)//wait
{
	string str1;
	string str2;
	int str3;
	sym_tab.open("symbol_table.txt");
	while (!sym_tab.eof())
	{
		sym_tab >> str1;
		sym_tab >> str2;
		sym_tab >> str3;
		if (id == str2)
		{
			sym_tab.close();
			return str3;
		}
	}
	cout << "Error_x1: ��������������ڱ���δ�����ʹ��";
	sym_tab.close();
	return -1;//׼�����������ף�������һ����==-1
}
int insert(string type, string var_name)//wait
{
	osym_tab.seekp(0, ios::end);
	osym_tab << endl;//�ļ��������ӣ�ѡ��space_used ���ȫ�ֱ�����ÿ��insertʱ��2������
	osym_tab << type << " " << var_name << " " << space_used;
	osym_tab.flush();//�������������⣬��ȷ��flush����ʹ�ú��Ч��������������˵������ˢ�»�������������У����Թرպ��ٴ�һ���ļ�
	space_used += 2;
	return 1;
}
int lookint(int integer)/////////////////////////////////////����д������д��pos ���񷵻���2 �����������loopupҲ�п��ܴ��ˡ�
{
	int inttab;
	int postab;
	iint_tab.open("����������.txt");
	oint_tab.seekp(0, ios::end);
	while (!iint_tab.eof())
	{
		iint_tab >> inttab;
		iint_tab >> postab;
		if (integer == inttab)
		{
			iint_tab.close();
			return postab;
		}
	}
	oint_tab << endl;
	oint_tab << integer << " " << iaddress;
	oint_tab.flush();
	iaddress += 2;
	iint_tab.close();
	return iaddress - 2;
}//////////////////////////////////////////����ķ�����ڵ�ַ��ָiaddress�𣬻����ڱ��еĶ�λ�����Ժ��ҵ����
void closethefile()
{
	oint_tab.close();
	osym_tab.close();
	ofstream toclearthefile("symbol_table.txt");//����ļ�
	toclearthefile.close();
	toclearthefile.open("����������.txt");
	toclearthefile.close();
	keywordfile.close();
	sourcefile.close();
}
int main()//if ��仹û��ʵ��
{
	string str;
#ifdef VER1
	cout << "this is version1";
#endif
	space_used = 0;///////////////////////////////////////////////////veryimportant//////////////////anothor thing is ififififififiififf you have forget to write the if statement ifififiifififififi
	iaddress = 0;
	osym_tab.open("symbol_table.txt");//'///////waitting
	oint_tab.open("����������.txt");
	/////////////////////////////////////////////////////////
	keywordfile.open("keyword.txt");
	printf("�����ļ���");//����keyword�ļ�����Ŀ����ԣ�������С�ĸ��Ĺ�����֪����û�ж���Ŀո� tab �س�����
	cout << endl;
	//cin >> fname;
	sourcefile.open("c.cpp");
	if (sourcefile)
	{
		char ch;
		sourcefile.get(ch);
		int valueofsymbol;
		if (!P(ch, valueofsymbol))
		{
			int x;
			cin >> x;//for testing
			closethefile();
			return 0;
		}
		if (valueofsymbol != 35) { cout << "Error17: ȱ���ļ���������#"; int x; cin >> x;  closethefile(); return 0; }
		getsym(ch, valueofsymbol);//�ĵ��Ѿ�������#���ź�������������κη��ţ���᷵��-128�����򣬷����Ǹ�����
		if (valueofsymbol != -128) { cout << "Error18: #���ź���δ��������"; int x; cin >> x; closethefile(); return 0; }
		cout << "succeed in compiling"; //relese�汾�Ƿ񲻻���ͣ��
		int x;
		cout << endl;
		cin >> x;//for testing
		closethefile();
		return 1;
	}
	else
	{
		cout << "Error: Can't open the file" << endl;
		closethefile();
		return 0;
	}
}