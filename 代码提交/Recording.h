#pragma once
#include<vector>

using namespace std;

class Recording {
private:
    int old_sp;
    int global_display;
    int parameter_num;
    vector<int> parameter;
    vector<int> display_table;
    vector<int> local_variables;
    vector<int> dope_vector;
    vector<int> temp_variables;
public:
    /*�����ʶ���������µ�token
    ����-1��ʾ�ñ��������ظ�����*/
    int addParameter(int token);
    int addLocalVariable(int token);
    int addTempVariable(int token);
    int searchVariable(int token);
    int setdisplay(int index);
    bool isExist(int token);
};

class Rstack {
private:
    int prelayer;
    vector<Recording> recordings;
public:
    void riseLayer();
    void dropLayer();

};