#include <iostream>
#include<fstream>
#include <iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<sstream>
#include<ctype.h>
#include<bits/stdc++.h>
#include<malloc.h>
using namespace std;
double tmp;
string double_to_string(double n)//double��תstring��
{
ostringstream stream;
stream<<n; //nΪint����
return stream.str();
}
double string_double(string str)//stringתdouble
{
    double num;
    stringstream sin(str);
    sin>>num;
    return num;
}
bool isnumber(string str)//�ж��Ƿ�����
{
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
		��ʱ���յ���.f�ⲿ�֣�������������������false
          */
		return false;
	}
	return true;
}
typedef struct node//��Ԫʽ
{
    string num;//���������
    string symbol;//����
    struct node *left,*right;//ָ�����ڵ�
    struct same *adj;//ָ��ȼ۱�����ָ��
}node;
node four[1000];
typedef struct same//����
{
    string one;//��һ������
    struct same *next;//ָ����һ���ȼ۱���
}same;
int num=0,locate=-1;
int main()
{   int sum=1;
    string str;//��Ԫʽ��Ԫ��
    string ele[4];
    char buffer[256];
    int i=0;
    ifstream outFile;
    outFile.open("C:\\Users\\Lenovo\\Desktop\\text1.txt",ios::in);
    cout<<"inFile.txt"<<"--- all file is as follows:---"<<endl;
    while(!outFile.eof())
    {   outFile.getline(buffer,256,'\n');
        for( i=0;buffer[i-1]!=')';i++)
        {
            if(buffer[i]=='(')
               continue;
            else
                if(buffer[i]!=','&&buffer[i]!=')')
                str+=buffer[i];
                else
                    {
                    ele[num++]=str;
                    str="";
                    }
        }
        num=0;
        same *p;
        node *q;
        if(ele[0]=="="&&isnumber(ele[1])!=true)//�򵥸�ֵ���
        {   cout<<"����Ǹ���ֵ���"<<endl;
            int flag=0;
            for( i=sum-1;i>=0;i--)
            {
               same *tmp;
               same *tmp1;
            if(ele[1]==four[i].num)//����֪��ע�ڵ�ȼ�,����ȼ۽ڵ㣨δ����λ�ã�
            {   locate=i;
                flag=1;
                same *p=new same;
                p->one=ele[3];
                p->next=NULL;
                if(four[i].adj==NULL)
                four[i].adj=p;
                else
                {   same *tmp=four[i].adj;
                    while(tmp->next!=NULL)
                    {
                        tmp=tmp->next;
                    }
                    tmp->next=p;
                }
                if(*((four[i].num).begin())=='t')
                {
                    string tmp1=four[i].num;
                    four[i].num=p->one;
                    p->one=tmp1;
                }
                break;
            }
            else
            {
               for(tmp=four[i].adj;tmp!=NULL;tmp=tmp->next)//�ڸýڵ�ĵȼ۽ڵ������
                  {
                      if(tmp->one==ele[3])
                      {
                          break;
                          locate=i;
                          flag=1;
                      }
                      if(tmp->one==ele[1])
                        flag=1;
                      if(tmp->next==NULL)
                        tmp1=tmp;

                  }
            if(flag==1)
            {   locate=i;
                cout<<"�򵥵ȼ����"<<endl;
                same *p=new same;
                p->one=ele[3];
                p->next=NULL;
                tmp1->next=p;
                if(*((four[i].num).begin())=='t')
                {
                    string tmp1=four[i].num;
                    four[i].num=p->one;
                    p->one=tmp1;

                }
                break;
            }
            }
            }
            if(i==-1&&flag==0)//mû���ֵȼۣ���Ҫ����
            {   cout<<"û���ּ򵥵ȼ����"<<endl;
                same *p=new same;
                        p->one=ele[1];
                        p->next=NULL;
                        four[sum].num=ele[3];
                        four[sum].symbol="=";
                        four[sum].left=NULL;
                        four[sum].right=NULL;
                        four[sum].adj=p;
                        sum++;
                        locate=sum-1;

            }

        }
        //�������Ԫʽ�ǳ������㣬�����������ڵ�ȼۻ����½��ڵ�
        else if((isnumber(ele[1])==true&&isnumber(ele[2])==true)||(isnumber(ele[1])&&ele[0]=="="))
        {
           double  number;
            //���������
            if     (ele[0]=="+") number=string_double(ele[1])+string_double(ele[2]);
            else if(ele[0]=="-") number=string_double(ele[1])-string_double(ele[2]);
            else if(ele[0]=="*") number=string_double(ele[1])*string_double(ele[2]);
            else if(ele[0]=="/") number=string_double(ele[1])/string_double(ele[2]);
            else number=string_double(ele[1]);
            for( i=sum-1;i>=0;i--)
            { //cout<<double_to_string(number)<<"=?="<<four[i].num<<endl;
                int flag=0;
                same *tmp1,*tmp;
                if(double_to_string(number)==four[i].num)//����֪�ڵ�ȼ�,����ȼ۽ڵ㣨δ����λ�ã�
                    flag=1;
                    for(tmp=four[i].adj;tmp!=NULL;tmp=tmp->next)//�ڸýڵ�ĵȼ۽ڵ������
                      {    //cout<<tmp->one<<" ?="<<ele[1]<<endl;
                          if(tmp->one==double_to_string(number))
                          {
                            locate=i;
                            flag=1;
                          }
                          if(tmp->next==NULL)
                            tmp1=tmp;

                      }
                    if(flag==1)
                    {
                        cout<<"�ҵ��ȼ۱��ʽ"<<endl;
                        same *p=new same;
                        p->one=ele[3];
                        p->next=NULL;
                        tmp1->next=p;
                        if(*((four[i].num).begin())=='t')
                        {
                            string tmp1=four[i].num;
                            four[i].num=p->one;
                            p->one=tmp1;

                        }
                        break;
                    }
            }
            if(i==-1)//û���ֵĵȼ۽ڵ�ͽ����½ڵ㲢��������������
            {           cout<<"�ǳ�����û�еȼ۵�,�½�"<<endl;
                        same *p=new same;
                        p->one=ele[3];
                        p->next=NULL;
                        four[sum].num=double_to_string(number);
                        //cout<<"sumber="<<sum<<"   "<<double_to_string(number)<<endl;
                        four[sum].symbol="=";
                        four[sum].left=NULL;
                        four[sum].right=NULL;
                        four[sum].adj=p;
                        sum++;
                        locate=sum-1;
            }
        }
        //���ǳ��������ǵȼۻ򴴽��ڵ㲢�������ҽڵ㣨��������Ӧ�õ��Ƿ�ȼ���ĳ����/�������������ظ�����
        else
        {
            same *tmp,*tmp1;
            int zuo=-1,you=-1, flag=0,flag1=0;
            for(i=sum-1;i>=0&&flag!=2;i--)
            {   zuo=-1;you=-1; flag=0;
                if(ele[0]==four[i].symbol)//������num�ڵ�͵ȼ۽ڵ�
                    flag=1;
                if((four[i].left)!=NULL&&ele[1]==(four[i].left)->num)
                    zuo=i;
                if((four[i].left)!=NULL&&ele[2]==(four[i].right)->num)
                    you=i;
                if(zuo==-1&&(four[i].left)!=NULL)
                    for(tmp=(four[i].left)->adj;tmp!=NULL;tmp=tmp->next)//�ڸýڵ���ڵ�ĵȼ۽ڵ������
                    {
                     if(tmp->one==ele[1])
                        zuo=i;
                    }
                if(you==-1&&(four[i].right)!=NULL)
                    for(tmp1=(four[i].right)->adj;tmp1!=NULL;tmp1=tmp1->next)//�ڸýڵ��ҽڵ�ĵȼ۽ڵ������
                      {
                        if(tmp1->one==ele[2])
                        you=i;
                      }
                flag=flag;
                if(zuo!=-1&&you!=-1&&flag==1)//�ȼ���Ҫ����
                {
                    same *p=new same,*tmp;
                    p->one=ele[3];
                   // cout<<"ele[3]=="<<ele[3]<<endl;
                    p->next=NULL;
                    if (four[zuo].adj==NULL)
                    {
                        four[zuo].adj=p;
                        flag=2;
                    }
                    else
                    for(tmp=four[zuo].adj;tmp!=NULL;tmp=tmp->next)
                    {
                       if (tmp->next==NULL)
                        {
                            tmp->next=p;
                            flag=2;
                            break;
                        }
                    }
                }
            }
            if(!(zuo!=-1&&you!=-1&&flag==2))//û�ҵ��ȼ۽ڵ��������Ӧ�õ�ֱ��жϣ��ж������Ǳ�����������
            {
            for(i=sum-1;i>=0&&(zuo==-1||you==-1);i--)
            {
                if(four[i].num==ele[1])
                    zuo=i;
                if(four[i].num==ele[2])
                    you=i;
                for(tmp=four[i].adj;tmp!=NULL;tmp=tmp->next)//�ڸýڵ�ĵȼ۽ڵ������
                      {
                          if(tmp->one==ele[1])
                          {
                               zuo=i;
                          }
                          if(tmp->one==ele[2])
                          {
                               you=i;
                          }
                      }
            }
           double number;
                if(zuo==-1)
                {   cout<<"�½���ڵ�"<<endl;
                    four[sum].num=ele[1];
                    four[sum].left=NULL;
                    four[sum].right=NULL;
                    four[sum].adj=NULL;
                    zuo=sum; //����λ����Ϣ
                    sum++;

                }
                else zuo=zuo;
                if(you==-1)
                {      cout<<"�½��ҽڵ�"<<endl;
                    four[sum].num=ele[2];
                    four[sum].left=NULL;
                    four[sum].right=NULL;
                    four[sum].adj=NULL;
                    you=sum; //����λ����Ϣ
                    sum++;
                }
                four[sum].num=ele[3];
                four[sum].symbol=ele[0];
                four[sum].left=&four[zuo];
                four[sum].right=&four[you];
                four[sum].adj=NULL;
                sum++;
                locate=sum-1;
             }
        }
        if(*ele[3].begin()!='t')
        for(i=sum-1;i>=0;i--)//�ض�������
        {   same *tmp;

            if(i==locate)
                continue;
            if(four[i].num==ele[3])
            {
                    four[i].num="t";
                    break;
            }
            for(tmp=four[i].adj;tmp!=NULL;tmp=tmp->next)
                  {
                      if(tmp->one==ele[3])
                      {
                            tmp->one="t";
                            break;
                      }
                  }

        }
    }
        same *tmp1;
    for(i=1;i<sum;i++)//�ڵ���Ϣ���
        {

              cout<<"node "<<i<<" is  "<<" �ڵ�ı���Ϊ"<<four[i].num<<" ";
            cout<<"�ȼ۱�����"<<"  ";
            for(tmp1=four[i].adj;tmp1!=NULL;tmp1=tmp1->next)
                  {
                      cout<<tmp1->one<<" ";

                  }
                  cout<<endl;


        }
    cout<<"������ԪʽΪ"<<endl;
for(i=1;i<sum;i++)
{
    if(four[i].left==NULL&&four[i].right==NULL)
    {  same *tmp1;
        for(tmp1=four[i].adj;tmp1!=NULL;tmp1=tmp1->next)
                  {  if(*((tmp1->one).begin())!='t')
                     cout<<"="<<tmp1->one<<" _ "<<four[i].num<<endl;
                  }
    }
    else
    {  same *tmp1;
       //if(*((four[i].num).begin())!='t')
        if(four[i].left->num=="t")
        {
            for(tmp1=four[i].left->adj;tmp1!=NULL;tmp1=tmp1->next)
                if(tmp1->one!="t")
                  {
                    four[i].left->num=tmp1->one;
                    break;
                  }
        }
         if(four[i].right->num=="t")
        {
            for(tmp1=four[i].right->adj;tmp1!=NULL;tmp1=tmp1->next)
                    if(tmp1->one!="t")
                  {
                    four[i].right->num=tmp1->one;
                    break;
                  }
        }
         if(four[i].num=="t")
        {
                    for(tmp1=four[i].adj;tmp1!=NULL;tmp1=tmp1->next)
                         if(tmp1->one!="t")
                      {
                        four[i].num=tmp1->one;
                        break;
                      }
        }
        cout<<four[i].symbol<<" "<<(four[i].left)->num<<" "<<(four[i].right)->num<<" "<<four[i].num<<endl;
       for(tmp1=four[i].adj;tmp1!=NULL;tmp1=tmp1->next)
                  { if(*((tmp1->one).begin())!='t')
                     cout<<"= "<<four[i].num<<" _ "<<tmp1->one<<endl;
                  }
    }
}
    return 0;
}
