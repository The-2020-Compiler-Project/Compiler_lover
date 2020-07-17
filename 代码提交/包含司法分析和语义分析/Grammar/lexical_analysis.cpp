#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "lexical_analysis.h"
#include "Recording.h"

Token tokens[1000];//token����
pfinfl pfi[10];  //������
twoele tok[1000];/*�ʷ����У���Ԫʽ�ṹ��*/
symbol environment[100];/*���ű�*/
anifl anif[100];//�����

extern Rstack record;


/********���ҹؼ���*****************/
int searchReserve(char reserveWord[][20], char s[]) {
    for (int i = 0; i < 21; i++) {
        if (strcmp(reserveWord[i], s) == 0) {//���ɹ����ң��򷵻��ֱ���
            return i + 1;                   //�����ֱ���
        }
    }
    return -1;                              //��ѯʧ�ܣ�����-1������Ϊ��ʶ��
}

/*********************�ж��Ƿ�Ϊ��ĸ********************/
bool IsLetter(char letter) {//ע��C���������»���ҲΪ��ʶ����һ���ֿ��Է����ײ��������ط�
    if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || letter == '_') {
        return true;
    }
    else {
        return false;
    }
}

/*****************�ж��Ƿ�Ϊ�ַ�������*******************/
bool jcharacter(char letter) {//ע��C���������»���ҲΪ��ʶ����һ���ֿ��Է����ײ��������ط�
    if (letter == '"' || letter == '\'') {
        return true;
    }
    else {
        return false;
    }
}

/*****************�ж��Ƿ�Ϊ����************************/
bool IsDigit(char digit) {
    if (digit >= '0' && digit <= '9') {
        return true;
    }
    else {
        return false;
    }
}

/*****************�ж��Ƿ�ΪС����*********************/
bool Issport(char digit) {
    if (digit == '.') {
        return true;
    }
    else {
        return false;
    }
}

/*****************����Ԥ����ȡ�������ַ���ע�ͣ�********************/
void filterResource(char r[], int pProject) {
    char tempString[10000];
    int count = 0;
    for (int i = 0; i <= pProject; i++)
    {
        if (r[i] == '/' && r[i + 1] == '/')
        {//��Ϊ����ע�͡�//��,��ȥ��ע�ͺ���Ķ�����ֱ�������س�����
            while (r[i] != '\n')
            {
                i++;//���ɨ��
            }
        }
        if (r[i] == '/' && r[i + 1] == '*')
        {//��Ϊ����ע�͡�/* ������*/����ȥ��������
            i += 2;
            while (r[i] != '*' || r[i + 1] != '/')
            {
                i++;//����ɨ��
                if (r[i] == '$')
                {
                    printf("ע�ͳ���û���ҵ� */���������������\n");
                    exit(0);
                }
            }
            i += 2;//�����*/��
        }
        if (r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r')
        {//�����������ַ�������ˣ��������
            tempString[count++] = r[i];
        }
    }
    tempString[count] = '\0';
    strcpy_s(r, 10000,tempString);//��������֮���Դ����
}

/****************************�����ӳ����㷨����***********************/
void Scanner(int& syn, char resourceProject[], char token[], int& pProject) {//����DFA��״̬ת��ͼ���
    int i, count = 0;//count������token[]��ָʾ�����ռ������ַ�
    char ch;//��Ϊ�ж�ʹ��
    ch = resourceProject[pProject];
    while (ch == ' ') {//���˿ո񣬷�ֹ������ʶ���˿ո������
        pProject++;
        ch = resourceProject[pProject];
    }
    for (i = 0; i < 20; i++) {//ÿ���ռ�ǰ������
        token[i] = '\0';
    }
    if (IsLetter(resourceProject[pProject])) {    //���ַ�Ϊ��ĸ������Ϊ��ʶ�����ؼ��֣��ַ����ַ���
        pProject--;
        if (!jcharacter(resourceProject[pProject])) {
            pProject++;
            token[count++] = resourceProject[pProject];//�ռ�
            pProject++;//����
            while (IsLetter(resourceProject[pProject]) || IsDigit(resourceProject[pProject])) {//�����ĸ������
                token[count++] = resourceProject[pProject];//�ռ�
                pProject++;//����
            }//�����һ���ַ������´ν�Ҫ��ʼ��ָ��λ��
            token[count] = '\0';
            syn = searchReserve(reserveWord, token);//����ҵ��ֱ���
            if (syn == -1) {//�����ǹؼ������Ǳ�ʶ��
                syn = 100;//��ʶ���ֱ���
            }
            return;
        }
        else  if (resourceProject[pProject] == '"') {//��һ����Ϊ����
            pProject++;//����
            while (!jcharacter(resourceProject[pProject])) {//�ַ���
                token[count++] = resourceProject[pProject];//�ռ�
                pProject++;//����
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
    else if (IsDigit(resourceProject[pProject])) {//���ַ�Ϊ����
        while (IsDigit(resourceProject[pProject]) || Issport(resourceProject[pProject])) {//������ֻ���С����
            token[count++] = resourceProject[pProject];//�ռ�
            pProject++;
        }//�����һ���ַ������´ν�Ҫ��ʼ��ָ��λ��
        token[count] = '\0';
        syn = 99;//�����ֱ���
    }
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ';' || ch == '(' || ch == ')' || ch == '^'
        || ch == ',' || ch == '%' || ch == '['
        || ch == ']' || ch == '{' || ch == '}' || ch == '.' || ch == ':' || ch == '\"' || ch == '\'')
    {//��Ϊ��������߽�������õ����
        token[0] = resourceProject[pProject];
        token[1] = '\0';//�γɵ��ַ���
        for (i = 0; i < 29; i++)
        {//������������
            if (strcmp(token, operatorOrDelimiter[i]) == 0)
            {
                syn = 22 + i;//����ֱ��룬ʹ����һ�㼼�ɣ�ʹ֮������ӳ��
                break;//�鵽���Ƴ�
            }
        }
        pProject++;//ָ�����ƣ�Ϊ��һɨ����׼��
        return;
    }
    else  if (resourceProject[pProject] == '<')
    {//<,<=,<<
        pProject++;//���ƣ���ǰ����
        if (resourceProject[pProject] == '=')
        {
            syn = 46;
        }
        else
        {
            pProject--;
            syn = 26;
        }
        pProject++;//ָ������
        return;
    }
    else  if (resourceProject[pProject] == '>')
    {//>,>=,>>
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            syn = 45;
        }
        else
        {
            pProject--;
            syn = 27;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '=')
    {//=.==
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            syn = 47;
        }
        else
        {
            pProject--;
            syn = 28;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '!')
    {//!,!=
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            syn = 48;
        }
        else
        {
            syn = 42;
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
            syn = 44;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '|')
    {//|,||
        pProject++;
        if (resourceProject[pProject] == '|')
        {
            syn = 43;
        }
        else
        {
            pProject--;
            syn = 34;
        }
        pProject++;
        return;
    }

    else  if (resourceProject[pProject] == '$') {//������־
        syn = 0;//�ֱ���Ϊ0
    }
    else {//���ܱ����ϴʷ�����ʶ�������
        printf("error��there is no exist %c \n", ch);
        syn = 0;
    }
}

void changfinial() {  //�ı�ip,rp,cp�ֱ�ָ��inblank,flblank,ccharacter�ı�β
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

Token getNextToken() {                           //�õ���һ��token����
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
}//�жϱ�����ֵ��

bool hadInit(int token) {
    if (strcmp(environment[token - 1].name, "") == 0)
        return false;
    else
        return true;
}//�ж��Ƿ����
/*
int addIdentifier(int token, int type, int kind) {
    bool b = hadInit(token);
    if (b) {
        token = getNewtoken();
    }
    token = record.recordings[record.preindex].addLocalVariable(token);
    if (token == -1)
        return -1;//��ʾ�ض���
    if (type == 0)
        environment[token - 1].type = in;
    else if (type == 1)
        environment[token - 1].type = real;
    else if (type == 2)
        environment[token - 1].type = ch;
    else
        cout << "define error" << endl;

    if (kind == 1)
        environment[token - 1].kind = con;
    else if (kind == 2)
        environment[token - 1].kind = v;
    else if (kind == 3)
        environment[token - 1].kind = vf;
    else cout << "define error" << endl;

    environment[token - 1].addr = -1;
    strcpy_s(environment[token - 1].name, IDentifierTbl[token - 1]);
}
*/
int addIdentifier(int token, int type, int kind, int value) {
    bool b = hadInit(token);
    if (b) {
        token = getNewtoken();
    }
    token = record.recordings[record.preindex].addLocalVariable(token);
    if (token == -1)
        return -1;//��ʾ�ض���

    if (type == 0)
        environment[token - 1].type = in;
    else if (type == 1)
        environment[token - 1].type = real;
    else if (type == 2)
        environment[token - 1].type = ch;
    else
        cout << "define error" << endl;

    if (kind == 1)
        environment[token - 1].kind = con;
    else if (kind == 2)
        environment[token - 1].kind = v;
    else if (kind == 3)
        environment[token - 1].kind = vf;
    else cout << "define error" << endl;

    environment[token - 1].addr = value;
    strcpy_s(environment[token - 1].name, IDentifierTbl[token - 1]);
    return token;
}

int addFunction(int token, int type) {
    bool b = hadInit(token);
    if (b) {
        token = getNewtoken();
    }
    token = record.recordings[record.preindex].addLocalVariable(token);
    if (token == -1)
        return -1;//��ʾ�ض���

    if (type == 0)
        environment[token - 1].type = in;
    else if (type == 1)
        environment[token - 1].type = real;
    else if (type == 2)
        environment[token - 1].type = ch;
    else if (type == 3)
        environment[token - 1].type = default1;
    else
        cout << "define error" << endl;
    strcpy_s(environment[token - 1].name, IDentifierTbl[token - 1]);
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
        return -1;//��ʾ�ض���

    pfi[environment[ftoken - 1].FD].fn++;
    strcpy_s(pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].name, IDentifierTbl[token - 1]);
    pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].kind = vf;
    if (type == 0)
        pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].type = in;
    else if (type == 1)
        pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].type = real;
    else if (type == 2)
        pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].type = ch;
    else
        cout << "define error" << endl;
    pfi[environment[ftoken - 1].FD].para[token - ftoken - 1].addr = -1;
    return token;
}

int changeValue(int token, int value) {
    environment[token - 1].addr = value;
    return token;
}

int changeArrayValue(int token, int index, int value) {//���������������ֵ
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
        return -1;//��ʾ�ض���
    //idc++;
    if (type == 0)
        environment[idc].type = in;
    else if (type == 1)
        environment[idc].type = real;
    else if (type == 2)
        environment[idc].type = ch;
    else
        cout << "define error" << endl;
    environment[idc].addr = value;
    environment[idc].kind = v;
    strcpy_s(environment[idc].name, "");
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
        return -1;//��ʾ�ض���

    if (type == 0) {
        anif[adl].type = in;
        anif[adl].klenth = 4;
    }
    else if (type == 1) {
        anif[adl].type = real;
        anif[adl].klenth = 8;
    }
    else if (type == 2) {
        anif[adl].type = ch;
        anif[adl].klenth = 1;
    }
    else
        cout << "define error" << endl;

    if (kind == 0)
        environment[token - 1].kind = f;
    else if (kind == 1)
        environment[token - 1].kind = con;
    else if (kind == 2)
        environment[token - 1].kind = v;
    else if (kind == 3)
        environment[token - 1].kind = vf;
    else cout << "define error" << endl;
    environment[token - 1].type = a;
    environment[token - 1].addr = np;
    np = np + length;
    environment[token - 1].AD = adl;
    strcpy_s(environment[token - 1].name, IDentifierTbl[token - 1]);
    anif[adl].down = 0;
    anif[adl].up = length;
    anif[adl].lenth = anif[adl].klenth * length;
    adl++;
}

string getName(int i) {                            //sΪ����洢tokenֵΪi�ı�ʶ�����ַ�����
    string s;
    s = IDentifierTbl[i - 1];
    return s;
}

string namekey(int i) {                           //sΪ����洢tokenֵΪi�Ĺؼ��ֵ��ַ�����
    string s;
    s = reserveWord[i - 1];
    return s;
}

void changneedl() {                               //��ʼ��������
    for (int i = 0;i < 1000;i++)
        needle[i] = -1;
}

void pri(symbol* out) {                            //���ű��ӡ
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

int lensym() {                                    //���ط��ű����һ�е���һ��
    int i;
    for (i = 0;;i++)
        if (strcmp(environment[i].name, "#") == 0)
            return i;
}


void tabprint() {                                 //��ӡ���ű����������¼�����ȱ�
    cout << "���ű���֯����:" << endl;
    cout << "����      ����       ����      ��ַ" << endl;
    pri(environment);
    cout << endl;
    int x, y;
    for (x = 0;x < fdl;x++) {
        cout << "����������(����������:" << endl;
        cout << "��������" << endl;
        cout << pfi[x].fn << endl << endl;
        cout << "����      ����       ����      ��ַ" << endl;
        pri(pfi[x].para);
    }
    cout << endl;
    for (x = 0;x < adl;x++) {
        cout << "���������:" << endl;
        cout << "�±�    �ϱ�    ����    ���ͳ���  ���鳤��" << endl;
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

void cblank() {                                   //���������Ϊ�������ʵ����
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

int tokenBorn(string filename) {                                 //�ʷ�����������token����
    char resourceProject[10000];
    char token[20] = { 0 };
    int syn = -1, i;//��ʼ��
    int pProject = 0;//Դ����ָ��
    int toki = 0;
    ifstream inFile;
    inFile.open(filename, ios::in);
    inFile.unsetf(ios::skipws);

    /*FILE* fp, * fp1;
    if ((fp = fopen("program.txt", "r")) == NULL) {//��һ���ļ�����ȡ���е�Դ����
        cout << "can't open this file";
        exit(0);
    }
    resourceProject[pProject] = fgetc(fp);
    while (resourceProject[pProject] != '$') {//��Դ�������resourceProject[]����
        pProject++;
        resourceProject[pProject] = fgetc(fp);
    }*/
    //fclose(fp);
    while (inFile >> resourceProject[pProject]) {
        pProject++;
    }
    resourceProject[pProject++] = '\0';
    inFile.close();
    cout << endl << "Դ����Ϊ:" << endl;
    cout << resourceProject << endl;
    filterResource(resourceProject, pProject);//ȥ�����ò��֣��򻯳���
    cout << endl << "����֮��ĳ���:" << endl;
    cout << resourceProject << endl;
    pProject = 0;                             //��ͷ��ʼ������
    /*if ((fp1 = fopen("token.txt", "w+")) == NULL) {//��Դ����
        cout << "can't open this file";
        exit(0);
    }*/
    ofstream outFile;
    outFile.open("token.txt", ios::out);
    cout << endl;
    while (syn != 0) { //����ɨ��
        Scanner(syn, resourceProject, token, pProject);
        if (syn == 100) {//��ʶ��
            for (i = 0; i < 1000; i++) {//�����ʶ������
                if (strcmp(IDentifierTbl[i], token) == 0) {//���ڱ���
                    break;
                }
                if (strcmp(IDentifierTbl[i], "") == 0) {//�����ڱ��У������
                    strcpy_s(IDentifierTbl[i], token);
                    break;
                }
            }
            strcpy_s(tok[toki].value1, token);
            tok[toki].value2 = i;
            tok[toki].kind = I;
            tokens[toki].table = "IT";
            tokens[toki].num = i;
            printf("(IT ,%2d)\n", i);                //������
            //fprintf(fp1, "(IT ,%2d)\n", i);
            outFile << "IT " << i << endl;
        }
        else if (syn >= 1 && syn <= 21) {//�ؼ���
            strcpy_s(tok[toki].value1, token);
            tok[toki].value2 = syn - 1;
            tok[toki].kind = K;
            tokens[toki].table = "KT";
            tokens[toki].num = syn - 1;
            printf("(KT ,%2d)\n", syn - 1);
            //fprintf(fp1, "(KT ,%2d)\n", syn-1);
            outFile << "KT " << syn - 1 << endl;
        }
        else if (syn == 99) {//����
            for (i = 0; i < 1000; i++) {//���볣������
                if (strcmp(nnumber[i], "") == 0) {//
                    strcpy_s(nnumber[i], token);
                    break;
                }
            }
            strcpy_s(tok[toki].value1, token);
            tok[toki].value2 = i;
            tok[toki].kind = C;
            tokens[toki].table = "CT";
            tokens[toki].num = i;
            printf("(CT ,%2d)\n", i);
            //fprintf(fp1, "(CT ,%2d)\n", i);
            outFile << "CT " << i << endl;
        }
        else if (syn >= 22 && syn <= 50) {//���
            strcpy_s(tok[toki].value1, token);
            tok[toki].value2 = syn - 22;
            tok[toki].kind = P;
            tokens[toki].table = "PT";
            tokens[toki].num = syn - 22;
            printf("(PT ,%2d)\n", syn - 22);
            //fprintf(fp1, "(PT ,%2d)\n", syn - 22);
            outFile << "PT " << syn - 22 << endl;
        }
        else if (syn == 101) {//�ַ�
            for (i = 0; i < 1000; i++) {
                if (strcmp(ccharacter[i], "") == 0) {//�������
                    strcpy_s(ccharacter[i], token);
                    break;
                }
            }
            strcpy_s(tok[toki].value1, token);
            tok[toki].value2 = i;
            tok[toki].kind = c;
            tokens[toki].table = "cT";
            tokens[toki].num = i;
            printf("(cT ,%2d)\n", i);
            //fprintf(fp1, "(cT ,%2d)\n", i);
            outFile << "cT " << i << endl;
        }
        else if (syn == 102) {//�ַ���
            for (i = 0; i < 1000; i++) {
                if (strcmp(sstring[i], "") == 0) {//�������
                    strcpy_s(sstring[i], token);
                    break;
                }
            }
            strcpy_s(tok[toki].value1, token);
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

bool readTokens(string filename) {
    ifstream inFile;
    inFile.open(filename, ios::in);
    int i = 0;
    int num;
    string table;
    while (inFile >> table >> num) {
        tokens[i].table = table;
        tokens[i].num = num;
        i++;
    }
    tokens[i].table = "KT";
    tokens[i].num = -1;
    inFile.close();
    return true;
}
