#ifndef LEXICAL_H
#define LEXICAL_H

#pragma once
#include <string>

using namespace std;

//�ʷ���������
//���ȶ����ֱ���
/*
��һ��  ��ʶ��
�ڶ���  ����
������  �ؼ���
������  ���
������  ���ַ�
������  �ַ���
<������99>
<��ʶ����100>
<�ַ���101>
<�ַ�����102>
*/
enum style { I, C, K, P, c, S, defaultA };/*��Ԫʽ����*/
enum TYP { in, real, ch, a, d, default1 };/*���ͣ�����int��float,char,����ͽṹ����*/
enum CAT { f, con, t, vd, v, vn, vf, default2 };/*���࣬����f������c������t���ͣ�d������v������vn�����βΣ�vf����ֵ�β�*/
enum ADDR { PFINFL, VALL, CONSL, ANIFL, RINFL, default3 };/*��ַ�����������������*/
/****************************************************************************************/
//�ؼ��ֱ�
static char reserveWord[21][20] = {
    "int","float","char","void","if","else",
    "switch","case","for","do","while","continue",
    "break","default","sizeof","return" ,"const","struct",
    "scanf","printf","main"
};

//�����
static char operatorOrDelimiter[29][10] = {
    "+", "-", "*", "/", "<",
    ">", "=", ";", "(", ")",
    "^", ",", "|", "%", "[",
    "]", "{","}", ".", ":",
    "!","||","&&",">=","<=",
    "==","!=","\"","\'"
};


static  char IDentifierTbl[1000][50] = { "" };//��ʶ����
static  char ccharacter[1000][50] = { "" };//���ַ���
static  char sstring[1000][50] = { "" };//�ַ�����
static  char nnumber[1000][50] = { "" };//ʵ����
static  int inblank[1000] = { 0 };
static  float flblank[1000] = { 0 };
static  int adl = 0, fdl = 0, idl = 0, rdl = 0, chl = 0, rin = 0;
static  int index = 0;
static  int needle[1000] = { 0 };
static  int np = 0, ip = 0, rp = 0, cp = 0, idc = 0;//idcָ�Ѿ����˵����һ����ʶ��tokenֵ��idc+1δ��
static  int toki = 0;//token��������

struct twoele {/*��Ԫʽ���ݽṹ*/
    style kind;
    char value1[25];
    int value2;
    int loc;                       //��¼token����λ�ã����ڼ�����
};

struct symbol {/*���ű�*/
    char name[15];    //����
    TYP type;         //����
    CAT kind;         //����
    int addr;
    int FD, AD, ID, RD, CH, RI;
};

struct anifl { /*�����*/
    int down;           /*�±�*/
    int up;             /*�ϱ�*/
    TYP type;           /*����*/
    int klenth;         /*���ͳ���*/
    int lenth;          /*���鳤��*/
};

struct pfinfl {/*������*/
    int fn;          //��������
    symbol para[5];/*������*/
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


extern Token tokens[1000];//token����
extern pfinfl pfi[10];  //������
extern twoele tok[1000];/*�ʷ����У���Ԫʽ�ṹ��*/
extern symbol environment[100];/*���ű�*/
extern anifl anif[100];//�����

int tokenBorn(string filename);
//�ʷ�����������token����
Token getNextToken();
//�õ���һ��token����
int addIdentifier(int token, int type, int kind, int value=-1);
//��ʼ��ʱû�и�ֵ����̬�����ʼ��Ϊ0�������˺�����ʶ��������
int addFunction(int token, int type);
//��Ӻ���
int addArray(int token, int type, int kind, int lenth);
//lenthΪ���鳤��
int addParameter(int token, int type, int ftoken); 
//kindΪ��������,ftokenΪ��������Ӧ��token��������ӵĲ�������ȥ��д�����Ĳ�������
int addStruct(int token, int type);
//kindΪ�ṹ������
int addMember(int token, int tpye, int Stoken);
//����ȷ����StokenΪ�ṹ���tokenֵ
int addTemp(int type, int value);
//�����ʱ�������Զ�����token����ʶ��ͳһ�涨һ�������߲�д
int changeValue(int token, int value);
//valueΪֵ��������
int changeArrayValue(int token, int index, int value);
//���������������ֵ
int changeStructValue(int token, int Subtoken, int value);
//subtokenΪ��Ա������token���㻹����ƽṹ�����ô��
int getNewtoken();
//��ȡһ��û��ʹ�õ�token
int getArrayLenth(int token);
//�������鳤��
string getName(int token);
//��ȡtoken��Ӧ�ı�ʶ��
bool hadValue(int token);
//�жϱ�����ֵ��
bool hadInit(int token);
//�ж��Ƿ����
int getType(int token);
//��������
int getKind(int token);
int getInt(int token);
float getFloat(int token);
char getChar(int token);
bool readTokens(string filename);

void tabprint();
//��ӡ���ű����������¼�����ȱ�
void changidc();
string nameid(int i);
string namekey(int i);
void changneedl();
void pri(symbol* out);
int lensym();
void cblank();
//���������Ϊ�������ʵ����

/********���ҹؼ���*****************/
int searchReserve(char reserveWord[][20], char s[]);
/*********************�ж��Ƿ�Ϊ��ĸ********************/
bool IsLetter(char letter);
/*****************�ж��Ƿ�Ϊ�ַ�������*******************/
bool jcharacter(char letter);
/*****************�ж��Ƿ�Ϊ����************************/
bool IsDigit(char digit);
/*****************�ж��Ƿ�ΪС����*********************/
bool Issport(char digit);
/***************** ����Ԥ����ȡ�������ַ���ע�ͣ�********************/
void filterResource(char r[], int pProject);
/****************************�����ӳ����㷨����***********************/
void Scanner(int& syn, char resourceProject[], char token[], int& pProject);
void changfinial();
//�ı�ip,rp,cp�ֱ�ָ��inblank,flblank,ccharacter�ı�β


#endif // !LEXICAL_H
