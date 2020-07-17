#pragma once


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
    bool isStatement1();                     //������䣨�ṹ��ר�ã����ܳ�ʼ����
    bool isInit1();                          //�ж����� a,b�����
public:
    vector<int> errors;


    Grammar(string filename) {
        pretoken = getNextToken();
        outF.open(filename, ios::out);
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
    bool IsConditionalExpression();             //�����ж�
    bool IsWLoopStatement();                 //Whileѭ�����
    bool IsFLoopStatement();                 //Forѭ������ж�
    bool IsReturnSatement();                //�������
    bool IsConditionalStatement();          //if�������
    bool IsAssignmentStatement();           //��ֵ���
    bool IsStatementBlock();                //����
    bool IsSubStatementBlock();
    bool IsSingleStatement();

};