// Grammar.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
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
#include <fstream>

#include"lexical_analysis.h"
#include"Recording.h"
#include"Grammar.h"

using namespace std;

Rstack record;




//int index = 0;
//Token tokens[50] = { Token("CT",1),Token("PT",2),Token("CT",2),Token("PT",1),Token("IT",5) };
//if 是KT14，(是PT8,)是PT9,{是PT16,}是PT17
//(表达式) {语句} else if {语句} else {语句}
/****
Token tokens[50] = { Token("IT",5),Token("PT",14),Token("CT",14),Token("PT",15) };
isidentifer的测试，标识符[13]
****/


/****文法类，包含对循环、条件、赋值等等语句的判断函数****/






/******算数表达式*****/
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
        outF << this->pretoken.table << " " << this->pretoken.num << endl;
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
        outF << this->pretoken.table << " " << this->pretoken.num << endl;
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
        int temptoken = addTemp(1, this->pretoken.num);//申请临时变量，储存CT
        this->pretoken.table = "IT";
        this->pretoken.num = temptoken;
        outF << this->pretoken.table << " " << this->pretoken.num << endl;
        this->pretoken = getNextToken();
        return true;
    }
    else if (isIdentifer()) {
        this->pretoken.num = record.searchVariable(this->prevariable);
        outF << this->pretoken.table << " " << this->pretoken.num << endl;
        return true;
    }
    else {
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {
            outF << this->pretoken.table << " " << this->pretoken.num << endl;
            this->pretoken = getNextToken();
            if (IsExpression()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {
                    outF << this->pretoken.table << " " << this->pretoken.num << endl;
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
bool Grammar::IsConditionalExpression() {
    if (IsExpression()) {
        if (this->pretoken.table == "PT" && ((this->pretoken.num >= 21&& this->pretoken.num <= 26 )|| this->pretoken.num == 4 || this->pretoken.num == 5)) {
            outF << "PT " << this->pretoken.num << endl;
            this->pretoken = getNextToken();
            if (IsExpression()) {
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
        this->prevariable = this->pretoken.num;
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 14) {//判断是否为‘[’，不是的话为一般变量
            this->pretoken = getNextToken();
            if (this->pretoken.table == "CT") {//既然已经有了‘[’，那么自然[]中间没有常数就要报错了
                preindex = this->pretoken.num;
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 15) {//判断是否为‘]’
                    this->pretoken = getNextToken();
                }
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
    if (this->pretoken.table == "CT" || this->pretoken.table == "cT"||this->pretoken.table=="sT") {
        changeArrayValue(this->prevariable, this->preindex, this->pretoken.num);        //数组赋值
        this->preindex++;                   //索引+1
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
        this->preindex = 0;
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
            this->pretoken = getNextToken();
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
            this->prevariable = addIdentifier(this->prevariable, this->pretype, 2);
            //outF << "IT " << this->prevariable << endl;
            //outF << "PT " << 11 << endl;

            this->pretoken = getNextToken();
            return this->isInit();
        }
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//判断是否为‘=’
            this->prevariable = addIdentifier(this->prevariable, this->pretype, 2);
            outF << "IT " << this->prevariable << endl;
            outF << this->pretoken.table << " " << this->pretoken.num << endl;
            this->pretoken = getNextToken();
            bool b = this->IsExpression();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
                //outF << "PT " << 11 << endl;
                this->pretoken = getNextToken();
                return this->isInit();
            }
        }
    }
    else if (mark == 2) {//数组变量
        
        if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
            this->prevariable = addArray(this->prevariable, this->pretype, 2, this->preindex);
            //outF << "IT " << this->prevariable << endl;
            //outF << "PT " << 14 << endl;
            this->pretoken = getNextToken();
            return this->isInit();
        }
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//判断是否为‘=’
            this->prevariable = addArray(this->prevariable, this->pretype, 2, this->preindex);
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
/*****判断a,b,c,d......的情况,但是结构体还没有实现*****/
bool Grammar::isInit1() {
    int b = this->isIdentifer();
    this->prevariable = record.searchVariable(this->prevariable);
    outF << "IT " << this->prevariable << endl;
    if (!b)//变量错误情况
        return false;
    if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
        outF << "PT " << 11 << endl;
        this->pretoken = getNextToken();
        return this->isInit1();
    }
    return true;
}
/******用于全局操作相关*****/
bool Grammar::isInit2() {
    if (this->pretoken.table == "PT" && this->pretoken.num == 14) {//判断是否为“[”
        this->pretoken = getNextToken();
        if (this->pretoken.table == "CT") {
            this->preindex = this->pretoken.num;
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 15) {//判断是否为“]”
                addArray(this->prevariable, this->pretype, 2, this->preindex);//添加全局变量至符号表
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
            else
                return false;
        }
        else
            return false;
        return true;
    }
    else {
        if (this->pretoken.table == "PT" && this->pretoken.num == 11) {//判断是否为‘,’
            this->prevariable=addIdentifier(this->prevariable, this->pretype, 2);
            this->pretoken = getNextToken();
            return this->isInit();
        }
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//判断是否为‘=’
            addIdentifier(this->prevariable, this->pretype, 2);
            outF << "IT " << this->prevariable << endl;
            outF << this->pretoken.table << " " << this->pretoken.num << endl;
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
    record.initRstack();
    while (this->isSubProgram()) {
        if (this->pretoken.num == -1) {//结束了，没有报错所以是C语言程序
            outF << this->pretoken.table << " " << this->pretoken.num << endl;
            outF.close();
            return true;
        }
    }
    outF.close();
    return false;

}
/*****判断C语言程序*****/
bool Grammar::isSubProgram() {
    if (this->isType()) {
        this->pretoken = getNextToken();
        if (this->pretoken.table == "KT" && this->pretoken.num == 20) {//判断是否为main
            outF << "KT " << this->pretype << endl;
            outF << "KT " << this->pretoken.num << endl;
            record.riseLayer();
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) {
                outF << "PT " << this->pretoken.num << endl;
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {
                    outF << "PT " << this->pretoken.num << endl;
                    this->pretoken = getNextToken();
                    bool b = IsStatementBlock();
                    if (!b)
                        return false;
                    record.dropLayer();
                    return true;
                    
                }
            }
            
        }
        else if (this->pretoken.table == "IT") {//判断是否为标识符
            this->prevariable = this->pretoken.num;
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//判断是否为‘（’函数
                this->prevariable=addFunction(this->prevariable, this->pretype);//添加函数至函数表
                outF << "KT " << this->pretype << endl;
                outF << "IT " << this->prevariable << endl;
                record.riseLayer();                             //进入下一层
                outF << "PT " << this->pretoken.num << endl;
                this->pretoken = getNextToken();
                while (this->isParameter()) {
                    if (this->pretoken.table == "PT" && this->pretoken.num == 11) {
                        outF << "PT " << 11 << endl;
                        this->pretoken = getNextToken();
                    }
                    else
                        break;
                }
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {
                    outF << "PT " << 9 << endl;
                    this->pretoken = getNextToken();
                    bool b = IsStatementBlock();
                    if (!b)
                        return false;
                    record.dropLayer();
                    return true;
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
        this->pretype = this->pretoken.num;
        this->pretoken = getNextToken();
        if (this->pretoken.table == "KT" && this->pretoken.num == 20) {//判断是否为main
            outF << "KT " << this->pretype << endl;
            outF << "KT " << this->pretoken.num << endl;
            record.riseLayer();
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) {
                outF << "PT " << this->pretoken.num << endl;
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {
                    outF << "PT " << this->pretoken.num << endl;
                    this->pretoken = getNextToken();
                    bool b = IsStatementBlock();
                    if (!b)
                        return false;
                    record.dropLayer();
                    return true;
                }
            }

        }
        else if (this->pretoken.table == "IT") {//判断是否为标识符
            this->prevariable = this->pretoken.num;
            this->pretoken = getNextToken();
            if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//判断是否为‘（’函数
                this->prevariable = addFunction(this->prevariable, this->pretype);//添加函数至函数表
                outF << "KT " << this->pretype << endl;
                outF << "IT " << this->prevariable << endl;
                record.riseLayer();                             //进入下一层
                outF << "PT " << this->pretoken.num << endl;
                this->pretoken = getNextToken();
                while (this->isParameter()) {
                    if (this->pretoken.table == "PT" && this->pretoken.num == 11) {
                        outF << "PT " << 11 << endl;
                        this->pretoken = getNextToken();
                    }
                    else
                        break;
                }
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {
                    outF << "PT " << 9 << endl;
                    this->pretoken = getNextToken();
                        bool b = IsStatementBlock();
                        if (!b)
                            return false; 
                        record.dropLayer();
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
        this->pretoken.num = record.searchVariable(this->pretoken.num);
        int kind = getKind(this->pretoken.num);
        if (kind)
            return false;
        outF << "IT " << this->pretoken.num << endl;
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是否为(
            outF << "PT " << 8 << endl;
            this->pretoken = getNextToken();
            bool b = this->isInit1();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//是否为)
                outF << "PT " << 9 << endl;
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//是否为;结尾，不是报错
                    outF << "PT " << 7 << endl;
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
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
            this->prevariable = addParameter(this->pretoken.num, this->pretype, this->prevariable);
            outF << "KT " << this->pretype << endl;
            outF << "IT " << this->prevariable << endl;
            return true;
        }return false;
    }
    return false;
}
/*****类型******/
bool Grammar::isType() {
    if (this->pretoken.table == "KT" && (this->pretoken.num == 0 || this->pretoken.num == 1 || this->pretoken.num == 2)) {
        //KT表前三就是int，float，char
        this->pretype = this->pretoken.num;
        return true;
    }
    else if (this->pretoken.table == "KT" && this->pretoken.num == 17) {//判断结构体
        this->pretoken = getNextToken();
        if (this->pretoken.table == "IT") {
            this->pretype = 3;
            return true;
        }
    }
    return false;
}


/*****语句块，疯狂右递归*****/
bool Grammar::IsStatementBlock() {//实现任意条substatementblock
    if (this->pretoken.table == "PT" && this->pretoken.num == 16) {//左括号起头
        outF << "PT " << 16 << endl;
        this->pretoken = getNextToken();
        while (!(this->pretoken.table == "PT" && this->pretoken.num == 17)) {//没有读到右括号的情况下，只要是那几个语句就行
            bool anystatement = IsSubStatementBlock();
            if (!anystatement)
                return anystatement;
        }
        outF << "PT " << 17 << endl;
        this->pretoken = getNextToken();
        return true;//直到读到右括号，返回正确
    }
    else
        return false;
}

bool Grammar::IsSubStatementBlock() {
    if (this->pretoken.table == "KT" && (this->pretoken.num == 0 || this->pretoken.num == 1 || this->pretoken.num == 2)) {//是不是声明
        bool b = isStatement();
        if (!b)
            return b;
    }
    else if (this->pretoken.table == "IT") {//是否是标识符，是的话则可能是赋值或者函数调用语句
        this->pretoken.num = record.searchVariable(this->pretoken.num);//更新token
        outF << this->pretoken.table<< " " << this->pretoken.num << endl;
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//看看是不是赋值
            outF << "PT " << 6 << endl;
            this->pretoken = getNextToken();
            if (IsExpression()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//是否是分号
                    outF << "PT " << 7 << endl;
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
            return false;
        }
        else if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//是不是函数调用
            this->pretoken.num = record.searchVariable(this->pretoken.num);//更新token
            outF << this->pretoken.table << " " << this->pretoken.num << endl;
            this->pretoken = getNextToken();
            bool b = this->isInit1();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//是否为)
                outF << this->pretoken.table << " " << this->pretoken.num << endl;
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//是否为;结尾，不是报错
                    outF << this->pretoken.table << " " << this->pretoken.num << endl;
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
        }
        else
            return false;
    }
    //while
    else if (this->pretoken.table == "KT" && this->pretoken.num == 10) {
        bool b = IsWLoopStatement();
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


/*****IF条件语句*****/
bool Grammar::IsConditionalStatement() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 4) {//判断是不是IF，不是的话直接报错即可
        outF << this->pretoken.table << " " << this->pretoken.num << endl;
        record.riseLayer();
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是的话看后面是不是左括号，不是的话直接报错
            outF << this->pretoken.table << " " << this->pretoken.num << endl;
            this->pretoken = getNextToken();
            if (IsConditionalExpression()) {//再看后续是不是一个判断语句，不是的话报错
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//表达式之后看看是不是右括号，不是的话报错
                    outF << this->pretoken.table << " " << this->pretoken.num << endl;
                    this->pretoken = getNextToken();
                    if (IsStatementBlock()) {
                        record.dropLayer();
                        if (this->pretoken.table == "KT" && this->pretoken.num == 5) {
                            record.riseLayer();
                            outF << this->pretoken.table << " " << this->pretoken.num << endl;
                            if (IsStatementBlock()) {
                                record.dropLayer();
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
        outF << this->pretoken.table << " " << this->pretoken.num << endl;
        record.riseLayer();
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是否左括号
            outF << this->pretoken.table << " " << this->pretoken.num << endl;
            this->pretoken = getNextToken();
            if (IsConditionalExpression()) {//是否为判断语句
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//是否右括号
                    outF << this->pretoken.table << " " << this->pretoken.num << endl;
                    this->pretoken = getNextToken();
                    if (IsStatementBlock()) {
                        record.dropLayer();
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
        outF << this->pretoken.table << " " << this->pretoken.num << endl;
        record.riseLayer();
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//是否为(
            outF << this->pretoken.table << " " << this->pretoken.num << endl;
            this->pretoken = getNextToken();
            if (isIdentifer()) {
                outF << "IT " << this->prevariable << endl;
                if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//是否是等于号
                    outF << this->pretoken.table << " " << this->pretoken.num << endl;
                    this->pretoken = getNextToken();
                    if (IsExpression()) {
                        /*****这时候已经完成了第一个分号之前的判断*****/
                        if (this->pretoken.table == "PT" && this->pretoken.num == 7) {
                            outF << this->pretoken.table << " " << this->pretoken.num << endl;
                            this->pretoken = getNextToken();
                            if (IsConditionalExpression()) {
                                /*****这里到了第二个分号之前*****/
                                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {
                                    outF << this->pretoken.table << " " << this->pretoken.num << endl;
                                    this->pretoken = getNextToken();
                                    if (isIdentifer()) {
                                        outF << "IT " << this->prevariable << endl;
                                        if (this->pretoken.table == "PT" && this->pretoken.num == 6) {
                                            outF << this->pretoken.table << " " << this->pretoken.num << endl;
                                            this->pretoken = getNextToken();
                                            if (IsExpression()) {
                                                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {
                                                    outF << this->pretoken.table << " " << this->pretoken.num << endl;
                                                    this->pretoken = getNextToken();
                                                    if (IsStatementBlock()) {
                                                        record.dropLayer();
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


int main()
{
    int num = tokenBorn("Program1.txt");
    //readTokens("token.txt");
    Grammar g("Token1.txt");
    bool a = g.isProgram();
    if (a)
        cout << "true" << endl;
    else
        cout << "false" << endl;
    return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
