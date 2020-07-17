#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

class Grammar {
private:
    ofstream outF;
    ifstream inF;
    int pretype;
    int prestate;
    int prekind;
    int preindex;
    int prevariable;
    int prevalue;
    Token pretoken;
    bool isStatement1();                     //声明语句（结构体专用，不能初始化）
    bool isInit1();                          //判断类似 a,b的情况
public:
    vector<int> errors;


    Grammar(string filename) {
        pretoken = getNextToken();
        outF.open(filename, ios::out);
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
    bool IsConditionalExpression();             //条件判断
    bool IsWLoopStatement();                 //While循环语句
    bool IsFLoopStatement();                 //For循环语句判断
    bool IsReturnSatement();                //返回语句
    bool IsConditionalStatement();          //if条件语句
    bool IsAssignmentStatement();           //赋值语句
    bool IsStatementBlock();                //语句块
    bool IsSubStatementBlock();
    bool IsSingleStatement();

};

#endif 