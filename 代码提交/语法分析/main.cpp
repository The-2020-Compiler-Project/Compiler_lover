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
using namespace std;
/****
PT是界符
KT是关键字表
IT是标识符
CT是常数表
****/
//关键字表
static char reserveWord[20][20] = {
    "int","float","char","void","if",
    "else","switch","case","for","do",
    "while","continue","break","default","sizeof",
    "return","const","struct","scanf","printf"
};
//界符表
static char operatorOrDelimiter[27][10] = {
    "+", "-", "*", "/", "<",
    ">", "=", ";", "(", ")",
    "^", ",", "|", "%", "[",
    "]", "{","}", ".", ":",
    "!"
    , "||","&&",">=","<=","==", "!="
};
class Token {
public:
    string table;
    int num;
    Token() {};
    Token(string table, int num) :table(table), num(num) {};
};

int index = 0;
Token tokens[100] = {   Token("KT",10),Token("PT",8),

Token("IT",7),Token("PT",24),Token("IT",9),Token("PT",9),
Token("PT",16),

Token("KT",10),Token("PT",8),

Token("IT",7),Token("PT",24),Token("IT",9),Token("PT",9),
Token("PT",16),Token("IT",2),Token("PT",6),Token("IT",2),Token("PT",2),Token("IT",3),Token("PT",7),
Token("IT",3),Token("PT",6),Token("IT",3),Token("PT",0),Token("CT",1),Token("PT",7),
Token("PT",17),

Token("IT",2),Token("PT",6),Token("IT",2),Token("PT",2),Token("IT",3),Token("PT",7),
Token("IT",3),Token("PT",6),Token("IT",3),Token("PT",0),Token("CT",1),Token("PT",7),
Token("PT",17)
 };
 /*****
 测试while嵌套while是否会被判定为合格的语句块还有while，测试代码如下
 while(a <=b){
    i=i*k;
    k=k+1;
    while(a <=b){
        i=i*k;
        k=k+1;
    }
 }
 *****/
/*****
测试备份：完成一个阶乘，内嵌一个同样的内容测是否嵌套
for ( a=1;a<=b ;a=a+2 ){
      i=i*k;
      k=k+1;
      for ( a=1;a<=b ;a=a+2 ){
        i=i*k;
        k=k+1;
      }
}
序列如下，方便复用
Token("KT",8),Token("PT",8),
Token("IT",7),Token("PT",6),Token("CT",1),Token("PT",7),
Token("IT",7),Token("PT",24),Token("IT",9),Token("PT",7),
Token("IT",7),Token("PT",6),Token("IT",2),Token("PT",0),Token("CT",3),Token("PT",9),
Token("PT",16),

Token("KT",8),Token("PT",8),
Token("IT",7),Token("PT",6),Token("CT",1),Token("PT",7),
Token("IT",7),Token("PT",24),Token("IT",9),Token("PT",7),
Token("IT",7),Token("PT",6),Token("IT",2),Token("PT",0),Token("CT",3),Token("PT",9),
Token("PT",16),
Token("IT",2),Token("PT",6),Token("IT",2),Token("PT",2),Token("IT",3),Token("PT",7),
Token("IT",3),Token("PT",6),Token("IT",3),Token("PT",0),Token("CT",1),Token("PT",7),
Token("PT",17),

Token("IT",2),Token("PT",6),Token("IT",2),Token("PT",2),Token("IT",3),Token("PT",7),
Token("IT",3),Token("PT",6),Token("IT",3),Token("PT",0),Token("CT",1),Token("PT",7),
Token("PT",17)
*****/

Token getNextToken() {
    return tokens[index++];
}

/****文法类，包含对循环、条件、赋值等等语句的判断函数****/
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
    bool IsSubExpression();                 //判断语句
    bool IsT();                             //算数表达式的因子
    bool IsSubT();
    bool IsF();
    bool IsConditionalExpression();
    bool IsWLoopStatement();                 //While循环语句
    bool IsFLoopStatement();                 //For循环语句判断
    bool IsReturnSatement();                //返回语句
    bool IsConditionalStatement();          //if条件语句
    bool IsAssignmentStatement();           //赋值语句
    bool IsStatementBlock();                //语句块
    bool IsSubStatementBlock();
    bool IsSingleStatement();               //单句语句，在循环等语句没有大括号的情况下
/*****
isIdentifer();                     OK //返回1为普通变量，返回2为数组，返回0为错误
isSubarrayvalue();                 OK//判断形如 1,5,3,4,5的串
isArrayvalue();                    OK//判断形如 {1,5,3,4,5}的串
isStatement();                     OK//声明语句如int a=b+c*32;
isInit();                          OK//判断类似 a,b=c,d={1,2,3,4}
isInit1();                         OK//判断类似 a,b,c,d
isInit2();                         OK//判断类似 a,b=c,d={1,2,3,4}
isProgram();                        //c程序
isSubProgram();
isFunction();                      无//函数
isStruct();                        OK//结构体
isMain();                          无//main
isFunCall();                       OK//函数调用,类似于 add(a,b);
isParameter();                     OK//函数内的参数调用，但是读完int a就不读了，没有考虑更多
isType();                          OK//类型标识符

IsExpression();                    OK//判断算数表达式
IsSubExpression();                 OK
IsT();                             OK
IsSubT();                          OK
IsF();                             OK
IsWLoopStatement();                OK//While循环语句
IsFLoopStatement();                  //For循环语句判断
IsReturnSatement();                OK//返回语句,如return a;或者return;
IsConditionalStatement();            //if条件语句
IsAssignmentStatement();           OK//赋值语句，如a=54*c;
IsStatementBlock();                  //语句块
IsSingleStatement();               OK//单句语句，指代赋值、声明、返回等等，在循环等语句没有大括号的情况下使用

*****/

};

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

/*****判断语句*****/
bool Grammar::IsConditionalExpression(){
     if(IsExpression() ){
        if(  this->pretoken.table == "PT" && ( (this->pretoken.num >=21 &&this->pretoken.num<=26) || this->pretoken.num ==4 || this->pretoken.num== 5) ){
            this->pretoken = getNextToken();
            if(IsExpression()){
                return true;
            }
            return false;
        }
        return true;
     }
     return false;
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
/*****1,2,3,4（不带括号）*****/
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
/*****带花括号的如{1，2，}*****/
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
/*****声明定义，包括结尾的分号*****/
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
    return false;
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
/******这样的情况*****/
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
/*****判断整个*****/
bool Grammar::isProgram() {
    while (this->isSubProgram()) {
        if (this->pretoken.num == -1) {//结束了，没有报错所以是C语言程序
            return true;
        }
    }
    return false;

}
/*****判断程序*****/
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
    else if(IsSubStatementBlock())
        return true;
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
//        if (getKind(this->pretoken.num))//符号表操作
//          return false;
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是否为(
            this->pretoken = getNextToken();
            bool b = this->isInit1();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//是否为)
                this->pretoken = getNextToken();
                if(this->pretoken.table == "PT" &&this->pretoken.num == 7){//是否为;结尾，不是报错
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
        }
        return false;
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


/*****语句块*****/
bool Grammar::IsSubStatementBlock(){
     if( this->pretoken.table == "KT" &&( this->pretoken.num ==0 ||this->pretoken.num ==1 ||this->pretoken.num ==2)  ){//是不是声明
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
     else if(this->pretoken.table == "IT"){//是否是标识符，是的话则可能是赋值或者函数调用语句
        this->pretoken = getNextToken();
        if(this->pretoken.table == "PT" && this->pretoken.num == 6){//看看是不是赋值
             this->pretoken =getNextToken();
             if (IsExpression()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//是否是分号
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
            return false;
        }
        else if(this->pretoken.table == "PT" && this->pretoken.num == 6){//是不是函数调用
            this->pretoken = getNextToken();
            bool b = this->isInit1();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//是否为)
                this->pretoken = getNextToken();
                if(this->pretoken.table == "PT" &&this->pretoken.num == 7){//是否为;结尾，不是报错
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
        }
     }
/*****while的嵌套*****/
        else if(this->pretoken.table =="KT" && this->pretoken.num ==10 ){
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是否左括号
                    this->pretoken = getNextToken();
                    if (IsConditionalExpression()) {//是否为判断语句
                        if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//是否右括号
                        this->pretoken = getNextToken();
                            if(IsStatementBlock()){
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
/*****for语句的嵌套*****/
        else if (this->pretoken.table == "KT" && this->pretoken.num == 8) {//是否为for
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是否为(
            this->pretoken = getNextToken();
            if (isIdentifer()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//是否是等于号
                    this->pretoken = getNextToken();
                    if (IsExpression()) {
                    /*****这时候已经完成了第一个分号之前的判断*****/
                        if( this->pretoken.table == "PT" && this->pretoken.num == 7 ){
                           this->pretoken = getNextToken();
                           if(IsConditionalExpression()){
                            /*****这里到了第二个分号之前*****/
                             if(this->pretoken.table == "PT" && this->pretoken.num == 7 ){
                                this->pretoken = getNextToken();
                                if(isIdentifer()){
                                    if(this->pretoken.table == "PT" && this->pretoken.num == 6 ){
                                        this->pretoken = getNextToken();
                                        if( IsExpression() ){
                                            if(this->pretoken.table == "PT" && this->pretoken.num == 9){
                                                this->pretoken = getNextToken();
                                                if(IsStatementBlock()){
                                                    return true;
                                                }
                                            }
                                        }
                                    }
                                }
                             }
                           }
                        }
                    }
                    return false;
               }
              return false;
            }
            return false;
        }
        return false;
    }
        /******if的嵌套******/
        else if(this->pretoken.table == "KT" && this->pretoken.num == 4){
                this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是的话看后面是不是左括号，不是的话直接报错
            this->pretoken = getNextToken();
            if (IsConditionalExpression()) {//再看后续是不是一个判断语句，不是的话报错
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//表达式之后看看是不是右括号，不是的话报错
                    this->pretoken = getNextToken();
                    if(IsStatementBlock()){
                        if(this->pretoken.table == "KT" && this->pretoken.num == 5){
                            if(IsStatementBlock()){
                                return true;
                            }
                            return false;
                        }
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
bool Grammar::IsStatementBlock(){//实现任意条substatementblock
    if(this->pretoken.table == "PT" && this->pretoken.num == 16){//左括号起头
        this->pretoken = getNextToken();
        while(!(this->pretoken.table == "PT" && this->pretoken.num == 17)){//没有读到右括号的情况下，只要是那几个语句就行
            bool anystatement = IsSubStatementBlock();
            if(!anystatement)
                return anystatement;
        }
        this->pretoken = getNextToken();
        return true;//直到读到右括号，返回正确
    }
    else
        return false;
}
/*****单句语句*****/
bool Grammar::IsSingleStatement() {
    if (IsAssignmentStatement()) {//声明句可以
        return true;
    }
    else if (isStatement()) {//赋值语句可以
        return true;
    }
    else if (IsReturnSatement()) {//返回语句也可以
        return true;
    }
    else if ( isFunCall()) {//函数调用也可以
        return true;
    }
    //都不是就报错
    return false;
}
/*****IF条件语句*****/
bool Grammar::IsConditionalStatement() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 4) {//判断是不是IF，不是的话直接报错即可
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是的话看后面是不是左括号，不是的话直接报错
            this->pretoken = getNextToken();
            if (IsConditionalExpression()) {//再看后续是不是一个判断语句，不是的话报错
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//表达式之后看看是不是右括号，不是的话报错
                    this->pretoken = getNextToken();
                    if(IsStatementBlock()){
                        if(this->pretoken.table == "KT" && this->pretoken.num == 5){
                            if(IsStatementBlock()){
                                return true;
                            }
                            return false;
                        }
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
    if (this->pretoken.table == "KT" && this->pretoken.num == 10) {//是否为while
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是否左括号
            this->pretoken = getNextToken();
            if (IsConditionalExpression()) {//是否为判断语句
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//是否右括号
                    this->pretoken = getNextToken();
                    if(IsStatementBlock()){
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
            if (isIdentifer()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//是否是等于号
                    this->pretoken = getNextToken();
                    if (IsExpression()) {
                    /*****这时候已经完成了第一个分号之前的判断*****/
                        if( this->pretoken.table == "PT" && this->pretoken.num == 7 ){
                           this->pretoken = getNextToken();
                           if(IsConditionalExpression()){
                            /*****这里到了第二个分号之前*****/
                             if(this->pretoken.table == "PT" && this->pretoken.num == 7 ){
                                this->pretoken = getNextToken();
                                if(isIdentifer()){
                                    if(this->pretoken.table == "PT" && this->pretoken.num == 6 ){
                                        this->pretoken = getNextToken();
                                        if( IsExpression() ){
                                            if(this->pretoken.table == "PT" && this->pretoken.num == 9){
                                                this->pretoken = getNextToken();
                                                if(IsStatementBlock()){
                                                    return true;
                                                }
                                            }
                                        }
                                    }
                                }
                             }
                           }
                        }
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
{
    Grammar g;
    bool b= g.IsSubStatementBlock();
    if (b)
        cout << "while循环嵌套语句块为true" << endl;
    else
        cout << "false" << endl;
   //for循环测试完毕，包括复杂的循环,嵌套也可以
   //语句块测试完毕，包括直接上一个for语句那种也可以

    return 0;
}
