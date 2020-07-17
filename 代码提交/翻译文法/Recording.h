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
    /*增添标识符，返回新的token
    返回-1表示该变量存在重复定义*/
    int addParameter(int token);
    int addLocalVariable(int token);
    int addTempVariable(int token);
    int searchVariable(int token);
    //在当前活动记录搜索变量
    int setdisplay(int index, vector<int> distable);
    bool isExist(int token);
    vector<int> getDisplaytable();
    int getGlobalDisplay();
    
};

class Rstack {
private:
    int prelayer;           //当前层
    
public:
    int preindex;           //当前活动记录
    vector<Recording> recordings;
    void riseLayer();
    void dropLayer();
    int searchVariable(int token);
    int initRstack();
    //在当前活动记录，搜索，若没有，则搜索其全局display

};


#endif // !RECORDING_H