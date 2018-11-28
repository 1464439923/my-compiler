// compiler_v0.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "string"
#include "fstream"
#include"cstring"

#define STACKSIZE 500
#define uMax 30000
//ע��flag��0��ʾ���ص��ǵ�һ������ֵ����string 
class cCounter
{
	unsigned int cnt;
public:
	cCounter(void) { cnt = 0; }
	cCounter(int x) { cnt = x; }
	void zerosetting(void) { cnt = 0; }
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
cCounter name_count;
ifstream keywordfile;
fstream func_tab;
ofstream assembler("�����assembler.txt");
int stack[STACKSIZE];
cCounter counter;
extern int getsym(char & the_single_char_readed_now,int &);
char fname[20];
string keyword="";
fstream sym_tab;
fstream osym_tab;
fstream iint_tab;
fstream oint_tab;
ifstream sourcefile;
int space_used;
int iaddress;
int tnumber;
int linelabel=0;
int fly_lookup;
int sp;
int insert(string type, string var_name);
int lookint(int integer);
int lookup(string id);
string NEWT(void);
string _add(string y, string z, string t);
string _divided(string y, string z, string t);
string getlabel();
string _mult(int y, string z, string t);
string _mult(string y, int z, string t);
string _mult(int y, int z, string t);
string _mult(string y, string z, string t);
string _return(string return_v);

string _WgenJP(string label, int retention_linelabel);
string _genJZ(string E, string label);
string _genJP(string label);
string _add(int y, string z, string t);
string _add(string y, int z, string t);
string _add(int y, int z, string t);
string _sub(int y, string z, string t);
string _sub(string y, int z, string t);
string _sub(int y, int z, string t);
string _equal(string y, string z);
string _equal(string y, int z);
string _divided(int y, string z, string t);
string _divided(string y, int z, string t);
string _divided(int y, int z, string t);
string _sub(string y, string z, string t);
string _relop(string y, string z, string t, string relop);
string _and(string y, string z, string t);
string _or(string y, string z, string t);
string _print(string y);
string _call(string func, string parameter);
string _par(string parameter);
string _accept(string varible);
int insert_func_tab(string func_name, string return_type, int entry_address, int num_formal_para, int entry_address_of_varible, int lens_of_varible,int end_of_varible);
int insert_func(string, string);
int int2str(int num, char *des, int des_len);
void zipper_backfilling(string label1, int linelabel)
{
	fstream backlabel("�����assembler.txt");
	fstream write("�����assembler.txt");
	char label[20];
	while (!backlabel.eof())//���ǵ�����ʱ���ļ������Ƿ��Ӱ��ԭ����assemblerд����
	{
		backlabel >> label;
		if (label == label1)
		{
			write.seekp(backlabel.tellg(), ios::beg);
			int size = strlen(label);
			write.seekp(-size, ios::cur);
			int2str(linelabel, label, sizeof(label));
			int i = strlen(label);
			while (size > i)
			{
				write << " ";
				size--;
			}
			write << linelabel;
			write.flush();
			break;
		}
	}
	write.close();
	backlabel.close();
}
int S(char&ch, int &valueofsymbol,string &returned,int &ireturned,int &flag);
int L(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int LL(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int B(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int T(char &ch, int &valueofsymbol, string &returned,int &ireturned,int &flag);
int E(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int F(char& ch, int &valueofsymbol, string &returned,int &ireturned,int &flag);
int TT(char &ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int FF(char &ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int int2str(int num, char *des, int des_len)
{
	int count = 0;
	int tmp = num;
	// ��ȡ����λ��
	while (tmp /= 10)
	{
		count++;
	}
	if (num < 0)	// ����
	{
		if (des_len < count + 2)	// Ŀ�껺�������Ȳ���
			return -1;
		*(des + count + 2) = '\0';	// �ַ���������־
		*des = '-';	// ��������
		num = -num;	// תΪ����
		des++;
	}
	else	// �Ǹ���
	{
		if (des_len < count + 1)	// Ŀ�껺�������Ȳ���
			return -1;
		*(des + count + 1) = '\0';	// �ַ���������־
	}
	// �ַ�����ֵ
	while (count >= 0)
	{
		*(des + count) = num % 10 + '0';
		num /= 10;
		count--;
	}
	return 1;
}
int F(char& ch, int &valueofsymbol, string &returned,int &ireturned,int &flag)
{
	if (valueofsymbol == 40)// (
	{
		getsym(ch, valueofsymbol);
		if (!E(ch, valueofsymbol,returned,ireturned,flag))return 0;//���E���ص���һ��������FҲ�������֣�flag��E����ͬ��
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
		ireturned = tnumber;
		lookint(ireturned);
		getsym(ch, valueofsymbol);
		flag = 1;
		return 1;
	}
	if (valueofsymbol == 49)//ID
	{
		flag = 0;
		returned = keyword;
		if(lookup(keyword)==-1)return 0;
		getsym(ch,valueofsymbol);
		return 1;
	}
	cout << "Error16: �˴��ڴ����֣� NUM ID ";
	return 0;
}
int LL(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag)
{
	if (valueofsymbol == 59)
	{
		getsym(ch, valueofsymbol);
		if(!L(ch, valueofsymbol,returned,ireturned,flag)) return 0;
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
int D(char &ch,int &valueofsymbol, string &returned,int &ireturned,int &flag)
{
	string keyword_temp;
	if (valueofsymbol == 65|| valueofsymbol == 66|| valueofsymbol == 67|| valueofsymbol == 69|| valueofsymbol == 72|| valueofsymbol == 78||valueofsymbol==82)//int char float double long short signed
	{
		keyword_temp = keyword;
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error5: �������ȱ�ٱ�����";//����0 ����������
			return 0;
		}
		insert(keyword_temp, keyword);//���뵽���ű�
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 59)//��
		{
			cout << "Error5: �������ȱ�١�����";//����0 ����������
			return 0;
		}
		getsym(ch, valueofsymbol);
		while (valueofsymbol == 65 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)
		{
			keyword_temp = keyword;
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 49)
			{
				cout << "Error6:�������ȱ�ٱ�����";
				return 0;
			}
			insert(keyword_temp, keyword);//���뵽���ű�
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 59)//��
			{
				cout << "Error5: �������ȱ�١�����";//����0 ����������
				return 0;
			}
			getsym(ch, valueofsymbol);
		}
		cout << "������������ȷ��"<<endl;
		return 1;
	}
	else
	{
		cout << "Error4: �������δ��int char float double long short signed��ʼ";
		return 0;
	}
}
int O(char &ch, int &valueofsymbol, string &returned, int &ireturned, int &flag,int &formal_para_num)
{
	string keyword_temp;
	formal_para_num = 0;
	if (valueofsymbol == 65 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)//int char float double long short signed
	{
		keyword_temp = keyword;
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error22: ����������ȱ�ٱ�����";//����0 ����������
			return 0;
		}
		insert(keyword_temp, keyword);//���뵽���ű�
		formal_para_num++;
		getsym(ch, valueofsymbol);
		while (valueofsymbol==44)//,
		{
			getsym(ch, valueofsymbol);
			if (!(valueofsymbol == 65 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82))
			{
				cout << "Error23: ����������,����ȱ��ȱ�ٱ�������";//����0 ����������
				return 0;
			}
			keyword_temp = keyword;
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 49)
			{
				cout << "Error6:�������ȱ�ٱ�����";
				return 0;
			}
			insert(keyword_temp, keyword);//���뵽���ű�
			formal_para_num++;
			getsym(ch, valueofsymbol);
		}
		cout << "������������������ȷ��" << endl;
		return 1;
	}
	else
	{
		cout << "Error4: �������δ��int char float double long short signed��ʼ";
		return 0;
	}
}
int T(char &ch, int &valueofsymbol, string &returned,int &ireturned,int &flag)
{
	if (!F(ch, valueofsymbol,returned,ireturned,flag))return 0;
	string str = returned;
	string tstr;
	bool onlyoneF = true;
	int i = ireturned,tflag=flag;
	while (valueofsymbol == 42|| valueofsymbol == 47)//* /
	{//if valueo == 42 �ǳˡ�����
		if (valueofsymbol == 42)//*
		{
			getsym(ch, valueofsymbol);
			if (!F(ch, valueofsymbol, returned, ireturned, flag))return 0;
			if (tflag == 0)
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _mult(str,returned,tstr);
					insert("int",tstr);                    //�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
				else
				{
					tstr = NEWT();
					cout << _mult(str, ireturned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
			}
			else
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _mult(i, returned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
				else
				{
					tstr = NEWT();
					cout << _mult(i, ireturned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
			}
			cout << endl;
		}
		else
		{
			getsym(ch, valueofsymbol);
			if (!F(ch, valueofsymbol, returned, ireturned, flag))return 0;
			if (tflag == 0)
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _divided(str, returned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
				else
				{
					tstr = NEWT();
					cout << _divided(str, ireturned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
			}
			else
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _divided(i, returned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
				else
				{
					tstr = NEWT();
					cout << _divided(i, ireturned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
			}
			cout << endl;
		}
		tflag = 0;
		str = tstr;
		onlyoneF = false;
	}//�Ƿ��ǼȲ���42 �ֲ���47�����ֲ���follow��������             �ҽ�� �������ұ߽��
	if(onlyoneF)	return 1;
	else { flag = 0; returned = str; return 1; }
}
int E(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag)//E��T{+T| -T}
{
	if(!T(ch, valueofsymbol,returned,ireturned,flag))return 0;
	string str = returned;
	string tstr;
	bool onlyoneE = true;
	int i = ireturned, tflag = flag;
	while(valueofsymbol == 43|| valueofsymbol == 45)//+ -
	{
		if (valueofsymbol == 43)//+
		{
			getsym(ch, valueofsymbol);
			if (!T(ch, valueofsymbol, returned, ireturned, flag))return 0;
			if (tflag == 0)
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _add(str, returned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
				else
				{
					tstr = NEWT();
					cout << _add(str, ireturned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
			}
			else
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _add(i, returned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
				else
				{
					tstr = NEWT();
					cout << _add(i, ireturned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
			}
			cout << endl;
		}
		else
		{
			getsym(ch, valueofsymbol);
			if (!T(ch, valueofsymbol, returned, ireturned, flag))return 0;
			if (tflag == 0)
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _sub(str, returned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
				else
				{
					tstr = NEWT();
					cout << _sub(str, ireturned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
			}
			else
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _sub(i, returned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
				else
				{
					tstr = NEWT();
					cout << _sub(i, ireturned, tstr);
					insert("int", tstr);//�����Ժ���������str returned���������;����������int ���ǡ���float���������ʱ�ĸ��� ����任��
				}
			}
			cout << endl;
		}
		tflag = 0;
		str = tstr;
		onlyoneE = false;
	}	
	if (onlyoneE)	return 1;
	else { flag = 0; returned = str; return 1; }//���û��+����-Ҳ�ǿ��Ե�
}
int L(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag)
{
	if (!S(ch, valueofsymbol,returned,ireturned,flag))return 0;
	if (!LL(ch, valueofsymbol,returned,ireturned,flag))return 0;
	return 1;
}
int FF(char &ch, int&valueofsymbol, string &returned,int &ireturned,int &flag)//����ʱ����������д�ıȽϼ���
{
	string str;
	string tstr;
	if (valueofsymbol != 49)//ID
	{
		cout << "Error17: �߼����������ǰȱ�ٱ���";
		return 0;
	}
	if(lookup(keyword)==-1)return 0;
	returned = keyword;
	str = keyword;
	getsym(ch, valueofsymbol);
	if (valueofsymbol == 23 || valueofsymbol == 24 || valueofsymbol == 25 || valueofsymbol == 26||valueofsymbol==21||valueofsymbol==22)//��ʵ���еȺ� ���в��Ⱥ�
	{
		int temp = valueofsymbol;
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error18: �߼��������������ȱ�ٱ���";
			return 0;
		}
		if (lookup(keyword) == -1)return 0;
		switch (temp) //<= 23	< 24	>= 25	> 26    := 21(�еȺ�)      : 22
		{
			case 21://�¼���21 ��22 ���ܳ�������
			{
				tstr = NEWT();
				cout << _relop(str, keyword, tstr, ":=");
				insert("int", tstr);
				break; 
			}
			case 22:
			{
				tstr = NEWT();
				cout << _relop(str, keyword, tstr, ":");
				insert("int", tstr);
				break;
			}
			case 23:
			{
				tstr = NEWT();
				cout<<_relop(str, keyword, tstr, "<=");
				insert("int", tstr);
				break;
			}
			case 24:
			{
				tstr = NEWT();
				cout << _relop(str, keyword, tstr, "<");
				insert("int", tstr);
				break;
			}
			case 25:
			{
				tstr = NEWT();
				cout << _relop(str, keyword, tstr, ">=");
				insert("int", tstr);
				break;
			}
			case 26:
			{
				tstr = NEWT();
				cout << _relop(str, keyword, tstr, ">");
				insert("int", tstr);
				break;
			}
		}
		cout << endl;
		getsym(ch, valueofsymbol);
		flag = 0;
		returned = tstr;
		return 1;
	}
	flag = 0;
	return 1;
}
int TT(char &ch, int &valueofsymbol, string &returned,int &ireturned,int &flag)//T�� ��F�� {�� F�� } 
{
	if (!FF(ch, valueofsymbol,returned,ireturned,flag)) return 0;
	string str = returned;
	string tstr;
	while (valueofsymbol == 17)//and
	{
		getsym(ch, valueofsymbol);
		if (!FF(ch, valueofsymbol,returned,ireturned,flag))return 0;
		tstr = NEWT();
		cout << _and(str, returned, tstr);
		insert("int", tstr);
		str = tstr;
		cout << endl;
	}
	flag = 0;
	returned = str;
	return 1;
}
int B(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag)//B��T��{	��T��}
{
	if (!TT(ch, valueofsymbol,returned,ireturned,flag)) return 0;
	string str = returned;
	string tstr;
	while (valueofsymbol == 18 )//or
	{
		getsym(ch, valueofsymbol);
		if (!TT(ch, valueofsymbol,returned,ireturned,flag))return 0;
		tstr = NEWT();
		cout << _or(str, returned, tstr);
		insert("int", tstr);
		str = tstr;
		cout << endl;
	}
	flag = 0;
	returned = str;
	return 1;
}
int Y(char&ch, int&valueofsymbol, string &returned, int &ireturned, int &flag)
{//Y=ID(E{,E}) 
	string func,parameter;
	int num_of_parameter=0;
	if (valueofsymbol != 49)
	{
		cout << "Error27: ��������ȱ�ٺ�����";
		return 0;
	}
	func = keyword;//����Y��ĵ�һ��ID�Ƿ��岻��Ҫlookup��Ϊ��S�ڵ���Yʱ�Ѿ�ȷ������ID�����Ѷ��壬���һ�ȷ���Ǻ�����
	getsym(ch, valueofsymbol);
	if (valueofsymbol != 40)//(
	{
		cout << "Error28: ����������ȱ�١�����";
		return 0;
	}
	getsym(ch, valueofsymbol);
	if (!E(ch, valueofsymbol, returned, ireturned, flag))return 0;
	
	if (flag == 0)//E�ַ�������ֵ
	{
		parameter = returned;
	}
	else
	{
		char b[11];
		int2str(ireturned, b, sizeof(b));
		parameter = b;
	}
	cout << _par(parameter);
	cout << endl;
	num_of_parameter++;
	while (valueofsymbol == 44)//,
	{
		getsym(ch, valueofsymbol);
		if (!E(ch, valueofsymbol, returned, ireturned, flag))return 0;
		if (flag == 0)//E�ַ�������ֵ
		{
			parameter =returned;
		}
		else
		{
			char b[11];
			int2str(ireturned, b, sizeof(b));
			parameter = b;
		}
		num_of_parameter++;
		cout << _par(parameter);
		cout << endl;
	}
	if (valueofsymbol != 41)//)
	{
		cout << "Error10: �������ú���ȱ�١�����" << endl;
		return 0;
	}
	char b[20];
	int2str(num_of_parameter, b, sizeof(b));
	cout << _call(func, b);
	cout<< endl;
	getsym(ch, valueofsymbol);
	flag = 0;
	return 1;
}
int S(char&ch,int &valueofsymbol, string &returned,int &ireturned,int &flag)
{
	int temp1;
	if (valueofsymbol == 49)
	{
		temp1 = fly_lookup;
		fly_lookup = 0;
		if (lookup(keyword) == -2)
		{
			fly_lookup = temp1;
			if (!Y(ch, valueofsymbol, returned, ireturned, flag)) return 0;
		}
		else
		{
			fly_lookup = temp1;
			string str = keyword;
			string tstr;
			if (lookup(keyword) == -1)
				return 0;//check the lookup after ID
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 61)
			{
				cout << "Error7: ��ֵ���ȱ�١�=��";
				return 0;
			}
			getsym(ch, valueofsymbol);
			if (valueofsymbol == 49)
			{
				temp1 = fly_lookup;//fly_lookup���õ�Ŀ����Ϊ����lookup ��ȫ�������������;ֲ�����������֮���л�����������3�ֲ���Ϊȫ�֣����������ı任
				fly_lookup = 0;
				if (lookup(keyword) == -2)
				{
					fly_lookup = temp1;
					if (!Y(ch, valueofsymbol, returned, ireturned, flag)) return 0;
					cout << _accept(str);
				}
				else
				{
					fly_lookup = temp1;
					if (!E(ch, valueofsymbol, returned, ireturned, flag)) return 0;
					if (flag == 0)
						cout << _equal(str, returned);
					else
						cout << _equal(str, ireturned);
				}
			}
			else {
				if (!E(ch, valueofsymbol, returned, ireturned, flag)) return 0; 
				if (flag == 0)
					cout << _equal(str, returned);
				else
					cout << _equal(str, ireturned);
			}
			cout << endl;
			counter++;
			cout << "���" << counter << "������ȷ" << endl;//��дһ��������������x��
		}
		return 1;
	}
	if (valueofsymbol == 123)//{
	{
		getsym(ch, valueofsymbol);
		if (!L(ch, valueofsymbol,returned,ireturned,flag))return 0;
		if (valueofsymbol != 125)
		{
			cout << "Error8: �������ȱ�٣�" << endl;
			return 0;
		}
		getsym(ch, valueofsymbol);
		counter++;
		cout << "���" << counter << "������ȷ" << endl;//��дһ��������������x��
		return 1;
	}
	if (valueofsymbol == 86)
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)
		{
			cout << "Error21: print ������������һ��ID" << endl;
			return 0;
		}
		else {
			if (lookup(keyword) == -1)
				return 0;//check the lookup after ID
			cout << _print(keyword);
			cout << endl;
			counter++;
			cout << "���" << counter << "������ȷ" << endl;//��дһ��������������x��
		}
		getsym(ch, valueofsymbol);
		return 1;
	}
	if (valueofsymbol == 88)//while
	{
		string label1 = getlabel();
		cout << label1 + ":" << endl;
		int retention_linelabel = linelabel;
		zipper_backfilling(label1, linelabel);
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 40)//(
		{
			cout << "Error9: while����ȱ�١�����";
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!B(ch, valueofsymbol,returned,ireturned,flag))return 0;
		if (valueofsymbol != 41)//)
		{
			cout << "Error10: while ����ж���������ȱ�١�����" << endl;
			return 0;
		}
		string label2 = getlabel();
		cout<<_genJZ(returned, label2)<<endl;
		getsym(ch, valueofsymbol);
		if (valueofsymbol == 87)//do
		{
			getsym(ch, valueofsymbol);
			if (!S(ch, valueofsymbol,returned,ireturned,flag)) return 0;
			cout << _WgenJP(label1,retention_linelabel) << endl;
			cout << label2 + ":" << endl;
			zipper_backfilling(label2, linelabel);
			cout << "while������ɹ�" << endl;
			return 1;
		}
		else
		{
			cout << "Error11�� while���ȱ��do" << endl;
			return 0;
		}
	}
	if (valueofsymbol == 68)//if
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 40)//(
		{
			cout << "Error19: if����ȱ�١�����";
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!B(ch, valueofsymbol,returned,ireturned,flag))return 0;
		if (valueofsymbol != 41)//)
		{
			cout << "Error20: if ����ж���������ȱ�١�����" << endl;
			return 0;
		}
		string label1=getlabel();
		cout<<_genJZ(returned,label1);
		cout << endl;
		getsym(ch, valueofsymbol);
		if (valueofsymbol == 19)//then
		{
			getsym(ch, valueofsymbol);
			if (!S(ch, valueofsymbol,returned,ireturned,flag)) return 0;
			string label2 = getlabel();
			cout<<_genJP(label2);
			cout << endl;

			assembler.flush();
			cout << label1 + ":" << endl;
			//assembler << label1 + ":" << endl; 
			zipper_backfilling(label1, linelabel);
			////////////////////////////////////////////

			if (valueofsymbol == 71)//else
			{
				getsym(ch, valueofsymbol);
				if (!S(ch, valueofsymbol,returned,ireturned,flag)) return 0;
			}
			cout << label2 + ":" << endl;//////////////////////////////////////////////////
			zipper_backfilling(label2, linelabel);
			cout << "if������ɹ�" << endl;
			return 1;
		}
	}
	if (valueofsymbol == 76)//return
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)
		{
			cout << "Error29: return ������������һ��ID" << endl;
			return 0;
		}
		if (lookup(keyword) == -1)
			return 0;//check the lookup after ID
		cout << _return(keyword);
		cout << endl;
		getsym(ch, valueofsymbol);
		return 1;
	}
	{//�������Ķ�û�г���
		cout << "Error12: ���������������У�,ID,while,if�е�һ��" << endl;
		return 0;
	}
}
int P(char &ch, int &valueofsymbol, string &returned,int &ireturned,int &flag)//��ʼʹ�ó�ǰ���� ��ǰ����һ��symbol
{
	//getsym(ch, valueofsymbol);
	if (valueofsymbol == 123)//{
	{
		getsym(ch, valueofsymbol);
		if (!D(ch, valueofsymbol,returned,ireturned,flag)) return 0;///////ready to be changed
		if (!S(ch, valueofsymbol,returned,ireturned,flag)) return 0;

		if (valueofsymbol != 125)//}
		{
			cout << "Error3: �������ȱ�٣�";
			return 0;
		}
		getsym(ch, valueofsymbol);
		//#����//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		return 1;
	}
	else
	{
		cout << "Error2: ����ʼȱ�٣�";
		return 0;
	}
}
int X(char &ch, int &valueofsymbol, string &returned, int &ireturned, int &flag)
{
	string keyword_temp;
	getsym(ch, valueofsymbol);
	if (valueofsymbol == 65 || valueofsymbol==83||valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)//int char float double long short signed
	{//X ��int ID(O) P { int ID(O) P }
		keyword_temp = keyword;
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error24: ȱ�ٺ�����";//����0 ����������
			return 0;
		}
		insert_func(keyword_temp, keyword);//���뵽���ű�
		int entry_of_varible = osym_tab.tellp();
		fly_lookup = entry_of_varible;
		string func_name = keyword;
		string return_type = keyword_temp;
		int entry_address = linelabel;
		int end_of_varible = 0;
		keyword_temp = keyword;
		name_count.zerosetting();
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 40)//(
		{
			cout << "Error25: ����������ȱ�١�����";
			return 0;
		}
		getsym(ch, valueofsymbol);
		int formal_para_num=0;
		if (!O(ch, valueofsymbol, returned, ireturned, flag,formal_para_num))return 0;
		if (valueofsymbol != 41)//)
		{
			cout << "Error26: ����������ȱ�١�����" << endl;
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!P(ch, valueofsymbol, returned, ireturned, flag))return 0;
		end_of_varible= osym_tab.tellp();//�����Ļ�Ҳ���ڻس�ǰ��λ�ý�����˼�����λ���Ƿ�  ��������ʼ��ȡlookup  Ӱ��,����������һ����¼���������Ƿ�Ӱ�����һ����¼��lookup��ȡ����ʧ������Ϣ��
		insert_func_tab(func_name, return_type, entry_address,formal_para_num,entry_of_varible,space_used,end_of_varible);
		if (keyword_temp == "main") {
			assembler.width(5);
			assembler << linelabel;
			cout.width(5);
			cout << linelabel++;
			cout <<  "HALT" << endl;
			assembler <<"HALT" << endl;
		}
		while (valueofsymbol == 65 || valueofsymbol == 83 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)
		{
			keyword_temp = keyword;
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 49)
			{
				cout << "Error24: ȱ�ٺ�����";
				return 0;
			}
			insert_func(keyword_temp, keyword);//���뵽���ű�
			return_type = keyword_temp;
			func_name = keyword;
			entry_of_varible = osym_tab.tellp();
			fly_lookup = entry_of_varible;
			entry_address = linelabel;
			keyword_temp = keyword;
			name_count.zerosetting();
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 40)//(
			{
				cout << "Error25: ����������ȱ�١�����";
				return 0;
			}
			getsym(ch, valueofsymbol);
			formal_para_num = 0;
			if (!O(ch, valueofsymbol, returned, ireturned, flag,formal_para_num))return 0;
			if (valueofsymbol != 41)//)
			{
				cout << "Error26: ����������ȱ�١�����" << endl;
				return 0;
			}
			getsym(ch, valueofsymbol);
			if (!P(ch, valueofsymbol, returned, ireturned, flag))return 0;
			end_of_varible = osym_tab.tellp();//�����Ļ�Ҳ���ڻس�ǰ��λ�ý�����˼�����λ���Ƿ�  ��������ʼ��ȡlookup  Ӱ��,����������һ����¼���������Ƿ�Ӱ�����һ����¼��lookup��ȡ����ʧ������Ϣ��
			insert_func_tab(func_name, return_type, entry_address, formal_para_num, entry_of_varible, space_used,end_of_varible);
			if (keyword_temp == "main") {
				assembler.width(5);
				assembler << linelabel;
				cout.width(5);
				cout << linelabel++;
				cout << "HALT" << endl;
				assembler << "HALT" << endl;
			}
		}
		//#����//////////////////////////////////////////////////////////////////////////////////////////
		return 1;
	}
	else
	{
		cout << "Error4: ����ȱ�ٷ���ֵ���͡���int char float double long short signed";
		return 0;
	}
}
string NEWT(void)
{
	static const string tname = "t";
	string temporary_var;
	char b[11];
	int2str(name_count++,b, sizeof(b));
	temporary_var = tname + b;
	return temporary_var;
}
string getlabel()
{
	static cCounter label_count(1);
	const string lname = "label";
	string label;
	char b[11];
	int2str(label_count++, b, sizeof(b));
	label = lname + b ;
	return label;
}
string _add(string y,string z,string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler << "+ " + y + " " + z + " " + t  <<endl;;
	return "+ " + y + " " + z + " " + t  ;
}
string _relop(string y, string z, string t,string relop)//��ϵ�����ֻ�ܱ�ID��ID����
{
	assembler.width(5);
	assembler << linelabel;
	assembler <<  relop + " " + y + " " + z + " " + t  <<endl;;
	cout.width(5) ; cout << linelabel++;
	return  relop + " " + y + " " + z+" " + t  ;
}
string _and(string y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler << "�� " + y + " " + z + " " + t  <<endl;;
	return "�� " + y + " " + z + " " + t  ;
}
string _or(string y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler<<"�� " + y + " " + z + " " + t  <<endl;;
	return "�� " + y + " " + z + " " + t  ;
}
string _equal(string y, string z)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler<< "= " + z + " " + y  <<endl;;
	return "= " + z + " " + y  ;
}
string _equal(string y, int z)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(z, b, sizeof(b));
	string bb = b;
	assembler<< "= " + bb + " " + y  <<endl;;
	return "= " + bb+ " " + y  ;
}
string _mult(string y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler << "* " + y + " " + z + " " + t  <<endl;
	return "* " + y + " " + z + " " + t  ;
}
string _call(string func, string parameter)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5); 
	cout << linelabel++;
	assembler << "CALL " + func + " " + parameter<< endl;
	return "CALL " + func + " " +parameter;
}
string _par(string parameter)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5);
	cout << linelabel++;
	assembler << "PAR " + parameter << endl;
	return "PAR " + parameter;
}
string _accept(string varible)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5);
	cout << linelabel++;
	assembler << "ACC " + varible << endl;
	return "ACC " +varible;
}
string _return(string return_v)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5);
	cout << linelabel++;
	assembler << "RETURN " + return_v << endl;
	return "RETURN " + return_v;
}
string _mult(int y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(y, b, sizeof(b));
	string bb = b;
	assembler<< "* " + bb + " " + z + " " + t  <<endl;;
	return "* " +bb + " " + z + " " + t  ;
}
string _mult(string y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(z, b, sizeof(b));
	string bb = b;
	assembler << "* " + y + " " + bb + " " + t  <<endl;;;
	return "* " + y + " " + bb + " " + t  ;
}
string _mult(int y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	char c[11];
	int2str(y, c, sizeof(c));
	int2str(z, b, sizeof(b));
	string cc = c;
	string bb = b;
	assembler<<"* " + cc + " " + bb + " " + t  <<endl;;
	return "* " + cc + " " + bb + " " + t  ;
}
string _genJZ(string E_place, string label)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5); cout << linelabel++;
	assembler<< "JZ " + E_place + " " + label  <<endl;;
	return "JZ " + E_place + " " + label  ;
}
string _genJP(string label)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5); cout << linelabel++;
	assembler<< "JP " + label  <<endl;;
	return "JP " + label  ;
}
string _print(string y)//ERROR �����д� print ���±��
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5); cout << linelabel++;
	assembler << "PRINT " + y << endl;
	return "PRINT " + y;
}
string _WgenJP(string label, int retention_linelabel)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5); cout << linelabel++;
	assembler << "JP " <<retention_linelabel << endl;;
	return "JP " + label;
}
string _sub(string y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler << "- " + y + " " + z + " " + t  <<endl;;
	return "- " + y + " " + z + " " + t  ;
}
string _divided(string y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler << "/ " + y + " " + z + " " + t  <<endl;;
	return "/ " + y + " " + z + " " + t  ;
}
string _divided(int y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(y, b, sizeof(b));
	string bb = b;
	assembler << "/ " + bb + " " + z + " " + t  <<endl;;
	return "/ " + bb + " " + z + " " + t  ;
}
string _divided(string y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(z, b, sizeof(b));
	string bb = b;
	assembler<< "/ " + y + " " + bb + " " + t  <<endl;;
	return "/ " + y + " " + bb + " " + t  ;
}
string _divided(int y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	char c[11];
	int2str(y, c, sizeof(c));
	int2str(z, b, sizeof(b));
	string cc = c;
	string bb = b;
	assembler<< "/ " + cc + " " + bb + " " + t  <<endl;;
	return "/ " + cc + " " + bb + " " + t  ;
}
string _add(int y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(y, b, sizeof(b));
	string bb = b;
	assembler<< "+ " + bb + " " + z + " " + t  <<endl;;
	return "+ " + bb + " " + z + " " + t  ;
}
string _add(string y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(z, b, sizeof(b));
	string bb = b;
	assembler<< "+ " + y + " " + bb + " " + t  <<endl;;
	return "+ " + y + " " + bb + " " + t  ;
}
string _add(int y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	char c[11];
	int2str(y, c, sizeof(c));
	int2str(z, b, sizeof(b));
	string cc = c;
	string bb = b;
	assembler<< "+ " + cc + " " + bb + " " + t  <<endl;;
	return "+ " + cc + " " + bb + " " + t  ;
}
string _sub(int y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(y, b, sizeof(b));
	string bb = b;
	assembler << "- " + bb + " " + z + " " + t  <<endl;;
	return "- " + bb + " " + z + " " + t  ;
}
string _sub(string y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(z, b, sizeof(b));
	string bb = b;
	assembler<< "- " + y + " " + bb + " " + t  <<endl;;
	return "- " + y + " " + bb + " " + t  ;
}
string _sub(int y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	char c[11];
	int2str(y, c, sizeof(c));
	int2str(z, b, sizeof(b));
	string cc = c;
	string bb = b;
	assembler << "- " + cc + " " + bb + " " + t  <<endl;;
	return "- " + cc + " " + bb + " " + t  ;
}
int lookup(string id)//wait
{
	string str1;
	string str2;
	int str3;
	sym_tab.open("symbol_table.txt");
	sym_tab.seekg(fly_lookup, ios::beg);
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
	cout << "Error_x1: ��������������ڱ�������δ�����ʹ��";
	sym_tab.close();
	return -1;//׼�����������ף�������һ����==-1
}
int insert_func_tab(string func_name,string return_type,int entry_address,int num_formal_para,int entry_address_of_varible,int lens_of_varible,int end_of_varible)
//���������ơ�����ֵ���͡����������ڵ�ַ���βθ������βμ��ֲ������ڱ�����Ŀ�ʼλ�á��������ĳ���,������Ľ���λ�����뺯�����ű��С�
{//���������ڵ�ַ���кţ��βθ������β������ı�Ŵ�һ���βμ��ֲ������ڱ�����Ŀ�ʼλ����tellp�ڸ���һ���س��ľ��뷵�صģ��������ĳ�Ϊ�βμ��ֲ������ĸ���,the last one is������Ľ���λ��
	func_tab << func_name << " " << return_type << " " << entry_address << " " << num_formal_para << " " << entry_address_of_varible << " " << lens_of_varible <<" "<<end_of_varible<< endl;
	return 1;
}
class function_table_info
{//���������ơ�����ֵ���͡����������ڵ�ַ���βθ������βμ��ֲ������ڱ�����Ŀ�ʼλ�á��������ĳ���,������Ľ���λ�����뺯�����ű��С�
public:
	string func_name;
	string return_type;
	int entry_address;
	int num_formal_para;
	int entry_address_of_varible;
	int lens_of_varible;
	int end_of_varible;
};
function_table_info lookup_func_tab(string func_name)
{
	function_table_info func_info;
	fstream func_tabl("�������ű�.txt");
	while (!func_tabl.eof())
	{
		func_tabl >> func_info.func_name;
		func_tabl >> func_info.return_type;
		func_tabl >> func_info.entry_address;
		func_tabl >> func_info.num_formal_para;
		func_tabl >> func_info.entry_address_of_varible;
		func_tabl >> func_info.lens_of_varible;
		func_tabl >> func_info.end_of_varible;
		if (func_info.func_name == func_name)
		{
			func_tabl.close();
			return func_info;
		}
	}
	func_tabl.close();
}
int insert(string type, string var_name)//wait
{
	osym_tab.seekp(0, ios::end);
	osym_tab << endl;//�ļ��������ӣ�ѡ��space_used ���ȫ�ֱ�����ÿ��insertʱ��1������
	osym_tab << type << " " << var_name << " " << space_used;
	osym_tab.flush();//�������������⣬��ȷ��flush����ʹ�ú��Ч��������������˵������ˢ�»�������������У����Թرպ��ٴ�һ���ļ�
	space_used += 1;
	return 1;
}
int insert_func(string type, string func_name)
{
	osym_tab.seekp(0, ios::end);
	osym_tab << endl;//�ļ��������ӣ�ѡ��space_used ���ȫ�ֱ�����ÿ��insertʱ��1������
	osym_tab << type << " " << func_name << " " << -2;//-1Ϊ��������־
	osym_tab.flush();//�������������⣬��ȷ��flush����ʹ�ú��Ч��������������˵������ˢ�»�������������У����Թرպ��ٴ�һ���ļ�
	space_used =0;
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
	oint_tab<< integer <<" "<< iaddress;
	oint_tab.flush();
	iaddress += 2;
	iint_tab.close();
	return iaddress-2;
}//////////////////////////////////////////����ķ�����ڵ�ַ��ָiaddress�𣬻����ڱ��еĶ�λ�����Ժ��ҵ����
void closethefile()
{
	system("pause");
	oint_tab.close();
	osym_tab.close();
	assembler.close();	
	func_tab.close();//�����ļ�
	ofstream toclearthefile("symbol_table.txt");//����ļ�
	toclearthefile.close();
	toclearthefile.open("����������.txt");
	toclearthefile.close();
	toclearthefile.open("�������ű�.txt");
	toclearthefile.close();
	keywordfile.close();
	sourcefile.close();
}
class PS
{
public:
	int run;
	int finish;
	int error;
};
void Line2op(char line[], int linesize, int &pc, string &ir_op, string &ir_op1,string &ir_op2, string &ir_op3)
{
	char t[20];
	ofstream otemp("temp.txt");
	otemp << line;
	otemp.close();
	fstream temp("temp.txt");
	temp >> pc;
	if (temp.eof())
		ir_op = "";
	temp >> ir_op;
	if (temp.eof()) 
		ir_op1 = "";
	temp >> ir_op1;
	if (temp.eof()) 
		ir_op2 = "";
	temp >> ir_op2;	
	if (temp.eof()) //���temp������ɾ����ǰ���µ��ַ��������ؿ��ַ�
		ir_op3 = "";
	temp >> ir_op3;

	temp.close();
}
int compiler()
{
	string str;
#ifdef VER5
	cout << "this is version5(���������úͽ��ʹ����ܵı�����)";
#endif
	string returned;
	int ireturned = 0, flag = 0;
	space_used = 0;///////////////////////////////////////////////////veryimportant//////////////////anothor thing is ififififififiififf you have forget to write the if statement ifififiifififififi
	iaddress = 0;
	osym_tab.open("symbol_table.txt");//'///////waitting
	oint_tab.open("����������.txt");
	func_tab.open("�������ű�.txt");
	/////////////////////////////////////////////////////////
	keywordfile.open("keyword.txt");
	cout << endl;
	printf("�����ļ���");//����keyword�ļ�����Ŀ����ԣ�������С�ĸ��Ĺ�����֪����û�ж���Ŀո� tab �س�����
	cout << endl;
	cin >> fname;
	cout << endl;
	sourcefile.open(fname);
	cout << endl;
	cout.setf(std::ios::left);
	if (!assembler) {
		cout << "�򲻿��м��������ļ�";
		return 0;
	}
	assembler.setf(std::ios::left);
	if (sourcefile)
	{
		char ch;
		sourcefile.get(ch);
		int valueofsymbol;
		if (!X(ch, valueofsymbol, returned, ireturned, flag))
		{
			int x;
			cin >> x;//for testing
			closethefile();
			return 0;
		}
		if (valueofsymbol != 35) { cout << "Error17: ȱ���ļ���������#"; int x; cin >> x;  closethefile(); return 0; }
		getsym(ch, valueofsymbol);//�ĵ��Ѿ�������#���ź�������������κη��ţ���᷵��-128�����򣬷����Ǹ�����
		if (valueofsymbol != -128) { cout << "Error18: #���ź���δ��������"; int x; cin >> x; closethefile(); return 0; }
		//cout << linelabel << "   HALT" << endl;
		//assembler << linelabel << "   HALT" << endl;
		cout << "succeed in compiling,now come into the phase of interpretation"; //relese�汾�Ƿ񲻻���ͣ��
		//int x;
		cout << endl << endl << endl;
		//cin >> x;//for testing
		oint_tab.close();
		osym_tab.close();
		assembler.close();
		func_tab.close();
		return 1;
	}
	else
	{
		cout << "Error: Can't open the file" << endl;
		closethefile();
		return 0;
	}
}
int fly_lookup_end=0;
int lookupi(string id)                                              //wait
{
	string str1;
	string str2;
	int str3;
	sym_tab.open("symbol_table.txt");
	sym_tab.seekg(fly_lookup, ios::beg);
	while (fly_lookup_end > sym_tab.tellg())/////////////ע���˿��ܳ���û�в���
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
	cout << "Error_x1: ��������������ڱ�������δ�����ʹ��";
	sym_tab.close();
	return -1;                                                              //׼�����������ף�������һ����==-1
}
void op2reg(string ir_op1, int &ax)
{
	int bx;
	if (ir_op1[0] < 60)//������
	{
		int b = atoi(ir_op1.c_str());
		ax = b;
	}
	else {
		bx = lookupi(ir_op1);
		ax = stack[sp+3+bx];
	}
}
int interpretation()
{
	function_table_info func_info;
	PS ps;//ps: (run, finish, error); {����״̬��}
	fstream code("�����assembler.txt");     //ir: order; {ָ��Ĵ���}
	string ir_op, ir_op1, ir_op2, ir_op3;//ָ�������,������1��x����������2��y��
	int ax, bx, cx, dx;
	int pc;// : integer; {ָ�������}
	int t; //integer; {ջ���Ĵ���}//ʵ����������ռջ�ռ�Ϊ0-11
	sp=0;// : integer; {����ַ�Ĵ���}
		  //display: ARRAY[0..7] OF integer; {��ַ�Ĵ�����}
		  //code: ARRAY[0..codesize] OF order; {������}
	int top; //���¼��ָ��
	char line[50];
	int TimesOfTransferPar = 1;
	string func_stack[100];
	func_info = lookup_func_tab("main");
	func_stack[0] = "main";
	int func_stacki = 1;
	//���������ơ�����ֵ���͡����������ڵ�ַ���βθ������βμ��ֲ������ڱ�����Ŀ�ʼλ�á��������ĳ���,������Ľ���λ�����뺯�����ű��С�
	//���������ڵ�ַ���кţ��βθ������β������ı�Ŵ�һ���βμ��ֲ������ڱ�����Ŀ�ʼλ����tellp�ڸ���һ���س��ľ��뷵�صģ��������ĳ�Ϊ�βμ��ֲ������ĸ���,the last one is������Ľ���λ��
	//���������ơ�����ֵ���͡����������ڵ�ַ���βθ������βμ��ֲ������ڱ�����Ŀ�ʼλ�á��������ĳ���,������Ľ���λ�����뺯�����ű��С�
	{	
		fstream temp("�����assembler.txt");
		int b = func_info.entry_address;
		char a[50];
		ax = -1;
		while (ax != b - 1)
		{
			temp.getline(a, 50);
			Line2op(a, 50, ax, ir_op, ir_op1, ir_op2, ir_op3);
		}
		code.seekg(temp.tellg(), ios::beg);
		temp.close();
	}
	top = func_info.lens_of_varible+3-1;
	fly_lookup = func_info.entry_address_of_varible;
	fly_lookup_end = func_info.end_of_varible;
	cout << "����������-------------------------------------------------------" << endl;
	while (!code.eof())//���order�Ѷ������������޷��ƶ��ļ�ָ�룬ע��
	{
		code.getline(line, 50);
		Line2op(line, 50, pc, ir_op, ir_op1, ir_op2, ir_op3);
		if (ir_op == "=")
		{
			op2reg(ir_op1, ax);
			cx=lookupi(ir_op2);//�ڶ���������ֻ�����Ǳ���
			stack[sp+3+cx] = ax;
		}
		else if (ir_op == "��")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax&&cx);
		}
		else if (ir_op == "��")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax||cx);
		}
		else if (ir_op == ">=")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax >=cx);
		}
		else if (ir_op == "<=")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax <= cx);
		}
		else if (ir_op == ">")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax > cx);
		}
		else if (ir_op == "<")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax < cx);
		}
		else if (ir_op == ":")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax != cx);
		}
		else if (ir_op == ":=")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax == cx);
		}
		else if (ir_op == "+")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax + cx);
		}
		else if (ir_op == "*")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax * cx);
		}
		else if (ir_op == "-")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax - cx);
		}
		else if (ir_op == "/")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			if (cx == 0) { cout << "ErrorY2:����0����"<<endl; return 0; }
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax / cx);
		}
		else if (ir_op == "JP")
		{
			fstream temp("�����assembler.txt");
			int b = atoi(ir_op1.c_str());
			char a[50];
			ax = -1;
			while (ax != b - 1)
			{
				temp.getline(a, 50);
				Line2op(a, 50, ax, ir_op, ir_op1, ir_op2, ir_op3);
			}
			code.seekg(temp.tellg(), ios::beg);
			temp.close();
		}
		else if (ir_op == "JZ")
		{
			op2reg(ir_op1, ax);
			if (ax != 0)continue;
			fstream temp("�����assembler.txt");
			int b = atoi(ir_op2.c_str());
			char a[50];
			ax = -1;
			while (ax != b - 1)
			{
				temp.getline(a, 50);
				Line2op(a, 50, ax, ir_op, ir_op1, ir_op2, ir_op3);
			}
			code.seekg(temp.tellg(), ios::beg);
			temp.close();
		}
		else if (ir_op == "RETURN")
		{
			op2reg(ir_op1, dx);//������ֵ����dx�Ĵ����У�������ܣ���һ�����ڷ��ص�ַ�����ʶ������ܵ�����������Ӱ�졣
			bx = stack[sp + 1];
			top = sp - 1;
			sp = stack[sp];
			{//ת�����ص�ַ
				fstream temp("�����assembler.txt");
				char a[50];
				ax = -1;
				while (ax != bx - 1)
				{
					temp.getline(a, 50);
					Line2op(a, 50, ax, ir_op, ir_op1, ir_op2, ir_op3);
				}
				code.seekg(temp.tellg(), ios::beg);
				temp.close();
			}
			func_stacki--;
			func_info=lookup_func_tab(func_stack[func_stacki - 1]);
			fly_lookup = func_info.entry_address_of_varible;
			fly_lookup_end = func_info.end_of_varible;//�ָ�lookup��Ĳ������䣡
		}
		else if (ir_op == "ACC")
		{
			bx=lookupi(ir_op1);
			stack[sp + 3 + bx] = dx;
		}
		else if (ir_op == "CALL")
		{
			func_info = lookup_func_tab(ir_op1);
			func_stack[func_stacki++] = ir_op1;///////////////////////ע��: ���ܳ������ir_op1����������func_stack������ַ�������Ҳ����ʧ
			fly_lookup = func_info.entry_address_of_varible;
			fly_lookup_end = func_info.end_of_varible;
			TimesOfTransferPar = 1;
			stack[top + 1] = sp;
			sp = top + 1;
			top = func_info.lens_of_varible + 3 - 1+sp;
			op2reg(ir_op2, ax);
			if (ax != func_info.num_formal_para)
			{
				cout << "ErrorY3: ��������뺯������ʱ�Ĳ���������һ��" << endl;
				cout << "-------------------------------------------------------------------" << endl;
				return 0;
			}
			stack[sp + 2] = ax;
			stack[sp + 1] = pc + 1;//���ص�ַ��call����һ�������кš�
			{//ת���ӳ������
				fstream temp("�����assembler.txt");
				int b = func_info.entry_address;
				char a[50];
				ax = -1;
				while (ax != b - 1)
				{
					temp.getline(a, 50);
					Line2op(a, 50, ax, ir_op, ir_op1, ir_op2, ir_op3);
				}
				code.seekg(temp.tellg(), ios::beg);
				temp.close();
			}
		}
		else if (ir_op == "PAR")
		{
			op2reg(ir_op1, ax);
			stack[top + 4 + TimesOfTransferPar++ - 1] = ax;
		}
		else if (ir_op == "PRINT")
		{
			op2reg(ir_op1, ax);
			cout << ax << endl;
		}
		else if (ir_op == "HALT")
		{
			cout << "-------------------------------------------------------------------" << endl;
			return 1;
		}
		else
		{
			cout << "ErrorY1: ָ���޷�ʶ��" << endl;
		}
	}
}
int main()//if ��仹û��ʵ��
{
	if (!compiler())
	{
		system("pause");
		closethefile();
		return 0;
	}
	//����ģ��
	if (!interpretation())
	{
		system("pause");
		closethefile();
		return 0;
	}
	//����ģ��


	system("pause");
	ofstream toclearthefile("symbol_table.txt");//����ļ�
	toclearthefile.close();
	toclearthefile.open("����������.txt");
	toclearthefile.close();
	toclearthefile.open("�������ű�.txt");
	toclearthefile.close();
	keywordfile.close();
	sourcefile.close();
	return 1;
}