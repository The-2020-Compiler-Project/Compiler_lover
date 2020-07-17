#pragma once
#ifndef RECORDING_H
#define RECORDING_H


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
    int layer;
    Recording(int i=0) :global_display(i) {};
    /*�����ʶ���������µ�token
    ����-1��ʾ�ñ��������ظ�����*/
    int addParameter(int token);
    int addLocalVariable(int token);
    int addTempVariable(int token);
    int searchVariable(int token);
    //�ڵ�ǰ���¼��������
    int setdisplay(int index, vector<int> distable);
    bool isExist(int token);
    vector<int> getDisplaytable();
    int getGlobalDisplay();
    
};

class Rstack {
private:
    int prelayer;           //��ǰ��
    
public:
    int preindex;           //��ǰ���¼
    vector<Recording> recordings;
    void riseLayer();
    void dropLayer();
    int searchVariable(int token);
    int initRstack();
    //�ڵ�ǰ���¼����������û�У���������ȫ��display

};


#endif // !RECORDING_H