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
PT是界符
KT是关键字表
IT是标识符
CT是常数表****/
/****文法类，包含对循环、条件、赋值等等语句的判断函数****/
//Token tokens[50] = { Token("CT",1),Token("PT",2),Token("CT",2),Token("PT",1),Token("IT",5) };
//if 是KT14，(是PT8,)是PT9,{是PT16,}是PT17
//(表达式) {语句} else if {语句} else {语句}
/****
Token tokens[50] = { Token("IT",5),Token("PT",14),Token("CT",14),Token("PT",15) };
isidentifer的测试，标识符[13]
****/
Rstack record;
vector<Token>stk;
typedef struct four
{
    int  code;     //第一元素
    int  addr1; //第二元素
    int  addr2; //第三元素
    int  result;//第四元素
}four;
four fourr[300];
//memset(fourr,-1,1200);
int fournum=0;
class Grammar {
private:
    Token pretoken;
    bool isStatement1();                     //声明语句（结构体专用，不能初始化）
    bool isInit1();                          //判断类似 a,b的情况
public:
    Grammar() {
        pretoken = getNextToken();
    }
    int isIdentifer();                      //返回1为普通变量，返回2为数组，返回0为错误
    bool isSubarrayvalue();                 //判断形如 1,5,3,4,5
    bool isArrayvalue();                    //判断形如 {1,5,3,4,5}
    bool isStatement();                     //声明语句
    bool isInit();                          //判断类似 a,b=c,d={1,2,3,4}
    bool isInit2();                          //判断类似 a,b=c,d={1,2,3,4}
    bool isProgram();                       //c程序
    bool isSubProgram();
    bool isFunction();                      //函数
    bool isStruct();                        //结构体
    bool isMain();                          //main
    bool isFunCall();                       //函数调用
    bool isParameter();                     //参数
    bool isType();                          //类型标识符
    bool IsExpression();                    //判断算数表达式
    bool IsSubExpression();
    bool IsT();                             //算数表达式的因子
    bool IsSubT();
    bool IsF();
    bool IsWLoopStatement();                 //While循环语句
    bool IsFLoopStatement();                 //For循环语句判断
    bool IsReturnSatement();                //返回语句
    bool IsConditionalStatement();          //if条件语句
    bool IsAssignmentStatement();           //赋值语句
    bool IsStatementBlock();                //语句块
    bool IsSingleStatement();               //单句语句，在循环等语句没有大括号的情况下
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
/*****bool表达式相关的一套）****/
bool Grammar::IsboolExpression() {
   // this->pretoken = getNextToken();
   //  cout<<"1#"<<this->pretoken.num<<endl;
        if (boolE())
            return true;
        else
            return false;
}
/*****判断语句*****/
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
    if ((this->pretoken.table == "PT")&&(this->pretoken.num==20)) {//如果是！
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
/*****算数表达式相关的一套）****/
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
    if ((this->pretoken.table == "IT")||(this->pretoken.table=="CT")) {//如果是常数或则变量


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
/******算数表达式相关的一套*****/
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
/*****变量名，返回1为一般，2为数组，0为错误*****/
int Grammar::isIdentifer() {
    if (this->pretoken.table == "IT") {//判断是否为标识符，不是直接报错
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 14) {//判断是否为‘[’，不是的话为一般变量
            this->pretoken = getNextToken();
            if (this->pretoken.table == "CT") {//既然已经有了‘[’，那么自然[]中间没有常数就要报错了
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 15)//判断是否为‘]’
                    this->pretoken = getNextToken();
                else
                    return 0;
                return 2;//有]故而为数组变量
            }
            else
                return 0;
        }
        return 1;
    }
    return 0;
}
/*****多个数组元素直接赋值（不带括号）*****/
bool Grammar::isSubarrayvalue() {
    if (this->pretoken.table == "CT" || this->pretoken.table == "cT") {
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
            this->pretoken = getNextToken();
            return this->isSubarrayvalue();//右递归故而可以判断1，2，3，4这样的情况
        }
        else
            return true;
    }
    return false;
}

/*****带花括号的多个数组元素直接赋值*****/
bool Grammar::isArrayvalue() {
    if (this->pretoken.table == "PT" && this->pretoken.num == 16) {//判断是否为‘{’
        this->pretoken = getNextToken();
        bool b = this->isSubarrayvalue();
        if (!b)//直到遇到第一个不为逗号的符号
            return b;
    }
    if (this->pretoken.table == "PT" && this->pretoken.num == 17) {//判断该符号是否为‘}’，如果是则正确，读取下一词
        this->pretoken = getNextToken();
        return true;
    }
    return false;
}
/*****声明定义*****/
bool Grammar::isStatement() {
    if (this->isType()) {
        this->pretoken = getNextToken();
        bool b = isInit();
        if (!b)
            return false;
        if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//判断是否为‘;’
            return true;
        }
        else
            return false;
    }
    return true;
}
/*****声明定义之中变量直接赋值*****/
bool Grammar::isInit() {
    int mark = this->isIdentifer();//判断是一般变量还是数组变量
    if (mark == 1) {//一般变量
        if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
            this->pretoken = getNextToken();
            return this->isInit();
        }
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//判断是否为‘=’
            this->pretoken = getNextToken();
            bool b = IsExpression();//考虑声明之中直接赋值情况
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
                this->pretoken = getNextToken();
                return this->isInit();
            }
        }
    }
    else if (mark == 2) {//数组变量
        if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
            this->pretoken = getNextToken();
            return this->isInit();
        }
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//判断是否为‘=’
            this->pretoken = getNextToken();
            bool b = this->isArrayvalue();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
                this->pretoken = getNextToken();
                return this->isInit();
            }
        }
    }
    else
        return false;
    return true;
}
/*****判断a,b,c,d......的情况*****/
bool Grammar::isInit1() {
    int b = this->isIdentifer();
    if (!b)//变量错误情况
        return false;
    if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
        this->pretoken = getNextToken();
        return this->isInit1();
    }
    return true;
}
/******声明定义之中数组变量直接赋值*****/
bool Grammar::isInit2() {
    if (this->pretoken.table == "PT" && this->pretoken.num == 14) {//判断是否为“[”
        this->pretoken = getNextToken();
        if (this->pretoken.table == "CT") {
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 15) {//判断是否为“]”
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
                    this->pretoken = getNextToken();
                    return this->isInit();
                }
                else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//判断是否为‘=’
                    this->pretoken = getNextToken();
                    bool b = this->isArrayvalue();
                    if (!b)
                        return false;
                    if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
                        this->pretoken = getNextToken();
                        return this->isInit();
                    }
                }
            }

        }
    }
    else {
        if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
            this->pretoken = getNextToken();
            return this->isInit();
        }
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//判断是否为‘=’
            this->pretoken = getNextToken();
            bool b = IsExpression();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
                this->pretoken = getNextToken();
                return this->isInit();
            }
        }
    }

    return false;
}
/*****判断整个程序是C语言程序*****/
bool Grammar::isProgram() {
    while (this->isSubProgram()) {
        if (this->pretoken.num == -1) {//结束了，没有报错所以是C语言程序
            return true;
        }
    }
    return false;

}
/*****判断C语言程序*****/
bool Grammar::isSubProgram() {
    if (this->isType()) {
        this->pretoken = getNextToken();
        if (this->pretoken.table == "KT" && this->pretoken.num == 20) {//判断是否为main
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
        else if (this->pretoken.table == "IT") {//判断是否为标识符
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//判断是否为‘（’函数
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
            else {//全局变量的声明
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
        if (this->pretoken.table == "KT" && this->pretoken.num == 20) {//判断是否为main
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) //判断是否为‘（’函数
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
        else if (this->pretoken.table == "IT") {//判断是否为标识符
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//判断是否为‘（’函数
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
/*****结构体专用声明*****/
bool Grammar::isStatement1() {
    if (this->isType()) {
        this->pretoken = getNextToken();
        bool b = isInit1();
        if (!b)
            return false;
        if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//判断是否为‘;’
            this->pretoken = getNextToken();
            return true;
        }
        else
            return false;
    }
    return false;
}
/*****结构体*****/
bool Grammar::isStruct() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 17) {//判断是否为‘struct’
        this->pretoken = getNextToken();
        if (this->pretoken.table == "IT") {//判断是否为标识符
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 16) {//判断是否为‘{’
                this->pretoken = getNextToken();
                while (!(this->pretoken.table == "PT" && this->pretoken.num == 17)) {//判断是否为‘}’
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
/*****函数调用*****/
bool Grammar::isFunCall() {
    if (this->pretoken.table == "IT") {//是否标识符，是的话看看后续有没有括号
        if (getKind(this->pretoken.num))//符号表操作
            return false;
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是否为(
            this->pretoken = getNextToken();
            bool b = this->isInit1();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//是否为)
                this->pretoken = getNextToken();
                return true;
            }
        }
    }
    return false;
}
/*****判断参数*****/
bool Grammar::isParameter() {
    if (this->isType()) {
        this->pretoken = getNextToken();
        if (this->pretoken.table == "IT") {
            return true;
        }
    }
    return false;
}
/*****类型******/
bool Grammar::IsSubStatementBlock() {
   cout<<"this is "<<this->pretoken.table<<this->pretoken.num <<endl;
    if (this->pretoken.table == "KT" && (this->pretoken.num == 0 || this->pretoken.num == 1 || this->pretoken.num == 2)) {//是不是声明
        bool b = isStatement();
        if (!b)
            return b;
    }
    else if (this->pretoken.table == "IT"){ //是否是标识符，是的话则可能是赋值或者函数调用语句
       stk.push_back(pretoken);
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//看看是不是赋值
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
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//是否是分号
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
            return false;
        }}
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//是不是函数调用
            this->pretoken.num = record.searchVariable(this->pretoken.num);//更新token
         //   outF << this->pretoken.table <<" "<< this->pretoken.num << endl;
            this->pretoken = getNextToken();
            bool b = this->isInit1();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//是否为)
            //    outF << this->pretoken.table << " " << this->pretoken.num << endl;
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//是否为;结尾，不是报错
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
        //KT表前三就是int，float，char
        return true;
    }
    else if (this->pretoken.table == "KT" && this->pretoken.num == 17) {//判断结构体
        this->pretoken = getNextToken();
        if (this->pretoken.table == "IT") {
            return true;
        }
    }
    return false;
}
bool Grammar::IsStatementBlock() {//实现任意条substatementblock
    if (this->pretoken.table == "PT" && this->pretoken.num == 16) {//左括号起头
//        outF << "PT " << 16 << endl;
        this->pretoken = getNextToken();
        while (!(this->pretoken.table == "PT" && this->pretoken.num == 17)) {//没有读到右括号的情况下，只要是那几个语句就行
            bool anystatement = IsSubStatementBlock();
            if (!anystatement)
                return anystatement;
        }
//        outF << "PT " << 17 << endl;
        this->pretoken = getNextToken();
        return true;//直到读到右括号，返回正确
    }
    else
        return false;
}
/*****单句语句*****/
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
/*****IF条件语句*****/
bool Grammar::IsConditionalStatement() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 4) {
       // fourr[fournum].code=4;

          //  cout<<"if"<<endl;//判断是不是IF，不是的话直接报错即可
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是的话看后面是不是左括号，不是的话直接报错
            this->pretoken = getNextToken();
            int iff=fournum;
            if (IsConditionalExpression()) {//再看后续是不是一个条件判断语句，不是的话报错
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//表达式之后看看是不是右括号，不是的话报错
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
                        return true;//语句块是对的之后即使没有else也没关系
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

/*****While循环语句*****/
bool Grammar::IsWLoopStatement() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 10) {//是否是while单词
          fourr[fournum++].code=10;

          int start=fournum;
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是否左括号
            this->pretoken = getNextToken();
            int check=fournum+1;
            cout<<"check="<<check<<endl;
            if (IsConditionalExpression()) {//是否为判断语句
                fourr[fournum].code=9;
                fourr[fournum].addr1=stk.back().num;
                fournum++;
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {;//是否右括号
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
/*****For循环语句*****/
bool Grammar::IsFLoopStatement() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 8) {//是否为for
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是否为(
            this->pretoken = getNextToken();
            if (IsAssignmentStatement()) {}//默认初始化就是一个赋值语句比如i=2
            if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//是不是第一个分号，不能省去的，不是就报错
                this->pretoken = getNextToken();
                if (IsExpression()) {}//第二个句子是一个表达式
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//是不是第二个分号，不是就报错
                    if (IsAssignmentStatement()) {}//第三个句子暂时默认是赋值语句比如i=i+1
                    if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//是否为），不是就报错
                        this->pretoken = getNextToken();
                        if (this->pretoken.table == "PT" && this->pretoken.num == 16) {//是否为{，可有可无
                            this->pretoken = getNextToken();
                            if (IsStatementBlock()) {//是一个语句块的话
                                if (this->pretoken.table == "PT" && this->pretoken.num == 17) {//判断是不是右花括号，不是的话要报错，是的话就完美结束了
                                    this->pretoken = getNextToken();
                                    return true;
                                }
                                return false;
                            }
                            return false;
                        }
                        else if (IsSingleStatement()) {//没有花括号的话那么循环的语句就是一个句子了
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
/*****返回语句*****/
bool Grammar::IsReturnSatement() {
    //是否是return开头，不是直接报错
    if (this->pretoken.table == "KT" && this->pretoken.num == 15) {
        this->pretoken = getNextToken();
        //是否是分号，是就返回true
        if (this->pretoken.table == "PT" && this->pretoken.num == 7) {
            return true;
        }
        //不是的话看看是不是表达式加分号
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
/*****赋值语句*****/
bool Grammar::IsAssignmentStatement() {
    if (isIdentifer()) {
        if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//是否是等于号
            this->pretoken = getNextToken();
            if (IsExpression()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//是否是分号
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
