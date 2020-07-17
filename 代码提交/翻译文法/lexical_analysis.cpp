#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "lexical_analysis.h"
#include "Recording.h"
Token tokens[1000];//token序列
pfinfl pfi[10];  //函数表
twoele tok[1000];/*词法序列（二元式结构）*/
symbol environment[100];/*符号表*/
anifl anif[100];//数组表

extern Rstack record;


/********查找关键字*****************/
int searchReserve(char reserveWord[][20], char s[]) {
    for (int i = 0; i < 20; i++) {
        if (strcmp(reserveWord[i], s) == 0) {//若成功查找，则返回种别码
            return i + 1;                   //返回种别码
        }
    }
    return -1;                              //查询失败，返回-1，，即为标识符
}

/*********************判断是否为字母********************/
bool IsLetter(char letter) {//注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
    if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || letter == '_') {
        return true;
    }
    else {
        return false;
    }
}

/*****************判断是否为字符（串）*******************/
bool jcharacter(char letter) {//注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
    if (letter == '"' || letter == '\'') {
        return true;
    }
    else {
        return false;
    }
}

/*****************判断是否为数字************************/
bool IsDigit(char digit) {
    if (digit >= '0' && digit <= '9') {
        return true;
    }
    else {
        return false;
    }
}

/*****************判断是否为小数点*********************/
bool Issport(char digit) {
    if (digit == '.') {
        return true;
    }
    else {
        return false;
    }
}

/*****************编译预处理（取出无用字符和注释）********************/
void filterResource(char r[], int pProject) {
    char tempString[10000];
    int count = 0;
    for (int i = 0; i <= pProject; i++)
    {
        if (r[i] == '/' && r[i + 1] == '/')
        {//若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
            while (r[i] != '\n')
            {
                i++;//向后扫描
            }
        }
        if (r[i] == '/' && r[i + 1] == '*')
        {//若为多行注释“/* 。。。*/”则去除该内容
            i += 2;
            while (r[i] != '*' || r[i + 1] != '/')
            {
                i++;//继续扫描
                if (r[i] == '$')
                {
                    printf("注释出错，没有找到 */，程序结束！！！\n");
                    exit(0);
                }
            }
            i += 2;//跨过“*/”
        }
        if (r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r')
        {//若出现无用字符，则过滤；否则加载
            tempString[count++] = r[i];
        }
    }
    tempString[count] = '\0';
    strcpy(r,tempString);//产生净化之后的源程序
}

/****************************分析子程序，算法核心***********************/
void Scanner(int& syn, char resourceProject[], char token[], int& pProject) {//根据DFA的状态转换图设计
    int i, count = 0;//count用来做token[]的指示器，收集有用字符
    char ch;//作为判断使用
    ch = resourceProject[pProject];
    while (ch == ' ') {//过滤空格，防止程序因识别不了空格而结束
        pProject++;
        ch = resourceProject[pProject];
    }
    for (i = 0; i < 20; i++) {//每次收集前先清零
        token[i] = '\0';
    }
    if (IsLetter(resourceProject[pProject])) {    //首字符为字母，可能为标识符，关键字，字符或字符串
        pProject--;
        if (!jcharacter(resourceProject[pProject])) {
            pProject++;
            token[count++] = resourceProject[pProject];//收集
            pProject++;//下移
            while (IsLetter(resourceProject[pProject]) || IsDigit(resourceProject[pProject])) {//后跟字母或数字
                token[count++] = resourceProject[pProject];//收集
                pProject++;//下移
            }//多读了一个字符既是下次将要开始的指针位置
            token[count] = '\0';
            syn = searchReserve(reserveWord, token);//查表找到种别码
            if (syn == -1) {//若不是关键字则是标识符
                syn = 100;//标识符种别码
            }
            return;
        }
        else  if (resourceProject[pProject] == '"') {//上一符号为引号
            pProject++;//下移
            while (!jcharacter(resourceProject[pProject])) {//字符串
                token[count++] = resourceProject[pProject];//收集
                pProject++;//下移
            }
            token[count] = '\0';
            syn = 102;
            return;
        }
        else {
            pProject++;
            token[count] = resourceProject[pProject];
            pProject++;
            syn = 101;
            return;
        }
    }
    else if (IsDigit(resourceProject[pProject])) {//首字符为数字
        while (IsDigit(resourceProject[pProject]) || Issport(resourceProject[pProject])) {//后跟数字或者小数点
            token[count++] = resourceProject[pProject];//收集
            pProject++;
        }//多读了一个字符既是下次将要开始的指针位置
        token[count] = '\0';
        syn = 99;//常数种别码
    }
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ';' || ch == '(' || ch == ')' || ch == '^'
        || ch == ',' || ch == '%' || ch == '['
        || ch == ']' || ch == '{' || ch == '}' || ch == '.' || ch == ':' || ch == '\"' || ch == '\'')
    {//若为运算符或者界符，查表得到结果
        token[0] = resourceProject[pProject];
        token[1] = '\0';//形成单字符串
        for (i = 0; i < 29; i++)
        {//查运算符界符表
            if (strcmp(token, operatorOrDelimiter[i]) == 0)
            {
                syn = 21 + i;//获得种别码，使用了一点技巧，使之呈线性映射
                break;//查到即推出
            }
        }
        pProject++;//指针下移，为下一扫描做准备
        return;
    }
    else  if (resourceProject[pProject] == '<')
    {//<,<=,<<
        pProject++;//后移，超前搜索
        if (resourceProject[pProject] == '=')
        {
            syn = 45;
        }
        else
        {
            pProject--;
            syn = 25;
        }
        pProject++;//指针下移
        return;
    }
    else  if (resourceProject[pProject] == '>')
    {//>,>=,>>
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            syn = 44;
        }
        else
        {
            pProject--;
            syn = 26;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '=')
    {//=.==
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            syn = 46;
        }
        else
        {
            pProject--;
            syn = 27;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '!')
    {//!,!=
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            syn = 47;
        }
        else
        {
            syn = 41;
            pProject--;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '&')
    {//&,&&
        pProject++;
        if (resourceProject[pProject] == '&')
        {
            syn = 43;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '|')
    {//|,||
        pProject++;
        if (resourceProject[pProject] == '|')
        {
            syn = 42;
        }
        else
        {
            pProject--;
            syn = 33;
        }
        pProject++;
        return;
    }

    else  if (resourceProject[pProject] == '$') {//结束标志
        syn = 0;//种别码为0
    }
    else {//不能被以上词法分析识别，则出错。
        printf("error：there is no exist %c \n", ch);
        syn = 0;
    }
}

void changfinial() {  //改变ip,rp,cp分别指向inblank,flblank,ccharacter的表尾
    int i;
    for (i = 0;;i++) {
        if (strcmp(ccharacter[i], "") == 0) {
            cp = i;
            break;
        }
    }
    for (i = 0;;i++) {
        if (inblank[i] == 0) {
            ip = i;
            break;
        }
    }
    for (i = 0;;i++) {
        if (flblank[i] == 0) {
            rp = i;
            break;
        }
    }
}

Token getNextToken() {                           //得到下一个token序列
    return tokens[index++];
}

int getNewtoken() {
    idc++;
    return idc;
}

bool hadValue(int token) {
    if (environment[token - 1].addr == -1)
        return false;
    else
        return true;
}//判断变量否赋值；

bool hadInit(int token) {
    if (strcmp(environment[token - 1].name, "") == 0)
        return false;
    else
        return true;
}//判断是否定义过
/*
int addIdentifier(int token, int type, int kind) {
    bool b = hadInit(token);
    if (b) {
        token = getNewtoken();
    }
    token = record.recordings[record.preindex].addLocalVariable(token);
    if (token == -1)
        return -1;//表示重定义
    if (type == 1)
        environment[token - 1].type = in;
    else if (type == 2)
        environment[token - 1].type = real;
    else if (type == 3)
        environment[token - 1].type = ch;
    else
        cout << " ";

    if (kind == 1)
        environment[token - 1].kind = con;
    else if (kind == 2)
        environment[token - 1].kind = v;
    else if (kind == 3)
        environment[token - 1].kind = vf;
    else cout << " ";

    environment[token - 1].addr = -1;
    strcpy(environment[token - 1].name, IDentifierTbl[token - 1]);
}
*/
int addIdentifier(int token, int type, int kind, int value) {
    bool b = hadInit(token);
    if (b) {
        token = getNewtoken();
    }
    token = record.recordings[record.preindex].addLocalVariable(token);
    if (token == -1)
        return -1;//表示重定义

    if (type == 1)
        environment[token - 1].type = in;
    else if (type == 2)
        environment[token - 1].type = real;
    else if (type == 3)
        environment[token - 1].type = ch;

    if (kind == 1)
        environment[token - 1].kind = con;
    else if (kind == 2)
        environment[token - 1].kind = v;
    else if (kind == 3)
        environment[token - 1].kind = vf;

    environment[token - 1].addr = value;
    strcpy(environment[token - 1].name, IDentifierTbl[token - 1]);
    return token;
}

int addFunction(int token, int type) {
    bool b = hadInit(token);
    if (b) {
        token = getNewtoken();
    }
    token = record.recordings[record.preindex].addLocalVariable(token);
    if (token == -1)
        return -1;//表示重定义

    if (type == 1)
        environment[token - 1].type = in;
    else if (type == 2)
        environment[token - 1].type = real;
    else if (type == 3)
        environment[token - 1].type = ch;
    else if (type == 4)
        environment[token - 1].type = default1;
    else
        cout << " ";
    strcpy(environment[token - 1].name, IDentifierTbl[token - 1]);
    environment[token - 1].kind = f;
    environment[token - 1].FD = fdl;
    environment[token - 1].addr = -1;
    pfi[fdl].fn = 0;
    fdl++;
    return token;
}

int addParameter(int token, int type, int ftoken) {
    bool b = hadInit(token);
    if (b) {
        token = getNewtoken();
    }
    token = record.recordings[record.preindex].addParameter(token);
    if (token == -1)
        return -1;//表示重定义

    pfi[environment[ftoken - 1].FD].fn++;
    strcpy(pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].name, IDentifierTbl[token - 1]);
    pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].kind = vf;
    if (type == 1)
        pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].type = in;
    else if (type == 2)
        pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].type = real;
    else if (type == 3)
        pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].type = ch;
    else
        cout << " ";
    pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].addr = -1;
    return token;
}

int changeValue(int token, int value) {
    environment[token - 1].addr = value;
    return token;
}

int changeArrayValue(int token, int index, int value) {//根据索引填数组的值
    needle[environment[token - 1].addr + index - 1] = value;
    return token;
}

void changidc() {
    while (strcmp(IDentifierTbl[idc + 1], "") != 0)
        idc++;
}

int addTemp(int type, int value = -1) {
    int token = getNewtoken();
    token = record.recordings[record.preindex]. addTempVariable(token);
    if (token == -1)
        return -1;//表示重定义
   // idc++;
    if (type == 1)
        environment[idc].type = in;
    else if (type == 2)
        environment[idc].type = real;
    else if (type == 3)
        environment[idc].type = ch;
    else
        cout << " ";
    environment[idc].addr = value;
    environment[idc].kind = v;
    strcpy(environment[idc].name, "");
    return idc;
}

int getType(int token) {
    return environment[token - 1].type;
}

int getInt(int token) {
    return inblank[environment[token - 1].addr];
}

float getFloat(int token) {
    return flblank[environment[token - 1].addr];
}

char getChar(int token) {
    return ccharacter[environment[token - 1].addr][0];
}

int addArray(int token, int type, int kind, int length) {
    bool b = hadInit(token);
    if (b) {
        token = getNewtoken();
    }
    //token = record.addLocalVariable(token);
    if (token == -1)
        return -1;//表示重定义

    if (type == 1) {
        anif[adl].type = in;
        anif[adl].klenth = 4;
    }
    else if (type == 2) {
        anif[adl].type = real;
        anif[adl].klenth = 8;
    }
    else if (type == 3) {
        anif[adl].type = ch;
        anif[adl].klenth = 1;
    }
    else
        cout << " ";

    if (kind == 0)
        environment[token - 1].kind = f;
    else if (kind == 1)
        environment[token - 1].kind = con;
    else if (kind == 2)
        environment[token - 1].kind = v;
    else if (kind == 3)
        environment[token - 1].kind = vf;
    else cout << " ";
    environment[token - 1].type = a;
    environment[token - 1].addr = np;
    np = np + length;
    environment[token - 1].AD = adl;
    strcpy(environment[token - 1].name, IDentifierTbl[token - 1]);
    anif[adl].down = 0;
    anif[adl].up = length;
    anif[adl].lenth = anif[adl].klenth * length;
    adl++;
}

string getName(int i) {                            //s为你想存储token值为i的标识符的字符数组
    string s;
    s = IDentifierTbl[i - 1];
    return s;
}

string namekey(int i) {                           //s为你想存储token值为i的关键字的字符数组
    string s;
    s = reserveWord[i - 1];
    return s;
}

void changneedl() {                               //初始化索引表
    for (int i = 0;i < 1000;i++)
        needle[i] = -1;
}

void pri(symbol* out) {                            //符号表打印
    for (int g = 0;; g++) {
        if (out[g].kind == default2)break;
        cout << out[g].name;
        int j = strlen(out[g].name);
        while (j < 10) {
            cout << " ";
            j++;
        }
        switch (out[g].type) {
        case in:cout << "int        ";break;
        case real:cout << "float      ";   break;
        case ch:cout << "char       ";  break;
        case a:cout << "arr        ";  break;
        }
        switch (out[g].kind) {
        case f:cout << "function  "; break;
        case v:cout << "var       ";  break;
        case con:cout << "consl     ";  break;
        }
        switch (out[g].addr) {
        case PFINFL:cout << "PFINFL    "; break;
        case VALL:cout << "VALL      ";  break;
        case CONSL:cout << "CONSL      ";  break;
        case ANIFL:cout << "ANIFL      "; break;
        case RINFL:cout << "RINFL      "; break;
        }
        cout << endl;
    }
}

int lensym() {                                    //返回符号表最后一行的下一行
    int i;
    for (i = 0;;i++)
        if (strcmp(environment[i].name, "#") == 0)
            return i;
}


void tabprint() {                                 //打印符号表、函数表、活动记录表、长度表
    cout << "符号表组织如下:" << endl;
    cout << "名字      类型       种类      地址" << endl;
    pri(environment);
    cout << endl;
    int x, y;
    for (x = 0;x < fdl;x++) {
        cout << "函数表如下(包含参数表）:" << endl;
        cout << "参数个数" << endl;
        cout << pfi[x].fn << endl << endl;
        cout << "名字      类型       种类      地址" << endl;
        pri(pfi[x].para);
    }
    cout << endl;
    for (x = 0;x < adl;x++) {
        cout << "数组表如下:" << endl;
        cout << "下标    上标    类型    类型长度  数组长度" << endl;
        cout << anif[x].down << "        " << anif[x].up << "     ";
        switch (anif[x].type) {
        case in:cout << "int        ";break;
        case real:cout << "float      ";   break;
        case ch:cout << " char       ";  break;
        }
        cout << anif[x].klenth << "         " << anif[x].lenth << endl;
    }
    cout << endl;
    return;
}
//文件存入token数组
    bool readTokens(string filename) {
    ifstream inFile;
    inFile.open(filename, ios::in);
    int i = 0;
    int num;
    string table;
    while (inFile >> table >> num) {
        tokens[i].table = table;
        tokens[i].num = num;
    //    cout<<table<<"@@"<<num<<endl;
        i++;
    }
    inFile.close();
    return true;
}

void cblank() {                                   //将常数表分为整数表和实数表
    int k = 1;
    int a = 0, b = 0;
    for (int i = 0;i < 1000;i++) {
        for (int j = 0;j < 50;j++) {
            if (nnumber[i][j] == '.')
                k = 0;
        }
        if (k == 0)flblank[a++] = atof(nnumber[i]);
        else inblank[b++] = atoi(nnumber[i]);
    }
}

int tokenBorn(string filename) {                                 //词法分析，生成token序列
    char resourceProject[10000];
    char token[20] = { 0 };
    int syn = -1, i;//初始化
    int pProject = 0;//源程序指针
    int toki = 0;
    ifstream inFile;
    inFile.open(filename, ios::in);
    inFile.unsetf(ios::skipws);

    /*FILE* fp, * fp1;
    if ((fp = fopen("program.txt", "r")) == NULL) {//打开一个文件，读取其中的源程序
        cout << "can't open this file";
        exit(0);
    }
    resourceProject[pProject] = fgetc(fp);
    while (resourceProject[pProject] != '$') {//将源程序读入resourceProject[]数组
        pProject++;
        resourceProject[pProject] = fgetc(fp);
    }*/
    //fclose(fp);
    while (inFile >> resourceProject[pProject]) {
        pProject++;
    }
    resourceProject[pProject++] = '\0';
    inFile.close();
    cout << endl << "源程序为:" << endl;
    cout << resourceProject << endl;
    filterResource(resourceProject, pProject);//去掉无用部分，简化程序
    cout << endl << "过滤之后的程序:" << endl;
    cout << resourceProject << endl;
    pProject = 0;                             //从头开始读程序
    /*if ((fp1 = fopen("token.txt", "w+")) == NULL) {//打开源程序
        cout << "can't open this file";
        exit(0);
    }*/
    ofstream outFile;
    outFile.open("token.txt", ios::out);
    cout << endl;
    while (syn != 0) { //启动扫描
        Scanner(syn, resourceProject, token, pProject);
        if (syn == 100) {//标识符
            for (i = 0; i < 1000; i++) {//插入标识符表中
                if (strcmp(IDentifierTbl[i], token) == 0) {//已在表中
                    break;
                }
                if (strcmp(IDentifierTbl[i], "") == 0) {//若不在表中，则插入
                    strcpy(IDentifierTbl[i], token);
                    break;
                }
            }
            strcpy(tok[toki].value1, token);
            tok[toki].value2 = i;
            tok[toki].kind = I;
            tokens[toki].table = "IT";
            tokens[toki].num = i;
            printf("(IT ,%2d)\n", i);                //输出序号
            //fprintf(fp1, "(IT ,%2d)\n", i);
            outFile << "IT " << i << endl;
        }
        else if (syn >= 1 && syn <= 20) {//关键字
            strcpy(tok[toki].value1, token);
            tok[toki].value2 = syn - 1;
            tok[toki].kind = K;
            tokens[toki].table = "KT";
            tokens[toki].num = syn - 1;
            printf("(KT ,%2d)\n", syn - 1);
            //fprintf(fp1, "(KT ,%2d)\n", syn-1);
            outFile << "KT " << syn - 1 << endl;
        }
        else if (syn == 99) {//常数
            for (i = 0; i < 1000; i++) {//插入常数表中
                if (strcmp(nnumber[i], "") == 0) {//
                    strcpy(nnumber[i], token);
                    break;
                }
            }
            strcpy(tok[toki].value1, token);
            tok[toki].value2 = i;
            tok[toki].kind = C;
            tokens[toki].table = "CT";
            tokens[toki].num = i;
            printf("(CT ,%2d)\n", i);
            //fprintf(fp1, "(CT ,%2d)\n", i);
            outFile << "CT " << i << endl;
        }
        else if (syn >= 21 && syn <= 49) {//界符
            strcpy(tok[toki].value1, token);
            tok[toki].value2 = syn - 21;
            tok[toki].kind = P;
            tokens[toki].table = "PT";
            tokens[toki].num = syn - 21;
            printf("(PT ,%2d)\n", syn - 21);
            //fprintf(fp1, "(PT ,%2d)\n", syn - 21);
            outFile << "PT " << syn - 21 << endl;
        }
        else if (syn == 101) {//字符
            for (i = 0; i < 1000; i++) {
                if (strcmp(ccharacter[i], "") == 0) {//插入表中
                    strcpy(ccharacter[i], token);
                    break;
                }
            }
            strcpy(tok[toki].value1, token);
            tok[toki].value2 = i;
            tok[toki].kind = c;
            tokens[toki].table = "cT";
            tokens[toki].num = i;
            printf("(cT ,%2d)\n", i);
            //fprintf(fp1, "(cT ,%2d)\n", i);
            outFile << "cT " << i << endl;
        }
        else if (syn == 102) {//字符串
            for (i = 0; i < 1000; i++) {
                if (strcmp(sstring[i], "") == 0) {//插入表中
                    strcpy(sstring[i], token);
                    break;
                }
            }
            strcpy(tok[toki].value1, token);
            tok[toki].value2 = i;
            tok[toki].kind = S;
            tokens[toki].table = "ST";
            tokens[toki].num = i;
            printf("(sT ,%2d)\n", i);
            //fprintf(fp1, "(sT ,%2d)\n", i);
            outFile << "sT " << i << endl;
        }
        tok[toki].loc = toki;
        toki++;
    }
    //fclose(fp1);
    outFile << "KT " << -1 << endl;
    outFile.close();
    changidc();
    return toki;
}

int getKind(int token) {
    return environment[token - 1].kind;
}
int getArrayLenth(int token) {
    return anif[environment[token - 1].AD].up;
}
