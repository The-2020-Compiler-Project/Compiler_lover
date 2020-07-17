#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <array>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include<string>
#include<memory.h>
#include "lexical_analysis.h"
#include "Recording.h"
using namespace std;
/****
PT�ǽ��
KT�ǹؼ��ֱ�
IT�Ǳ�ʶ��
CT�ǳ�����****/
/****�ķ��࣬������ѭ������������ֵ�ȵ������жϺ���****/
//Token tokens[50] = { Token("CT",1),Token("PT",2),Token("CT",2),Token("PT",1),Token("IT",5) };
//if ��KT14��(��PT8,)��PT9,{��PT16,}��PT17
//(���ʽ) {���} else if {���} else {���}
/****
Token tokens[50] = { Token("IT",5),Token("PT",14),Token("CT",14),Token("PT",15) };
isidentifer�Ĳ��ԣ���ʶ��[13]
****/
Rstack record;
vector<Token>stk;
typedef struct four
{
    int  code;     //��һԪ��
    int  addr1; //�ڶ�Ԫ��
    int  addr2; //����Ԫ��
    int  result;//����Ԫ��
}four;
four fourr[300];
//memset(fourr,-1,1200);
int fournum=0;
class Grammar {
private:
    Token pretoken;
    bool isStatement1();                     //������䣨�ṹ��ר�ã����ܳ�ʼ����
    bool isInit1();                          //�ж����� a,b�����
public:
    Grammar() {
        pretoken = getNextToken();
    }
    int isIdentifer();                      //����1Ϊ��ͨ����������2Ϊ���飬����0Ϊ����
    bool isSubarrayvalue();                 //�ж����� 1,5,3,4,5
    bool isArrayvalue();                    //�ж����� {1,5,3,4,5}
    bool isStatement();                     //�������
    bool isInit();                          //�ж����� a,b=c,d={1,2,3,4}
    bool isInit2();                          //�ж����� a,b=c,d={1,2,3,4}
    bool isProgram();                       //c����
    bool isSubProgram();
    bool isFunction();                      //����
    bool isStruct();                        //�ṹ��
    bool isMain();                          //main
    bool isFunCall();                       //��������
    bool isParameter();                     //����
    bool isType();                          //���ͱ�ʶ��
    bool IsExpression();                    //�ж��������ʽ
    bool IsSubExpression();
    bool IsT();                             //�������ʽ������
    bool IsSubT();
    bool IsF();
    bool IsWLoopStatement();                 //Whileѭ�����
    bool IsFLoopStatement();                 //Forѭ������ж�
    bool IsReturnSatement();                //�������
    bool IsConditionalStatement();          //if�������
    bool IsAssignmentStatement();           //��ֵ���
    bool IsStatementBlock();                //����
    bool IsSingleStatement();               //������䣬��ѭ�������û�д����ŵ������
    bool IsboolExpression();
    bool boolE();
    bool boolT();
    bool boolF();
    bool T();
    bool F();
    bool E();
    bool myIsExpression();
    bool IsConditionalExpression();
    bool IsSubStatementBlock();
};
/*****bool���ʽ��ص�һ�ף�****/
bool Grammar::IsboolExpression() {
   // this->pretoken = getNextToken();
   //  cout<<"1#"<<this->pretoken.num<<endl;
        if (boolE())
            return true;
        else
            return false;
}
/*****�ж����*****/
bool Grammar::IsConditionalExpression() {
    if (E()) {
        if (this->pretoken.table == "PT" && ((this->pretoken.num >= 21&& this->pretoken.num <= 26 )|| this->pretoken.num == 4 || this->pretoken.num == 5)) {
          //  this->pretoken = getNextToken();
            int tmp=this->pretoken.num;
            this->pretoken = getNextToken();
            if (E()) {
                   Token a=stk.back();
                   stk.pop_back();
                   Token b=stk.back();
                   Token c;
                    c.table="IT";
                    c.num=addTemp(0,-1);
                   stk.pop_back();
                   fourr[fournum].code=tmp;
                   fourr[fournum].addr1=a.num;
                   fourr[fournum].addr2=b.num;
                   fourr[fournum].result=c.num;
                   cout<<"hre"<<endl;
             //      cout<<fournum<<" "<<tmp<<" "<<operatorOrDelimiter[a.num]<<" "<<operatorOrDelimiter[b.num]<<" "<<operatorOrDelimiter[c.num]<<endl;
                   fournum++;
                   stk.push_back(c);

                return true;
            }
            return false;
        }
        return true;
    }
    return false;
}
bool Grammar::boolE() {
        boolT();
    while(this->pretoken.table=="PT"&&(this->pretoken.num==21))
          {
                Token tmp =pretoken;
                this->pretoken = getNextToken();
                boolT();
                Token a=stk.back();
                stk.pop_back();
                Token b=stk.back();
                stk.pop_back();
                Token c;
                c.table="IT";
                c.num=addTemp(0,-1);
                    fourr[fournum].code=tmp.num;
                   fourr[fournum].addr1=a.num;
                   fourr[fournum].addr2=b.num;
                   fourr[fournum].result=c.num;
                   fournum++;
                stk.push_back(c);
          }

}
bool Grammar::boolT() {
           boolF();
    while(this->pretoken.table=="PT"&&(this->pretoken.num==22))
          {
                Token tmp =pretoken;
                this->pretoken = getNextToken();
                boolF();
                Token a=stk.back();
                stk.pop_back();
             //   getType(a);
                Token b=stk.back();
                stk.pop_back();
                Token c;
                c.table="IT";
                c.num=addTemp(0,-1);

                    fourr[fournum].code=tmp.num;
                   fourr[fournum].addr1=a.num;
                   fourr[fournum].addr2=b.num;
                   fourr[fournum].result=c.num;
                 //  cout<<fournum<<" "<<tmp<<" "<<a.num<<" "<<b.num<<" "<<c.num<<endl;
                   fournum++;
                stk.push_back(c);
           //             cout<<"this1c"<<c.num<<endl;
          }
}
bool Grammar::boolF() {
    if ((this->pretoken.table == "PT")&&(this->pretoken.num==20)) {//����ǣ�
        this->pretoken = getNextToken();
        boolE();
        Token a=stk.back();
        stk.pop_back();
        Token c;
        c.num=addTemp(0,-1)+100;
     //    cout<<"????"<<endl;
        c.table="PT";
        stk.push_back(c);
        return true;
    }
    else  if (this->pretoken.table == "PT" && this->pretoken.num == 8) {
            this->pretoken=getNextToken();
            boolE();
            this->pretoken=getNextToken();
        return true;
        }
    else
        {    //cout<<"second="<<this->pretoken.num<<endl;
            myIsExpression();
           // cout<<""<<this->pretoken.num<<endl;
            if(this->pretoken.table == "PT" &&(this->pretoken.num == 4||this->pretoken.num == 5||this->pretoken.num == 23||this->pretoken.num == 24||this->pretoken.num == 25||this->pretoken.num == 26))
            {
                Token c;
                c.table=this->pretoken.table;
                c.num=this->pretoken.num;
                this->pretoken=getNextToken();
                IsExpression();
                Token a=stk.back();
                stk.pop_back();
                Token b= stk.back();
                stk.pop_back();
                Token d;
                d.table="IT";
                d.num=addTemp(0,-1);
        //         cout<<"????"<<endl;
                    fourr[fournum].code=pretoken.num;
                   fourr[fournum].addr1=b.num;
                   fourr[fournum].addr2=a.num;
                   fourr[fournum].result=c.num;

                 //  cout<<fournum<<" "<<tmp<<" "<<a.num<<" "<<b.num<<" "<<c.num<<endl;
                   fournum++;
               cout<<fournum<<"  "<<pretoken.num<<" "<<b.num<<" "<<a.num<<" "<<c.num<<endl;
                stk.push_back(c);
                      //  cout<<"this2c"<<c.num<<endl;
              //  cout<<"this"<<endl;



            }
        }
    }
/*****�������ʽ��ص�һ�ף�****/
bool Grammar::myIsExpression() {
   // this->pretoken = getNextToken();
     cout<<"0"<<this->pretoken.num<<endl;
        if (E())
            return true;
        else
            return false;
}
bool Grammar::E() {
        T();
    while(this->pretoken.table=="PT"&&(this->pretoken.num==0||this->pretoken.num==1))//
          {
                Token tmp =pretoken;
                this->pretoken = getNextToken();
                T();
                Token a=stk.back();
                stk.pop_back();
//getType(a);
                Token b=stk.back();
                stk.pop_back();
                Token c;
                c.table="IT";
                c.num=addTemp(0,-1);
                    fourr[fournum].code=tmp.num;
                   fourr[fournum].addr1=b.num;
                   fourr[fournum].addr2=a.num;
                   fourr[fournum].result=c.num;
                 //  cout<<fournum<<" "<<tmp<<" "<<a.num<<" "<<b.num<<" "<<c.num<<endl;

                cout<<fournum<<"  "<<"  "<<tmp.num<<" "<<b.num<<" "<<a.num<<" "<<c.num<<endl;
                fournum++;
                stk.push_back(c);
                      //  cout<<"this3c"<<c.num<<endl;
          }
}
bool Grammar::T() {
        F();
    while(this->pretoken.table=="PT"&&(this->pretoken.num==2||this->pretoken.num==3))
          {
                Token tmp =pretoken;
                this->pretoken = getNextToken();
                F();
                Token a=stk.back();
                stk.pop_back();
             //   getType(a);
                Token b=stk.back();
                stk.pop_back();
                Token c;
                c.table="IT";
                c.num=addTemp(0,-1);

                cout<<fournum++<<"  "<<pretoken.num<<" "<<b.num<<" "<<a.num<<" "<<c.num<<endl;
                stk.push_back(c);
                 //   cout<<"this4"<<c.num<<endl;
          }
}
bool Grammar::F() {
    if ((this->pretoken.table == "IT")||(this->pretoken.table=="CT")) {//����ǳ����������


        stk.push_back(this->pretoken);
     //   cout<<"this"<<this->pretoken.num<<endl;
                    this->pretoken=getNextToken();

        return true;
    }
    else  if (this->pretoken.table == "PT" && this->pretoken.num == 8) {
            this->pretoken=getNextToken();
            E();
            this->pretoken=getNextToken();
             this->pretoken=getNextToken();
        return true;
        }

    }
/******�������ʽ��ص�һ��*****/
bool Grammar::IsExpression() {
    if (IsT()) {
        if (IsSubExpression())
            return true;
        else
            return false;
    }
    return false;
}
bool Grammar::IsSubExpression() {
    if ((this->pretoken.table == "PT") && (this->pretoken.num == 0 || this->pretoken.num == 1)) {
        this->pretoken = getNextToken();
        if (IsT()) {
            return this->IsSubExpression();
        }
        return false;
    }
    else
        return true;
}
bool Grammar::IsT() {
    if (IsF()) {
        if (IsSubT())
            return true;
        else
            return false;
    }
    return false;
}
bool Grammar::IsSubT() {
    if ((this->pretoken.table == "PT") && (this->pretoken.num == 2 || this->pretoken.num == 3)) {
        this->pretoken = getNextToken();
        if (IsF()) {
            return this->IsSubT();
        }
        return false;
    }
    else
        return true;
}
bool Grammar::IsF() {
    if (this->pretoken.table == "CT") {
        this->pretoken = getNextToken();
        return true;
    }
    else if (isIdentifer()) {
        return true;
    }
    else {
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {
            this->pretoken = getNextToken();
            if (IsExpression()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
            return false;
        }
        return false;
    }
}
/*****������������1Ϊһ�㣬2Ϊ���飬0Ϊ����*****/
int Grammar::isIdentifer() {
    if (this->pretoken.table == "IT") {//�ж��Ƿ�Ϊ��ʶ��������ֱ�ӱ���
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 14) {//�ж��Ƿ�Ϊ��[�������ǵĻ�Ϊһ�����
            this->pretoken = getNextToken();
            if (this->pretoken.table == "CT") {//��Ȼ�Ѿ����ˡ�[������ô��Ȼ[]�м�û�г�����Ҫ������
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 15)//�ж��Ƿ�Ϊ��]��
                    this->pretoken = getNextToken();
                else
                    return 0;
                return 2;//��]�ʶ�Ϊ�������
            }
            else
                return 0;
        }
        return 1;
    }
    return 0;
}
/*****�������Ԫ��ֱ�Ӹ�ֵ���������ţ�*****/
bool Grammar::isSubarrayvalue() {
    if (this->pretoken.table == "CT" || this->pretoken.table == "cT") {
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//�ж��Ƿ�Ϊ��,��
            this->pretoken = getNextToken();
            return this->isSubarrayvalue();//�ҵݹ�ʶ������ж�1��2��3��4���������
        }
        else
            return true;
    }
    return false;
}

/*****�������ŵĶ������Ԫ��ֱ�Ӹ�ֵ*****/
bool Grammar::isArrayvalue() {
    if (this->pretoken.table == "PT" && this->pretoken.num == 16) {//�ж��Ƿ�Ϊ��{��
        this->pretoken = getNextToken();
        bool b = this->isSubarrayvalue();
        if (!b)//ֱ��������һ����Ϊ���ŵķ���
            return b;
    }
    if (this->pretoken.table == "PT" && this->pretoken.num == 17) {//�жϸ÷����Ƿ�Ϊ��}�������������ȷ����ȡ��һ��
        this->pretoken = getNextToken();
        return true;
    }
    return false;
}
/*****��������*****/
bool Grammar::isStatement() {
    if (this->isType()) {
        this->pretoken = getNextToken();
        bool b = isInit();
        if (!b)
            return false;
        if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//�ж��Ƿ�Ϊ��;��
            return true;
        }
        else
            return false;
    }
    return true;
}
/*****��������֮�б���ֱ�Ӹ�ֵ*****/
bool Grammar::isInit() {
    int mark = this->isIdentifer();//�ж���һ����������������
    if (mark == 1) {//һ�����
        if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//�ж��Ƿ�Ϊ��,��
            this->pretoken = getNextToken();
            return this->isInit();
        }
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//�ж��Ƿ�Ϊ��=��
            this->pretoken = getNextToken();
            bool b = IsExpression();//��������֮��ֱ�Ӹ�ֵ���
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//�ж��Ƿ�Ϊ��,��
                this->pretoken = getNextToken();
                return this->isInit();
            }
        }
    }
    else if (mark == 2) {//�������
        if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//�ж��Ƿ�Ϊ��,��
            this->pretoken = getNextToken();
            return this->isInit();
        }
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//�ж��Ƿ�Ϊ��=��
            this->pretoken = getNextToken();
            bool b = this->isArrayvalue();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//�ж��Ƿ�Ϊ��,��
                this->pretoken = getNextToken();
                return this->isInit();
            }
        }
    }
    else
        return false;
    return true;
}
/*****�ж�a,b,c,d......�����*****/
bool Grammar::isInit1() {
    int b = this->isIdentifer();
    if (!b)//�����������
        return false;
    if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//�ж��Ƿ�Ϊ��,��
        this->pretoken = getNextToken();
        return this->isInit1();
    }
    return true;
}
/******��������֮���������ֱ�Ӹ�ֵ*****/
bool Grammar::isInit2() {
    if (this->pretoken.table == "PT" && this->pretoken.num == 14) {//�ж��Ƿ�Ϊ��[��
        this->pretoken = getNextToken();
        if (this->pretoken.table == "CT") {
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 15) {//�ж��Ƿ�Ϊ��]��
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//�ж��Ƿ�Ϊ��,��
                    this->pretoken = getNextToken();
                    return this->isInit();
                }
                else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//�ж��Ƿ�Ϊ��=��
                    this->pretoken = getNextToken();
                    bool b = this->isArrayvalue();
                    if (!b)
                        return false;
                    if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//�ж��Ƿ�Ϊ��,��
                        this->pretoken = getNextToken();
                        return this->isInit();
                    }
                }
            }

        }
    }
    else {
        if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//�ж��Ƿ�Ϊ��,��
            this->pretoken = getNextToken();
            return this->isInit();
        }
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//�ж��Ƿ�Ϊ��=��
            this->pretoken = getNextToken();
            bool b = IsExpression();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//�ж��Ƿ�Ϊ��,��
                this->pretoken = getNextToken();
                return this->isInit();
            }
        }
    }

    return false;
}
/*****�ж�����������C���Գ���*****/
bool Grammar::isProgram() {
    while (this->isSubProgram()) {
        if (this->pretoken.num == -1) {//�����ˣ�û�б���������C���Գ���
            return true;
        }
    }
    return false;

}
/*****�ж�C���Գ���*****/
bool Grammar::isSubProgram() {
    if (this->isType()) {
        this->pretoken = getNextToken();
        if (this->pretoken.table == "KT" && this->pretoken.num == 20) {//�ж��Ƿ�Ϊmain
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) {
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {
                    this->pretoken = getNextToken();
                    if (this->pretoken.table == "PT" && this->pretoken.num == 16) {;
                        this->pretoken = getNextToken();
                        bool b = IsStatementBlock();
                        if (!b)
                            return false;
                        if (this->pretoken.table == "PT" && this->pretoken.num == 17) {
                            this->pretoken = getNextToken();
                            return true;
                        }
                        else
                            return false;
                    }

                }
            }
        }
        else if (this->pretoken.table == "IT") {//�ж��Ƿ�Ϊ��ʶ��
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�ж��Ƿ�Ϊ����������
                this->pretoken = getNextToken();
                while (this->isParameter()) {
                    if (this->pretoken.table == "PT" && this->pretoken.num == 11) {
                        this->pretoken = getNextToken();
                    }
                    else
                        break;
                }
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {
                    this->pretoken = getNextToken();
                    if (this->pretoken.table == "PT" && this->pretoken.num == 16) {
                        this->pretoken = getNextToken();
                        bool b = IsStatementBlock();
                        if (!b)
                            return false;
                        if (this->pretoken.table == "PT" && this->pretoken.num == 17) {
                            this->pretoken = getNextToken();
                            return true;
                        }
                        else
                            return false;
                    }
                    else
                        return false;
                }
                else
                    return false;
            }
            else {//ȫ�ֱ���������
                if (this->isInit2()) {
                    if (this->pretoken.table == "PT" && this->pretoken.num == 7) {
                        this->pretoken = getNextToken();
                        return true;
                    }
                    return false;
                }
                else
                    return false;
            }

        }
        else
            return false;
    }
    else if (this->pretoken.table == "KT" && this->pretoken.num == 3) {//void
        this->pretoken = getNextToken();
        if (this->pretoken.table == "KT" && this->pretoken.num == 20) {//�ж��Ƿ�Ϊmain
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) //�ж��Ƿ�Ϊ����������
                this->pretoken = getNextToken();
            else
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9)
                this->pretoken = getNextToken();
            else
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 16) {
                this->pretoken = getNextToken();
                bool b = IsStatementBlock();
                if (!b)
                    return false;
                if (this->pretoken.table == "PT" && this->pretoken.num == 17) {
                    this->pretoken = getNextToken();
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
        else if (this->pretoken.table == "IT") {//�ж��Ƿ�Ϊ��ʶ��
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�ж��Ƿ�Ϊ����������
                this->pretoken = getNextToken();
                while (this->isParameter()) {
                    if (this->pretoken.table == "PT" && this->pretoken.num == 11) {
                        this->pretoken = getNextToken();
                    }
                    else
                        break;
                }
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {
                    this->pretoken = getNextToken();
                    if (this->pretoken.table == "PT" && this->pretoken.num == 16) {
                        this->pretoken = getNextToken();
                        bool b = IsStatementBlock();
                        if (!b)
                            return false;
                        if (this->pretoken.table == "PT" && this->pretoken.num == 17) {
                            this->pretoken = getNextToken();
                            return true;
                        }
                        else
                            return false;
                    }
                    else
                        return false;
                }
                else
                    return false;
            }
            else
                return false;
        }
    }
    else
        return false;
}
/*****�ṹ��ר������*****/
bool Grammar::isStatement1() {
    if (this->isType()) {
        this->pretoken = getNextToken();
        bool b = isInit1();
        if (!b)
            return false;
        if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//�ж��Ƿ�Ϊ��;��
            this->pretoken = getNextToken();
            return true;
        }
        else
            return false;
    }
    return false;
}
/*****�ṹ��*****/
bool Grammar::isStruct() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 17) {//�ж��Ƿ�Ϊ��struct��
        this->pretoken = getNextToken();
        if (this->pretoken.table == "IT") {//�ж��Ƿ�Ϊ��ʶ��
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 16) {//�ж��Ƿ�Ϊ��{��
                this->pretoken = getNextToken();
                while (!(this->pretoken.table == "PT" && this->pretoken.num == 17)) {//�ж��Ƿ�Ϊ��}��
                    bool b = isStatement1();
                    if (!b)
                        return b;
                }
                return true;
            }
        }
    }
    return false;
}
/*****��������*****/
bool Grammar::isFunCall() {
    if (this->pretoken.table == "IT") {//�Ƿ��ʶ�����ǵĻ�����������û������
        if (getKind(this->pretoken.num))//���ű����
            return false;
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�Ƿ�Ϊ(
            this->pretoken = getNextToken();
            bool b = this->isInit1();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//�Ƿ�Ϊ)
                this->pretoken = getNextToken();
                return true;
            }
        }
    }
    return false;
}
/*****�жϲ���*****/
bool Grammar::isParameter() {
    if (this->isType()) {
        this->pretoken = getNextToken();
        if (this->pretoken.table == "IT") {
            return true;
        }
    }
    return false;
}
/*****����******/
bool Grammar::IsSubStatementBlock() {
   cout<<"this is "<<this->pretoken.table<<this->pretoken.num <<endl;
    if (this->pretoken.table == "KT" && (this->pretoken.num == 0 || this->pretoken.num == 1 || this->pretoken.num == 2)) {//�ǲ�������
        bool b = isStatement();
        if (!b)
            return b;
    }
    else if (this->pretoken.table == "IT"){ //�Ƿ��Ǳ�ʶ�����ǵĻ�������Ǹ�ֵ���ߺ����������
       stk.push_back(pretoken);
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//�����ǲ��Ǹ�ֵ
            this->pretoken = getNextToken();
            if (myIsExpression()) {
                int a=stk.back().num;
            stk.pop_back();
            int b=stk.back().num;
            stk.pop_back();
                    fourr[fournum].code=6;
                    fourr[fournum].addr1=a;
                    fourr[fournum].addr2=-1;
                    fourr[fournum].result=b;
                 //  cout<<fournum<<" "<<tmp<<" "<<a.num<<" "<<b.num<<" "<<c.num<<endl;
                   fournum++;
                    cout<<fournum<<"  "<<"= "<<a<<" "<<"_ "<<b<<endl;
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//�Ƿ��Ƿֺ�
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
            return false;
        }}
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//�ǲ��Ǻ�������
            this->pretoken.num = record.searchVariable(this->pretoken.num);//����token
         //   outF << this->pretoken.table <<" "<< this->pretoken.num << endl;
            this->pretoken = getNextToken();
            bool b = this->isInit1();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//�Ƿ�Ϊ)
            //    outF << this->pretoken.table << " " << this->pretoken.num << endl;
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//�Ƿ�Ϊ;��β�����Ǳ���
//                    outF << this->pretoken.table << " " << this->pretoken.num << endl;
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
        }
        //while
        else if (this->pretoken.table == "KT" && this->pretoken.num == 10) {
            bool b=IsWLoopStatement();
            if (!b)
                return false;
        }
        //for
        else if (this->pretoken.table == "KT" && this->pretoken.num == 8) {
            bool b = IsFLoopStatement();
            if (!b)
                return false;
        }
        //if
        else if (this->pretoken.table == "KT" && this->pretoken.num == 4) {
            bool b = IsConditionalStatement();
            if (!b)
                return false;
        }

    else
        return false;

}
bool Grammar::isType() {
    if (this->pretoken.table == "KT" && (this->pretoken.num == 0 || this->pretoken.num == 1 || this->pretoken.num == 2)) {
        //KT��ǰ������int��float��char
        return true;
    }
    else if (this->pretoken.table == "KT" && this->pretoken.num == 17) {//�жϽṹ��
        this->pretoken = getNextToken();
        if (this->pretoken.table == "IT") {
            return true;
        }
    }
    return false;
}
bool Grammar::IsStatementBlock() {//ʵ��������substatementblock
    if (this->pretoken.table == "PT" && this->pretoken.num == 16) {//��������ͷ
//        outF << "PT " << 16 << endl;
        this->pretoken = getNextToken();
        while (!(this->pretoken.table == "PT" && this->pretoken.num == 17)) {//û�ж��������ŵ�����£�ֻҪ���Ǽ���������
            bool anystatement = IsSubStatementBlock();
            if (!anystatement)
                return anystatement;
        }
//        outF << "PT " << 17 << endl;
        this->pretoken = getNextToken();
        return true;//ֱ�����������ţ�������ȷ
    }
    else
        return false;
}
/*****�������*****/
bool Grammar::IsSingleStatement() {
    if (isStatement()) {
        return true;
    }
    else if (isFunCall()) {
        return true;
    }
    else if (IsAssignmentStatement()) {
        return true;
    }
    else if (IsReturnSatement()) {
        return true;
    }
    else
        return false;
}
/*****IF�������*****/
bool Grammar::IsConditionalStatement() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 4) {
       // fourr[fournum].code=4;

          //  cout<<"if"<<endl;//�ж��ǲ���IF�����ǵĻ�ֱ�ӱ�����
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�ǵĻ��������ǲ��������ţ����ǵĻ�ֱ�ӱ���
            this->pretoken = getNextToken();
            int iff=fournum;
            if (IsConditionalExpression()) {//�ٿ������ǲ���һ�������ж���䣬���ǵĻ�����
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//���ʽ֮�󿴿��ǲ��������ţ����ǵĻ�����
                    fourr[fournum].code=4;
                    fourr[fournum].addr1=stk.back().num;
                    fournum++;
                    this->pretoken = getNextToken();
                    if(IsStatementBlock()){
                        if(this->pretoken.table == "KT" && this->pretoken.num == 5){
                            if(IsStatementBlock()){
                                return true;
                            }
                            return false;
                        }
                        fourr[iff+1].result=fournum+1;
                        fourr[fournum].code=11;
                        fournum++;
                        return true;//�����ǶԵ�֮��ʹû��elseҲû��ϵ
                    }
                }
                else
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

/*****Whileѭ�����*****/
bool Grammar::IsWLoopStatement() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 10) {//�Ƿ���while����
          fourr[fournum++].code=10;

          int start=fournum;
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�Ƿ�������
            this->pretoken = getNextToken();
            int check=fournum+1;
            cout<<"check="<<check<<endl;
            if (IsConditionalExpression()) {//�Ƿ�Ϊ�ж����
                fourr[fournum].code=9;
                fourr[fournum].addr1=stk.back().num;
                fournum++;
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {;//�Ƿ�������
                    this->pretoken = getNextToken();
                    if(IsStatementBlock()){
                        fourr[fournum].code=11;
                        fourr[fournum].result=start;
                        fourr[fournum].addr1=-1;
                        fourr[fournum].addr2=-1;
                                                fournum++;
                        fourr[check].result=fournum;
                        return true;
                    }
                    return false;
                }
                return false;
            }
            return false;
        }
        return false;
    }
    return false;
}
/*****Forѭ�����*****/
bool Grammar::IsFLoopStatement() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 8) {//�Ƿ�Ϊfor
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�Ƿ�Ϊ(
            this->pretoken = getNextToken();
            if (IsAssignmentStatement()) {}//Ĭ�ϳ�ʼ������һ����ֵ������i=2
            if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//�ǲ��ǵ�һ���ֺţ�����ʡȥ�ģ����Ǿͱ���
                this->pretoken = getNextToken();
                if (IsExpression()) {}//�ڶ���������һ�����ʽ
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//�ǲ��ǵڶ����ֺţ����Ǿͱ���
                    if (IsAssignmentStatement()) {}//������������ʱĬ���Ǹ�ֵ������i=i+1
                    if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//�Ƿ�Ϊ�������Ǿͱ���
                        this->pretoken = getNextToken();
                        if (this->pretoken.table == "PT" && this->pretoken.num == 16) {//�Ƿ�Ϊ{�����п���
                            this->pretoken = getNextToken();
                            if (IsStatementBlock()) {//��һ������Ļ�
                                if (this->pretoken.table == "PT" && this->pretoken.num == 17) {//�ж��ǲ����һ����ţ����ǵĻ�Ҫ�����ǵĻ�������������
                                    this->pretoken = getNextToken();
                                    return true;
                                }
                                return false;
                            }
                            return false;
                        }
                        else if (IsSingleStatement()) {//û�л����ŵĻ���ôѭ����������һ��������
                            return true;
                        }
                        else
                            return false;
                    }
                    return false;
                }
                return false;
            }
            return false;
        }
        return false;
    }
    return false;
}
/*****�������*****/
bool Grammar::IsReturnSatement() {
    //�Ƿ���return��ͷ������ֱ�ӱ���
    if (this->pretoken.table == "KT" && this->pretoken.num == 15) {
        this->pretoken = getNextToken();
        //�Ƿ��Ƿֺţ��Ǿͷ���true
        if (this->pretoken.table == "PT" && this->pretoken.num == 7) {
            return true;
        }
        //���ǵĻ������ǲ��Ǳ��ʽ�ӷֺ�
        else if (IsExpression()) {
            if (this->pretoken.table == "PT" && this->pretoken.num == 7) {
                this->pretoken = getNextToken();
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}
/*****��ֵ���*****/
bool Grammar::IsAssignmentStatement() {
    if (isIdentifer()) {
        if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//�Ƿ��ǵ��ں�
            this->pretoken = getNextToken();
            if (IsExpression()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//�Ƿ��Ƿֺ�
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
            return false;
        }
        return false;
    }
    else
        return false;
}
int main()
{  tokenBorn("C:\\Users\\Lenovo\\Desktop\\text.txt");
   // string ele[4];
   // char buffer[256];
 //   int i=0;
//readTokens("C:\\Users\\Lenovo\\Desktop\\text.txt") ;
   readTokens("C:\\hero\\bool\\token.txt");
    ofstream outfile;
    outfile.open("data.txt", ios::trunc);
    record.initRstack();
    Grammar g;
    bool a = g.IsSubStatementBlock();
    if (a)
        cout << "true" << endl;
    else
        cout << "false" << endl;
    for(int i=0;i<fournum;i++)
        cout<<i<<" "<<fourr[i].code<<" "<<fourr[i].addr1<<" "<<fourr[i].addr2<<" "<<fourr[i].result<<endl;
    return 0;
}
