#include "Recording.h"
#include <string>
#include "lexical_analysis.h"


bool Recording::isExist(int token) {
	string name;
	name = getName(token);
	for (int i = 0;i < this->parameter.size();i++) {
		//getname(token)==getname(this->parameter[i])
		if (name==getName(this->parameter[i]))
			return true;
	}
	for (int i = 0;i < this->local_variables.size();i++) {
		//getname(token)==getname(this->local_variables[i])
		if (name== getName(this->local_variables[i]))
			return true;
	}
	return false;
}

int Recording::addParameter(int token)
{
	string name;
	//判断标识符当前层是否已经定义
	bool exist = this->isExist(token);
	if (exist)
		return -1;
	//判断该token值是否被定义，如果已定义，则返回新的token值
	if (hadInit(token)) {
		//申请新的token
		int newtoken=getNewtoken();
		//添加新token到参数表中
		this->parameter.push_back(newtoken);
		return newtoken;
	}
	this->parameter.push_back(token);
	return token;
}
int Recording::addLocalVariable(int token) {
	string name;
	//判断标识符当前层是否已经定义
	bool exist = this->isExist(token);
	if (exist)
		return -1;
	//判断该token值是否被定义，如果已定义，则返回新的token值
	if (hadInit(token)) {
		//申请新的token
		int newtoken = getNewtoken();
		//添加新token到参数表中
		this->local_variables.push_back(newtoken);
		return newtoken;
	}
	this->local_variables.push_back(token);
	return token;

}
int Recording::addTempVariable(int token) {
	//临时变量没必要判断重定义，直接加入token即可
	this->temp_variables.push_back(token);
	return token;
}
int Recording::searchVariable(int token) {//用于单个活动记录查值
	string name;
	name = getName(token);
	for (int i = 0;i < this->parameter.size();i++) {
		//getname(token)==getname(this->parameter[i])
		if (name== getName(this->parameter[i]))
			return this->parameter[i];
	}
	for (int i = 0;i < this->local_variables.size();i++) {
		//getname(token)==getname(this->local_variables[i])
		if (name== getName(this->local_variables[i]))
			return this->parameter[i];
	}
	return -1;
}
int Recording::setdisplay(int index,vector<int> distable) {
	this->global_display = index;
	this->display_table = distable;
	this->old_sp = 0;
	return 0;
}

vector<int> Recording::getDisplaytable() {
	return this->display_table;
}
int Recording::getGlobalDisplay() {
	return this->global_display;
}

/*Rstack相关函数实现*/
//由于该编译器只用于生成汇编语言，运行过程在8086/8088中进行，活动记录的作用不大，扩充起作用到重定义判断，作用域判断，同名变量问题
void Rstack::riseLayer() {
	this->prelayer++;		//层数增加，当前活动记录为其全局活动记录
	int index = this->preindex;
	vector<int> displays = this->recordings[index].getDisplaytable();
	Recording* newrecording = new Recording();
	newrecording->setdisplay(index, displays);		//初始化dispaly
	this->recordings.push_back(*newrecording);
	this->preindex = this->recordings.size() - 1;

}
void Rstack::dropLayer() {
	this->prelayer--;		//层数减小,将当前活动记录指向其全局display
	this->preindex = this->recordings[this->preindex].getGlobalDisplay();
	
}
int Rstack::searchVariable(int token) {//用于更新token值
	int recordindex = this->preindex;
	int newtoken;
	while (true) {
		newtoken = this->recordings[recordindex].searchVariable(token);
		if (newtoken != -1)
			return newtoken;
		if (this->recordings[recordindex].layer == 0)
			return -1;
		recordindex = this->recordings[recordindex].getGlobalDisplay();
	}
}

int Rstack::initRstack() {
	Recording recording(0);
	this->prelayer = 0;
	this->preindex = 0;
	this->recordings.push_back(recording);
	return 0;
}

//int addIdentifier(int type, int identifer, int value);
//tpye的token值 从KT表中查类型，identifer的token从标识符表查标识符，value根据type从相应的表中查值，根据这三个参数填写符号主表等

