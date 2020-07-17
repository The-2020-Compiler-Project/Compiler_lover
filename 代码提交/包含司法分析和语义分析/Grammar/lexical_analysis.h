#ifndef LEXICAL_H
#define LEXICAL_H

#pragma once
#include <string>

using namespace std;

//词法分析程序
//首先定义种别码
/*
第一类  标识符
第二类  常数
第三类  关键字
第四类  界符
第五类  常字符
第六类  字符串
<常数，99>
<标识符，100>
<字符，101>
<字符串，102>
*/
enum style { I, C, K, P, c, S, defaultA };/*二元式种类*/
enum TYP { in, real, ch, a, d, default1 };/*类型，包括int，float,char,数组和结构体型*/
enum CAT { f, con, t, vd, v, vn, vf, default2 };/*种类，包括f函数，c常量，t类型，d域名，v变量，vn换名形参，vf，赋值形参*/
enum ADDR { PFINFL, VALL, CONSL, ANIFL, RINFL, default3 };/*地址表，包括函数表，数组表*/
/****************************************************************************************/
//关键字表
static char reserveWord[21][20] = {
    "int","float","char","void","if","else",
    "switch","case","for","do","while","continue",
    "break","default","sizeof","return" ,"const","struct",
    "scanf","printf","main"
};

//界符表
static char operatorOrDelimiter[29][10] = {
    "+", "-", "*", "/", "<",
    ">", "=", ";", "(", ")",
    "^", ",", "|", "%", "[",
    "]", "{","}", ".", ":",
    "!","||","&&",">=","<=",
    "==","!=","\"","\'"
};


static  char IDentifierTbl[1000][50] = { "" };//标识符表
static  char ccharacter[1000][50] = { "" };//常字符表
static  char sstring[1000][50] = { "" };//字符串表
static  char nnumber[1000][50] = { "" };//实数表
static  int inblank[1000] = { 0 };
static  float flblank[1000] = { 0 };
static  int adl = 0, fdl = 0, idl = 0, rdl = 0, chl = 0, rin = 0;
static  int index = 0;
static  int needle[1000] = { 0 };
static  int np = 0, ip = 0, rp = 0, cp = 0, idc = 0;//idc指已经用了的最后一个标识符token值，idc+1未用
static  int toki = 0;//token序列总数

struct twoele {/*二元式数据结构*/
    style kind;
    char value1[25];
    int value2;
    int loc;                       //记录token序列位置（即第几个）
};

struct symbol {/*符号表*/
    char name[15];    //名字
    TYP type;         //类型
    CAT kind;         //种类
    int addr;
    int FD, AD, ID, RD, CH, RI;
};

struct anifl { /*数组表*/
    int down;           /*下标*/
    int up;             /*上标*/
    TYP type;           /*类型*/
    int klenth;         /*类型长度*/
    int lenth;          /*数组长度*/
};

struct pfinfl {/*函数表*/
    int fn;          //参数个数
    symbol para[5];/*参数表*/
};

class Token {
private:
public:
    string table;
    int num;
    Token() {};
    Token(string table, int num) :table(table), num(num) {
    };
};


extern Token tokens[1000];//token序列
extern pfinfl pfi[10];  //函数表
extern twoele tok[1000];/*词法序列（二元式结构）*/
extern symbol environment[100];/*符号表*/
extern anifl anif[100];//数组表

int tokenBorn(string filename);
//词法分析，生成token序列
Token getNextToken();
//得到下一个token序列
int addIdentifier(int token, int type, int kind, int value=-1);
//初始化时没有赋值（多态）则初始化为0，包含了函数标识符的声明
int addFunction(int token, int type);
//添加函数
int addArray(int token, int type, int kind, int lenth);
//lenth为数组长度
int addParameter(int token, int type, int ftoken); 
//kind为参数种类,ftoken为函数名对应的token；根据添加的参数个数去填写函数的参数个数
int addStruct(int token, int type);
//kind为结构体类型
int addMember(int token, int tpye, int Stoken);
//类型确定，Stoken为结构体的token值
int addTemp(int type, int value);
//添加临时变量，自动分配token，标识符统一规定一个，或者不写
int changeValue(int token, int value);
//value为值所在索引
int changeArrayValue(int token, int index, int value);
//根据索引填数组的值
int changeStructValue(int token, int Subtoken, int value);
//subtoken为成员变量的token，你还得设计结构体该怎么填
int getNewtoken();
//获取一个没有使用的token
int getArrayLenth(int token);
//返回数组长度
string getName(int token);
//获取token对应的标识符
bool hadValue(int token);
//判断变量否赋值；
bool hadInit(int token);
//判断是否定义过
int getType(int token);
//返回类型
int getKind(int token);
int getInt(int token);
float getFloat(int token);
char getChar(int token);
bool readTokens(string filename);

void tabprint();
//打印符号表、函数表、活动记录表、长度表
void changidc();
string nameid(int i);
string namekey(int i);
void changneedl();
void pri(symbol* out);
int lensym();
void cblank();
//将常数表分为整数表和实数表

/********查找关键字*****************/
int searchReserve(char reserveWord[][20], char s[]);
/*********************判断是否为字母********************/
bool IsLetter(char letter);
/*****************判断是否为字符（串）*******************/
bool jcharacter(char letter);
/*****************判断是否为数字************************/
bool IsDigit(char digit);
/*****************判断是否为小数点*********************/
bool Issport(char digit);
/***************** 编译预处理（取出无用字符和注释）********************/
void filterResource(char r[], int pProject);
/****************************分析子程序，算法核心***********************/
void Scanner(int& syn, char resourceProject[], char token[], int& pProject);
void changfinial();
//改变ip,rp,cp分别指向inblank,flblank,ccharacter的表尾


#endif // !LEXICAL_H
