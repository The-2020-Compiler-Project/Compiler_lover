#include <iostream>
#include<vector>
#include<fstream>
#include <iostream>
#include<fstream>
#include<string>
#include<map>
#include<set>
#include<sstream>
#include<ctype.h>
#include<bits/stdc++.h>
using namespace std;
string word;//��¼��ǰtakeֵ
int pai[10][3],point=0;
vector<string>stk;//����ջ
int i=-1,t=0;//���μ�¼take�ķ��ʾͣ�ͳ����ʱ�������ɸ���
////string take[30]={"(","a3",")","||","a1"};
string take[30]={"(","(","(","(","1","+","a3","<","12",")",")","&&","a1",")","&&","(","!","(","b2",")",")",")"};
//string take[30]={"a1","&&","(","!","(","b2",")",")"};
int numk=-1;
template <typename Ts>
string data_to_string(Ts n)//������תstring��
{
ostringstream stream;
stream<<n;
return stream.str();
}
bool isnumber(string str)//�ж��Ƿ�����
{   char tmp;
	stringstream sin(str);
	char c;
	if(!(sin >> tmp))
	{
		/*���ͣ�
            sin>>t��ʾ��sinת����double�ı�������ʵ����int��float�͵Ķ�����գ���
			���ת���ɹ�����ֵΪ��0�����ת�����ɹ��ͷ���Ϊ0
        */
		return false;
	}
	if (sin >> c)
	{
		/*���ͣ�
		�˲������ڼ����������У����ּ��ַ�����������ʽ�����磺34.f����������ĵĲ��֣�sin>>t��
		�Ѿ����ղ�ת������������ֲ��֣���stringstream����ӦҲ�����һ���ָ������
		��ʱ���յ���.f�ⲿ�֣�����������boolboolZ��������false
          */
		return false;
	}
	return true;
}
void  docheck()
{   int j,i;
    memset(pai,-1,120);
    for( point=0,i=0;take[i]!="";i++)
{
    if(take[i]=="(")
    {
    pai[point][0]=i;
    point++;
    }
    else if (take[i]==")")
    {   j=point;
       while(pai[j-1][1]!=-1)
        {
            j--;
        }
        pai[j-1][1]=i;
    }
     else if (take[i]=="&&"||take[i]=="||"||take[i]=="!")
    { j=point;
        while(j!=0)
        {              if(pai[j-1][1]==-1)
                        pai[j-1][2]=1;
                        j--;
        }
    }
}
}
int check(int n)
{
    int i=0;
    while(pai[i][0]!=n)
        i++;
    return pai[i][2];
}
void Z();
void F();
void T();
void E();
//
void boolZ();
void boolF();
void boolT();
void boolE();
    void Z()
{
    word = take[i];
    E();
}
void E()
{
    T();
    while (word=="+"||word=="-")
    {
        int nu=i;
        i+=1;
        word=take[i];
        T();
        t=t+1;
        string a,b;
        a=stk.back();
        stk.pop_back();
        b=stk.back();
        stk.pop_back();
        string tt="";
        tt=data_to_string(t);
        string str1=take[nu]+" "+b+" "+a+" "+"t"+tt;
        cout<<str1<<endl;
        stk.push_back("t"+tt);
    }
}
void T()
{
    F();
    while(word=="*"||word=="/")
    {
        int nu=i;
        i+=1;
        word=take[i];
        F();
        t=t+1;
        string a,b;
        a=stk.back();
        stk.pop_back();
        b=stk.back();
        stk.pop_back();
        string tt="";
        tt=data_to_string(t);
        string str1=take[nu]+" "+b+" "+a+" "+"t"+tt;
        cout<<str1<<endl;
        stk.push_back("t"+tt);
    }
}
void  F()
{
    char tmp=*word.begin();
  if (isalnum(tmp))
    {
        stk.push_back(word);
        i+=1;
        word=take[i];
    }
    else
    {
        if (word == "(")
        {
        i+=1;
        word = take[i];
        E();
        if (word == ")")
        {  // cout<<"here?"<<endl;
            i+=1;
            word=take[i];
        }
        }
    }
}
void boolZ()
{
    i+=1;
    word = take[i];
    boolE();
}
void boolE()
{
    boolT();

    while (word=="||")
    {
        int nu=i;
        i+=1;
        word=take[i];
        boolT();
        t=t+1;
        string a,b;
        a=stk.back();
        stk.pop_back();
        b=stk.back();
        stk.pop_back();
        string tt="";
        tt=data_to_string(t);
        string str1=take[nu]+" "+b+" "+a+" "+"t"+tt;
        cout<<str1<<endl;
        stk.push_back("t"+tt);
    }
}
void boolT()
{
    boolF();
    // cout<<word<<endl;
    while(word=="&&")
    {
        int nu=i;
        i+=1;
        word=take[i];
        boolF();
        t=t+1;
        string a,b;
        a=stk.back();
        stk.pop_back();
        b=stk.back();
        stk.pop_back();
        string tt="";
        tt=data_to_string(t);
        string str1=take[nu]+" "+b+" "+a+" "+"t"+tt;
        cout<<str1<<endl;
        stk.push_back("t"+tt);
    }
}
void  boolF()
    {
        if(word=="!")
        {
            i++;
            word=take[i];
            boolE();
            string a=stk.back();
            stk.pop_back();
            t++;
            string str="= ! "+a+" t"+data_to_string(t);
            cout<<str<<endl;
            stk.push_back(" t"+data_to_string(t));
        }
        else if (word=="(")
            {    numk++;
                if(check(i)==1)
                {
                    i++;
                    word=take[i];
                    boolE();
                    i+=1;
                    word=take[i];
                }
                else
                {
                   // i++;
                    Z();
                    word=take[i];
                     if(take[i]=="=="||take[i]=="<="||take[i]==">="||take[i]=="<"||take[i]==">")
            {
                string tmp2=take[i];
            i++;

            Z();
      //      cout<<"i==="<<i<<" "<<word<<endl;
            string tmp1=stk.back();
            stk.pop_back();
            string tmp3=stk.back();
            stk.pop_back();
            while(i<=pai[numk][1])
                i++;
          //  cout<<"kuo"<<pai[numk][1]<<endl;
         // cout<<take[i]<<"fsfs"<<endl;
            t++;
            stk.push_back(" t"+data_to_string(t));
            cout<<tmp2<<" "<<tmp3<<" "<<tmp1<<" t"<<data_to_string(t)<<endl;
        //     cout<<"i==="<<i<<" "<<word<<endl;
            word=take[i];
            }

                }

            }

        else
            {   vector<string>::iterator iter=stk.end();
  //      cout<<"i0="<<i<<endl;
            Z();
            if(take[i]=="=="||take[i]=="<="||take[i]==">="||take[i]=="<"||take[i]==">")
            {
                string tmp2=take[i];
            i++;
            Z();
            string tmp1=stk.back();
            stk.pop_back();
            string tmp3=stk.back();
            stk.pop_back();
            t++;
            stk.push_back(" t"+data_to_string(t));
            cout<<tmp2<<" "<<tmp3<<" "<<tmp1<<" t"<<data_to_string(t)<<endl;
            //stk.push_back(word);
           // i++;
           // word=take[i];
            }
    }

    }
int main()
{   docheck();
    boolZ();
}
