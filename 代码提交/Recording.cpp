#include "Recording.h"
#include <string>



bool Recording::isExist(int token) {
	string name;
	//getname获取token所对应的名字
	for (int i = 0;i < this->parameter.size();i++) {
		//getname(token)==getname(this->parameter[i])
		if (1)
			return true;
	}
	for (int i = 0;i < this->local_variables.size();i++) {
		//getname(token)==getname(this->local_variables[i])
		if (1)
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
	if (1) {
		int newtoken=-1;
		//申请新的token

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
	if (1) {
		int newtoken = -1;
		//申请新的token

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
	//getname获取token所对应的名字
	for (int i = 0;i < this->parameter.size();i++) {
		//getname(token)==getname(this->parameter[i])
		if (1)
			return this->parameter[i];
	}
	for (int i = 0;i < this->local_variables.size();i++) {
		//getname(token)==getname(this->local_variables[i])
		if (1)
			return this->parameter[i];
	}
	return -1;
}
int Recording::setdisplay(int index) {
	this->global_display = index;

}

int addIdentifier(int type, int identifer, int value);
//tpye的token值 从KT表中查类型，identifer的token从标识符表查标识符，value根据type从相应的表中查值，根据这三个参数填写符号主表等