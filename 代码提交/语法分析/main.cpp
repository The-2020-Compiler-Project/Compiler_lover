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
PT�ǽ��
KT�ǹؼ��ֱ�
IT�Ǳ�ʶ��
CT�ǳ�����
****/
//�ؼ��ֱ�
static char reserveWord[20][20] = {
    "int","float","char","void","if",
    "else","switch","case","for","do",
    "while","continue","break","default","sizeof",
    "return","const","struct","scanf","printf"
};
//�����
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
 ����whileǶ��while�Ƿ�ᱻ�ж�Ϊ�ϸ�����黹��while�����Դ�������
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
���Ա��ݣ����һ���׳ˣ���Ƕһ��ͬ�������ݲ��Ƿ�Ƕ��
for ( a=1;a<=b ;a=a+2 ){
      i=i*k;
      k=k+1;
      for ( a=1;a<=b ;a=a+2 ){
        i=i*k;
        k=k+1;
      }
}
�������£����㸴��
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

/****�ķ��࣬������ѭ������������ֵ�ȵ������жϺ���****/
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
    bool IsSubExpression();                 //�ж����
    bool IsT();                             //�������ʽ������
    bool IsSubT();
    bool IsF();
    bool IsConditionalExpression();
    bool IsWLoopStatement();                 //Whileѭ�����
    bool IsFLoopStatement();                 //Forѭ������ж�
    bool IsReturnSatement();                //�������
    bool IsConditionalStatement();          //if�������
    bool IsAssignmentStatement();           //��ֵ���
    bool IsStatementBlock();                //����
    bool IsSubStatementBlock();
    bool IsSingleStatement();               //������䣬��ѭ�������û�д����ŵ������
/*****
isIdentifer();                     OK //����1Ϊ��ͨ����������2Ϊ���飬����0Ϊ����
isSubarrayvalue();                 OK//�ж����� 1,5,3,4,5�Ĵ�
isArrayvalue();                    OK//�ж����� {1,5,3,4,5}�Ĵ�
isStatement();                     OK//���������int a=b+c*32;
isInit();                          OK//�ж����� a,b=c,d={1,2,3,4}
isInit1();                         OK//�ж����� a,b,c,d
isInit2();                         OK//�ж����� a,b=c,d={1,2,3,4}
isProgram();                        //c����
isSubProgram();
isFunction();                      ��//����
isStruct();                        OK//�ṹ��
isMain();                          ��//main
isFunCall();                       OK//��������,������ add(a,b);
isParameter();                     OK//�����ڵĲ������ã����Ƕ���int a�Ͳ����ˣ�û�п��Ǹ���
isType();                          OK//���ͱ�ʶ��

IsExpression();                    OK//�ж��������ʽ
IsSubExpression();                 OK
IsT();                             OK
IsSubT();                          OK
IsF();                             OK
IsWLoopStatement();                OK//Whileѭ�����
IsFLoopStatement();                  //Forѭ������ж�
IsReturnSatement();                OK//�������,��return a;����return;
IsConditionalStatement();            //if�������
IsAssignmentStatement();           OK//��ֵ��䣬��a=54*c;
IsStatementBlock();                  //����
IsSingleStatement();               OK//������䣬ָ����ֵ�����������صȵȣ���ѭ�������û�д����ŵ������ʹ��

*****/

};

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

/*****�ж����*****/
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
/*****1,2,3,4���������ţ�*****/
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
/*****�������ŵ���{1��2��}*****/
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
/*****�������壬������β�ķֺ�*****/
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
    return false;
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
/******���������*****/
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
/*****�ж�����*****/
bool Grammar::isProgram() {
    while (this->isSubProgram()) {
        if (this->pretoken.num == -1) {//�����ˣ�û�б���������C���Գ���
            return true;
        }
    }
    return false;

}
/*****�жϳ���*****/
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
    else if(IsSubStatementBlock())
        return true;
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
//        if (getKind(this->pretoken.num))//���ű����
//          return false;
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�Ƿ�Ϊ(
            this->pretoken = getNextToken();
            bool b = this->isInit1();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//�Ƿ�Ϊ)
                this->pretoken = getNextToken();
                if(this->pretoken.table == "PT" &&this->pretoken.num == 7){//�Ƿ�Ϊ;��β�����Ǳ���
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


/*****����*****/
bool Grammar::IsSubStatementBlock(){
     if( this->pretoken.table == "KT" &&( this->pretoken.num ==0 ||this->pretoken.num ==1 ||this->pretoken.num ==2)  ){//�ǲ�������
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
     else if(this->pretoken.table == "IT"){//�Ƿ��Ǳ�ʶ�����ǵĻ�������Ǹ�ֵ���ߺ����������
        this->pretoken = getNextToken();
        if(this->pretoken.table == "PT" && this->pretoken.num == 6){//�����ǲ��Ǹ�ֵ
             this->pretoken =getNextToken();
             if (IsExpression()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 7) {//�Ƿ��Ƿֺ�
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
            return false;
        }
        else if(this->pretoken.table == "PT" && this->pretoken.num == 6){//�ǲ��Ǻ�������
            this->pretoken = getNextToken();
            bool b = this->isInit1();
            if (!b)
                return false;
            if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//�Ƿ�Ϊ)
                this->pretoken = getNextToken();
                if(this->pretoken.table == "PT" &&this->pretoken.num == 7){//�Ƿ�Ϊ;��β�����Ǳ���
                    this->pretoken = getNextToken();
                    return true;
                }
                return false;
            }
        }
     }
/*****while��Ƕ��*****/
        else if(this->pretoken.table =="KT" && this->pretoken.num ==10 ){
                this->pretoken = getNextToken();
                if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�Ƿ�������
                    this->pretoken = getNextToken();
                    if (IsConditionalExpression()) {//�Ƿ�Ϊ�ж����
                        if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//�Ƿ�������
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
/*****for����Ƕ��*****/
        else if (this->pretoken.table == "KT" && this->pretoken.num == 8) {//�Ƿ�Ϊfor
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�Ƿ�Ϊ(
            this->pretoken = getNextToken();
            if (isIdentifer()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//�Ƿ��ǵ��ں�
                    this->pretoken = getNextToken();
                    if (IsExpression()) {
                    /*****��ʱ���Ѿ�����˵�һ���ֺ�֮ǰ���ж�*****/
                        if( this->pretoken.table == "PT" && this->pretoken.num == 7 ){
                           this->pretoken = getNextToken();
                           if(IsConditionalExpression()){
                            /*****���ﵽ�˵ڶ����ֺ�֮ǰ*****/
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
        /******if��Ƕ��******/
        else if(this->pretoken.table == "KT" && this->pretoken.num == 4){
                this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�ǵĻ��������ǲ��������ţ����ǵĻ�ֱ�ӱ���
            this->pretoken = getNextToken();
            if (IsConditionalExpression()) {//�ٿ������ǲ���һ���ж���䣬���ǵĻ�����
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//���ʽ֮�󿴿��ǲ��������ţ����ǵĻ�����
                    this->pretoken = getNextToken();
                    if(IsStatementBlock()){
                        if(this->pretoken.table == "KT" && this->pretoken.num == 5){
                            if(IsStatementBlock()){
                                return true;
                            }
                            return false;
                        }
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
bool Grammar::IsStatementBlock(){//ʵ��������substatementblock
    if(this->pretoken.table == "PT" && this->pretoken.num == 16){//��������ͷ
        this->pretoken = getNextToken();
        while(!(this->pretoken.table == "PT" && this->pretoken.num == 17)){//û�ж��������ŵ�����£�ֻҪ���Ǽ���������
            bool anystatement = IsSubStatementBlock();
            if(!anystatement)
                return anystatement;
        }
        this->pretoken = getNextToken();
        return true;//ֱ�����������ţ�������ȷ
    }
    else
        return false;
}
/*****�������*****/
bool Grammar::IsSingleStatement() {
    if (IsAssignmentStatement()) {//���������
        return true;
    }
    else if (isStatement()) {//��ֵ������
        return true;
    }
    else if (IsReturnSatement()) {//�������Ҳ����
        return true;
    }
    else if ( isFunCall()) {//��������Ҳ����
        return true;
    }
    //�����Ǿͱ���
    return false;
}
/*****IF�������*****/
bool Grammar::IsConditionalStatement() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 4) {//�ж��ǲ���IF�����ǵĻ�ֱ�ӱ�����
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�ǵĻ��������ǲ��������ţ����ǵĻ�ֱ�ӱ���
            this->pretoken = getNextToken();
            if (IsConditionalExpression()) {//�ٿ������ǲ���һ���ж���䣬���ǵĻ�����
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//���ʽ֮�󿴿��ǲ��������ţ����ǵĻ�����
                    this->pretoken = getNextToken();
                    if(IsStatementBlock()){
                        if(this->pretoken.table == "KT" && this->pretoken.num == 5){
                            if(IsStatementBlock()){
                                return true;
                            }
                            return false;
                        }
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
    if (this->pretoken.table == "KT" && this->pretoken.num == 10) {//�Ƿ�Ϊwhile
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�Ƿ�������
            this->pretoken = getNextToken();
            if (IsConditionalExpression()) {//�Ƿ�Ϊ�ж����
                if (this->pretoken.table == "PT" && this->pretoken.num == 9) {//�Ƿ�������
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
/*****Forѭ�����*****/
bool Grammar::IsFLoopStatement() {
    if (this->pretoken.table == "KT" && this->pretoken.num == 8) {//�Ƿ�Ϊfor
        this->pretoken = getNextToken();
        if (this->pretoken.table == "PT" && this->pretoken.num == 8) {//�Ƿ�Ϊ(
            this->pretoken = getNextToken();
            if (isIdentifer()) {
                if (this->pretoken.table == "PT" && this->pretoken.num == 6) {//�Ƿ��ǵ��ں�
                    this->pretoken = getNextToken();
                    if (IsExpression()) {
                    /*****��ʱ���Ѿ�����˵�һ���ֺ�֮ǰ���ж�*****/
                        if( this->pretoken.table == "PT" && this->pretoken.num == 7 ){
                           this->pretoken = getNextToken();
                           if(IsConditionalExpression()){
                            /*****���ﵽ�˵ڶ����ֺ�֮ǰ*****/
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
{
    Grammar g;
    bool b= g.IsSubStatementBlock();
    if (b)
        cout << "whileѭ��Ƕ������Ϊtrue" << endl;
    else
        cout << "false" << endl;
   //forѭ��������ϣ��������ӵ�ѭ��,Ƕ��Ҳ����
   //���������ϣ�����ֱ����һ��for�������Ҳ����

    return 0;
}
