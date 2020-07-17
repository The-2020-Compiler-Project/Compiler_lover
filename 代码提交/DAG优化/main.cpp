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
string double_to_string(double n)//double型转string型
{
ostringstream stream;
stream<<n; //n为int类型
return stream.str();
}
double string_double(string str)//string转double
{
    double num;
    stringstream sin(str);
    sin>>num;
    return num;
}
bool isnumber(string str)//判断是否是数
{
	stringstream sin(str);
	char c;
	if(!(sin >> tmp))
	{
		/*解释：
            sin>>t表示把sin转换成double的变量（其实对于int和float型的都会接收），
			如果转换成功，则值为非0，如果转换不成功就返回为0
        */
		return false;
	}
	if (sin >> c)
	{
		/*解释：
		此部分用于检测错误输入中，数字加字符串的输入形式（例如：34.f），在上面的的部分（sin>>t）
		已经接收并转换了输入的数字部分，在stringstream中相应也会把那一部分给清除，
		此时接收的是.f这部分，所以条件成立，返回false
          */
		return false;
	}
	return true;
}
typedef struct node//四元式
{
    string num;//常数或变量
    string symbol;//符号
    struct node *left,*right;//指向附属节点
    struct same *adj;//指向等价变量的指针
}node;
node four[1000];
typedef struct same//变量
{
    string one;//第一个变量
    struct same *next;//指向下一个等价变量
}same;
int num=0,locate=-1;
int main()
{   int sum=1;
    string str;//四元式的元素
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
        if(ele[0]=="="&&isnumber(ele[1])!=true)//简单赋值语句
        {   cout<<"这就是个赋值语句"<<endl;
            int flag=0;
            for( i=sum-1;i>=0;i--)
            {
               same *tmp;
               same *tmp1;
            if(ele[1]==four[i].num)//与已知标注节点等价,加入等价节点（未交换位置）
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
               for(tmp=four[i].adj;tmp!=NULL;tmp=tmp->next)//在该节点的等价节点继续找
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
                cout<<"简单等价语句"<<endl;
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
            if(i==-1&&flag==0)//m没发现等价，则要创建
            {   cout<<"没发现简单等价语句"<<endl;
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
        //如果该四元式是常数运算，则考虑与其他节点等价或则新建节点
        else if((isnumber(ele[1])==true&&isnumber(ele[2])==true)||(isnumber(ele[1])&&ele[0]=="="))
        {
           double  number;
            //计算该项结果
            if     (ele[0]=="+") number=string_double(ele[1])+string_double(ele[2]);
            else if(ele[0]=="-") number=string_double(ele[1])-string_double(ele[2]);
            else if(ele[0]=="*") number=string_double(ele[1])*string_double(ele[2]);
            else if(ele[0]=="/") number=string_double(ele[1])/string_double(ele[2]);
            else number=string_double(ele[1]);
            for( i=sum-1;i>=0;i--)
            { //cout<<double_to_string(number)<<"=?="<<four[i].num<<endl;
                int flag=0;
                same *tmp1,*tmp;
                if(double_to_string(number)==four[i].num)//与已知节点等价,加入等价节点（未交换位置）
                    flag=1;
                    for(tmp=four[i].adj;tmp!=NULL;tmp=tmp->next)//在该节点的等价节点继续找
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
                        cout<<"找到等价表达式"<<endl;
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
            if(i==-1)//没发现的等价节点就建立新节点并将变量关联起来
            {           cout<<"是常数但没有等价点,新建"<<endl;
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
        //不是常数，则考虑等价或创建节点并关联左右节点（考虑两个应用点是否等价于某变量/常数），或则重复定义
        else
        {
            same *tmp,*tmp1;
            int zuo=-1,you=-1, flag=0,flag1=0;
            for(i=sum-1;i>=0&&flag!=2;i--)
            {   zuo=-1;you=-1; flag=0;
                if(ele[0]==four[i].symbol)//访问主num节点和等价节点
                    flag=1;
                if((four[i].left)!=NULL&&ele[1]==(four[i].left)->num)
                    zuo=i;
                if((four[i].left)!=NULL&&ele[2]==(four[i].right)->num)
                    you=i;
                if(zuo==-1&&(four[i].left)!=NULL)
                    for(tmp=(four[i].left)->adj;tmp!=NULL;tmp=tmp->next)//在该节点左节点的等价节点继续找
                    {
                     if(tmp->one==ele[1])
                        zuo=i;
                    }
                if(you==-1&&(four[i].right)!=NULL)
                    for(tmp1=(four[i].right)->adj;tmp1!=NULL;tmp1=tmp1->next)//在该节点右节点的等价节点继续找
                      {
                        if(tmp1->one==ele[2])
                        you=i;
                      }
                flag=flag;
                if(zuo!=-1&&you!=-1&&flag==1)//等价则要关联
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
            if(!(zuo!=-1&&you!=-1&&flag==2))//没找到等价节点则对两个应用点分别判断（判断两点是变量还是数字
            {
            for(i=sum-1;i>=0&&(zuo==-1||you==-1);i--)
            {
                if(four[i].num==ele[1])
                    zuo=i;
                if(four[i].num==ele[2])
                    you=i;
                for(tmp=four[i].adj;tmp!=NULL;tmp=tmp->next)//在该节点的等价节点继续找
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
                {   cout<<"新建左节点"<<endl;
                    four[sum].num=ele[1];
                    four[sum].left=NULL;
                    four[sum].right=NULL;
                    four[sum].adj=NULL;
                    zuo=sum; //更新位置信息
                    sum++;

                }
                else zuo=zuo;
                if(you==-1)
                {      cout<<"新建右节点"<<endl;
                    four[sum].num=ele[2];
                    four[sum].left=NULL;
                    four[sum].right=NULL;
                    four[sum].adj=NULL;
                    you=sum; //更新位置信息
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
        for(i=sum-1;i>=0;i--)//重定义消除
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
    for(i=1;i<sum;i++)//节点信息输出
        {

              cout<<"node "<<i<<" is  "<<" 节点的变量为"<<four[i].num<<" ";
            cout<<"等价变量有"<<"  ";
            for(tmp1=four[i].adj;tmp1!=NULL;tmp1=tmp1->next)
                  {
                      cout<<tmp1->one<<" ";

                  }
                  cout<<endl;


        }
    cout<<"生成四元式为"<<endl;
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
