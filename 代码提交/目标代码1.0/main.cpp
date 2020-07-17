#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

struct registers//寄存器
{
	string	name;
	vector<string> Rvalue;
	registers(string n){ name = n; }
	registers(){};
	void clearPush(string n)
	{
		Rvalue.clear();
		Rvalue.push_back(n);
	}
	bool isinReg(string n)
	{
		bool flag = false;
		for (int i = 0; i < Rvalue.size(); i++)
		{
			if (Rvalue[i] == n)
				flag = true;
		}
		return flag;
	}
};
vector<registers> regs = { registers("AX"), registers("BX"), registers("CX") ,registers("DX")};
registers findreg(string arg,int &index)
{
	//如果找到原本有的
	for (int i = 0; i < regs.size(); i++)
	{
		if (regs[i].isinReg(arg))
		{
			index = i;
			return regs[i];
		}
	}
	//没有则返回空寄存器
	for (int i = 0; i < regs.size(); i++)
	{
		if (regs[i].Rvalue.size()==0)
		{
			index = i;
			return regs[i];
		}
	}
}
bool isExistRvalue(string arg)
{
	for (int i = 0; i < regs.size(); i++)
	{
			if (regs[i].isinReg(arg))
				return true;
	}
	return false;
}




struct GenStruct//四元式

{
    int label;    //四元式序号
    int code;     //第一元素
    string addr1="0"; //第二元素
    string addr2="0"; //第三元素
    string result="0";//第四元素
    int out_port = 0;  //记录该四元式是否为一个基本块的入口，是则为1，否则为0。
    void iGenStruct(int l,int c,string a1,string a2,string res)//设定四元式
    {
        label = l;
        code=c;
        addr1 = a1;
        addr2 = a2;
        result = res;
    }
};
vector<GenStruct>genStructs;//四元式存放

void changeOut_port(int a)//改变入口标志
{
    for(int i=0;i<genStructs.size();i++)
    {
        if(genStructs[i].label==a)
        {
            genStructs[i].out_port=1;
        }
    }

}

static int nameNum;//记录基本块数
string int2str( int val )//整型转字符串
{
    ostringstream out;
    out<<val;
    return out.str();
}
int str2int( const string &val )//字符串转整型
{
    istringstream in(val.c_str());
    int ret = 0;
    in>>ret;
    return ret;
}

struct basicBlock//基本块
{
	string name;
	vector<GenStruct> gens;
	vector <string> codes;
	basicBlock()
	{
		string str;
		str=int2str(nameNum);
		name = "L" + str;
		nameNum++;
	}
	void add(GenStruct g)
	{
		gens.push_back(g);
	}
};


class codeTable//代码表
{
private:
	vector<basicBlock> block; //基本块
public:

    void initblock();  //初始化基本块
	int findblocknameByGen(int index);  //根据四元式地址找到基本块
	string findblocknameByGen(string index){
		int i = stoi(index);
		i=findblocknameByGen(i);
		if (i >= block.size()) return "End";
		return block[i].name;
	}
	vector<string> createcode(GenStruct);
	void initblockcodes();
	bool stringisinGen(string,int);  //判断string 是否在index后四元式中存在
	vector<basicBlock> getBasicBlock(){ return block; }
	void clearNotUse(GenStruct);
	void printcodes();
};

void codeTable::initblock()//初始化基本块，设置入口标志和划分基本块
{
	for (int i = 0; i < genStructs.size()-1; i++)
	{
		if (i == 0) {   //程序第一条为入口
			changeOut_port(0);
		}
		if (genStructs[i].code >= 7 && genStructs[i].code <= 13 ||genStructs[i].code >= 23 && genStructs[i].code <= 26) //跳转语句
		{
			changeOut_port(str2int(genStructs[i].result));//跳转到的语句为入口语句
			changeOut_port(i+1); //跳转语句的下一行,为入口语句
		}
	}
	for (int i = 0; i < genStructs.size() ; ) //把入口语句加入到基本块
	{
		basicBlock bl;
		bl.add(genStructs[i]);
		i++;
		for (; i < genStructs.size(); i++)
		{
			if (genStructs[i].out_port == 1)
			{
				block.push_back(bl);
				break;
			}
			else
				bl.add(genStructs[i]);
		}
		if (i==genStructs.size())
			block.push_back(bl);
	}
}

int codeTable::findblocknameByGen(int index)//通过四元式标号寻找其基本块号
{
	for (int i = 0; i < block.size(); i++) //i为block索引
	{
		for (int j = 0; j < block[i].gens.size(); j++)
		{
			if (block[i].gens[j].label == index)
			{
				return i;
			}
		}
	}
	return -1;
}

bool codeTable::stringisinGen(string str,int index)//检查元素在后续是否出现
{
    int f;
    for(int i=index;i<genStructs.size();i++)
    {
        if(genStructs[i].out_port==1 && i==index)
            continue;

        if(genStructs[i].out_port==1 && i!=index)
        {
            f=i-1;
            break;
        }
        if(i==genStructs.size()-1)
        {
            f=i;
            break;
        }
    }
    for(int i=index+1;i<genStructs.size();i++)
    {

        if(i==genStructs.size()-1)
        {
            f=i;
            break;
        }
    }
    if(genStructs[index-1].code>=7 && genStructs[index-1].code<= 13||genStructs[index-1].code >= 23 && genStructs[index-1].code <= 26)
    {
        int a=str2int(genStructs[index].result);
        if(a<=index)
        {
            return true;
        }
    }
	for (int i = f; i >= index; i--)   //因为先删除后操作，所以要把iindex是加进去
	{
		if (genStructs[i].addr1 == str || genStructs[i].addr2 == str)
        {
            return true;
        }

	}
	return false;
}
//遍历所有基本块，把非待用的寄存器里值给删了,把基本块里的Rvalue
void codeTable::clearNotUse(GenStruct gen)//运行到四元式的值
{
	int index = gen.label;
	//遍历所有寄存器的Rvalue 看看后没有在index之后的四元式出现
	for (int i = 0; i < regs.size(); i++)
	{
		for (int j = 0; j < regs[i].Rvalue.size(); j++)
		{
			if (!stringisinGen(regs[i].Rvalue[j], index))
			{
				//删除
				regs[i].Rvalue.erase(regs[i].Rvalue.begin() + j);
			}
		}
	}
}

vector<string> codeTable::createcode(GenStruct gen)//代码翻译
{
	vector<string> codes;
	int code = gen.code;
	clearNotUse(gen);
	registers reg;
	int regindex,temp;
	switch (code)
	{
	case 1://op = "+";
		if (isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("ADD " + findreg(gen.addr1, regindex).name + "," + findreg(gen.addr2, temp).name);
			reg = findreg(gen.addr1, regindex);
			regs[regindex].clearPush(gen.result);
		}
		else if (isExistRvalue(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("ADD " + findreg(gen.addr1, regindex).name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
		}
		else if (!isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("ADD " + reg.name + "," + findreg(gen.addr2, regindex).name);
			reg = findreg(gen.addr1, regindex);
			regs[regindex].clearPush(gen.result);
		}
		else{
			reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("ADD " + reg.name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
		}
		break;
	case 2://op = "-";
		if (isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("SUB " + findreg(gen.addr1, regindex).name + "," + findreg(gen.addr2, temp).name);
			regs[regindex].clearPush(gen.result);
		}
		else if (isExistRvalue(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("SUB " + findreg(gen.addr1, regindex).name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
		}
		else if (!isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			regs[regindex].clearPush(gen.result);
			codes.push_back("SUB " + reg.name + ","+ findreg(gen.addr2, regindex).name);
		}
		else{
			reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("SUB " + reg.name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
		}
		break;
    case 3://op = "*";
		if (regs[0].isinReg(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("MUL " + findreg(gen.addr2, temp).name);
			regs[0].clearPush(gen.result);
		}
		else if (regs[0].isinReg(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("MUL " + gen.addr2);
			regs[0].clearPush(gen.result);
		}
		else if (!regs[0].isinReg(gen.addr1) && isExistRvalue(gen.addr2))
		{

		    if(isExistRvalue(gen.addr1))
                {
                    reg = findreg(gen.addr1, regindex);
			        codes.push_back("MOV AX," +  reg.name);
			        codes.push_back("MUL " + findreg(gen.addr2, regindex).name);
			        regs[0].clearPush(gen.result);
                }
                else
                {
			        codes.push_back("MOV AX," +  gen.addr1);
			        codes.push_back("MUL " + findreg(gen.addr2, regindex).name);
			        regs[0].clearPush(gen.result);
                }
		}
		else{
                if(isExistRvalue(gen.addr1))
                {
                    reg = findreg(gen.addr1, regindex);
			        codes.push_back("MOV AX," +  reg.name);
			        codes.push_back("MUL " + gen.addr2);
			        regs[0].clearPush(gen.result);
                }
                else
                {
			        codes.push_back("MOV AX," +  gen.addr1);
			        codes.push_back("MUL " + gen.addr2);
			        regs[0].clearPush(gen.result);
                }
		}
		break;
	case 4://op = "/";

	    if (regs[0].isinReg(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("DIV " + findreg(gen.addr2, temp).name);
			regs[0].clearPush(gen.result);
		}
		else if (regs[0].isinReg(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("DIV " + gen.addr2);
			regs[0].clearPush(gen.result);
		}
		else if (!regs[0].isinReg(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    if(isExistRvalue(gen.addr1))
                {
                    reg = findreg(gen.addr1, regindex);
			        codes.push_back("MOV AX," +  reg.name);
			        codes.push_back("DIV " + findreg(gen.addr2, regindex).name);
			        regs[0].clearPush(gen.result);
                }
                else
                {
			        codes.push_back("MOV AX," +  gen.addr1);
			        codes.push_back("DIV " + findreg(gen.addr2, regindex).name);
			        regs[0].clearPush(gen.result);
                }
		}
		else{
                if(isExistRvalue(gen.addr1))
                {
                    reg = findreg(gen.addr1, regindex);
			        codes.push_back("MOV AX," +  reg.name);
			        codes.push_back("DIV " + gen.addr2);
			        regs[0].clearPush(gen.result);
                }
                else
                {
			        codes.push_back("MOV AX," +  gen.addr1);
			        codes.push_back("DIV " + gen.addr2);
			        regs[0].clearPush(gen.result);
                }
		}
		break;
    case 5://op = "%";

	    if (regs[0].isinReg(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("DIV " + findreg(gen.addr2, temp).name);
			regs[3].clearPush(gen.result);
		}
		else if (regs[0].isinReg(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("DIV " + gen.addr2);
			regs[3].clearPush(gen.result);
		}
		else if (!regs[0].isinReg(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    if(isExistRvalue(gen.addr1))
                {
                    reg = findreg(gen.addr1, regindex);
			        codes.push_back("MOV AX," +  reg.name);
			        codes.push_back("DIV " + findreg(gen.addr2, regindex).name);
			        regs[3].clearPush(gen.result);
                }
                else
                {
			        codes.push_back("MOV AX," +  gen.addr1);
			        codes.push_back("DIV " + findreg(gen.addr2, regindex).name);
			        regs[3].clearPush(gen.result);
                }
		}
		else{
                if(isExistRvalue(gen.addr1))
                {
                    reg = findreg(gen.addr1, regindex);
			        codes.push_back("MOV AX," +  reg.name);
			        codes.push_back("DIV " + gen.addr2);
			        regs[3].clearPush(gen.result);
                }
                else
                {
			        codes.push_back("MOV AX," +  gen.addr1);
			        codes.push_back("DIV " + gen.addr2);
			        regs[3].clearPush(gen.result);
                }
		}
		break;
	case 6://op = "=";
		reg = findreg(gen.addr1, regindex);
		codes.push_back("MOV " + reg.name + "," + gen.result);
		regs[regindex].Rvalue.push_back(gen.result);
		break;
	case 7://op = "j";
		codes.push_back("JMP " + findblocknameByGen(gen.result));
		break;
	case 8://op = "j<";
	    if (isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + findreg(gen.addr2, temp).name);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JL " + findblocknameByGen(gen.result));
		}
		else if (isExistRvalue(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JL " + findblocknameByGen(gen.result));
		}
		else if (!isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			regs[regindex].clearPush(gen.result);
			codes.push_back("CMP " + reg.name + ","+ findreg(gen.addr2, regindex).name);
			codes.push_back("JL " + findblocknameByGen(gen.result));
		}
		else{
			reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("CMP " + reg.name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JL " + findblocknameByGen(gen.result));
		}
		break;
	case 9://op = "j<=";
	    if (isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + findreg(gen.addr2, temp).name);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JLE " + findblocknameByGen(gen.result));
		}
		else if (isExistRvalue(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JLE " + findblocknameByGen(gen.result));
		}
		else if (!isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			regs[regindex].clearPush(gen.result);
			codes.push_back("CMP " + reg.name + ","+ findreg(gen.addr2, regindex).name);
			codes.push_back("JLE " + findblocknameByGen(gen.result));
		}
		else{
			reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("CMP " + reg.name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JLE " + findblocknameByGen(gen.result));
		}
		break;
	case 10://op = "j!=";
	    if (isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + findreg(gen.addr2, temp).name);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JNE " + findblocknameByGen(gen.result));
		}
		else if (isExistRvalue(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JNE " + findblocknameByGen(gen.result));
		}
		else if (!isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			regs[regindex].clearPush(gen.result);
			codes.push_back("CMP " + reg.name + ","+ findreg(gen.addr2, regindex).name);
			codes.push_back("JNE " + findblocknameByGen(gen.result));
		}
		else{
			reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("CMP " + reg.name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JNE " + findblocknameByGen(gen.result));
		}
		break;
	case 11://op = "j=";
	    if (isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + findreg(gen.addr2, temp).name);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JE " + findblocknameByGen(gen.result));
		}
		else if (isExistRvalue(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JE " + findblocknameByGen(gen.result));
		}
		else if (!isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			regs[regindex].clearPush(gen.result);
			codes.push_back("CMP " + reg.name + ","+ findreg(gen.addr2, regindex).name);
			codes.push_back("JE " + findblocknameByGen(gen.result));
		}
		else{
			reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("CMP " + reg.name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JE " + findblocknameByGen(gen.result));
		}
		break;
	case 12://op = "j>";
	    if (isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + findreg(gen.addr2, temp).name);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JG " + findblocknameByGen(gen.result));
		}
		else if (isExistRvalue(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JG " + findblocknameByGen(gen.result));
		}
		else if (!isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			regs[regindex].clearPush(gen.result);
			codes.push_back("CMP " + reg.name + ","+ findreg(gen.addr2, regindex).name);
			codes.push_back("JG " + findblocknameByGen(gen.result));
		}
		else{
			reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("CMP " + reg.name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JG " + findblocknameByGen(gen.result));
		}
		break;
	case 13://op = "j>=";
	    if (isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + findreg(gen.addr2, temp).name);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JGE " + findblocknameByGen(gen.result));
		}
		else if (isExistRvalue(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("CMP " + findreg(gen.addr1, regindex).name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JGE " + findblocknameByGen(gen.result));
		}
		else if (!isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			regs[regindex].clearPush(gen.result);
			codes.push_back("CMP " + reg.name + ","+ findreg(gen.addr2, regindex).name);
			codes.push_back("JGE " + findblocknameByGen(gen.result));
		}
		else{
			reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("CMP " + reg.name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
			codes.push_back("JGE " + findblocknameByGen(gen.result));
		}
		break;
    case 14://op = "!";
        if(isExistRvalue(gen.addr1))
        {
            reg=findreg(gen.addr1,regindex);
            codes.push_back("NOT "+ reg.name);
            regs[regindex].clearPush(gen.result);
        }
        else
        {
            reg = findreg(gen.addr1, regindex);
            codes.push_back("MOV " + reg.name + "," + gen.addr1);
            codes.push_back("NOT "+ reg.name);
            regs[regindex].clearPush(gen.result);
        }
        break;
    case 15://op ="&&"
            if (isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("AND " + findreg(gen.addr1, regindex).name + "," + findreg(gen.addr2, temp).name);
			reg = findreg(gen.addr1, regindex);
			regs[regindex].clearPush(gen.result);
		}
		else if (isExistRvalue(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("AND " + findreg(gen.addr1, regindex).name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
		}
		else if (!isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("AND " + reg.name + "," + findreg(gen.addr2, regindex).name);
			reg = findreg(gen.addr1, regindex);
			regs[regindex].clearPush(gen.result);
		}
		else{
			reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("AND " + reg.name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
		}
		break;
    case 16://op="||";
        if (isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
			codes.push_back("OR " + findreg(gen.addr1, regindex).name + "," + findreg(gen.addr2, temp).name);
			reg = findreg(gen.addr1, regindex);
			regs[regindex].clearPush(gen.result);
		}
		else if (isExistRvalue(gen.addr1) && !isExistRvalue(gen.addr2))
		{
			codes.push_back("OR " + findreg(gen.addr1, regindex).name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
		}
		else if (!isExistRvalue(gen.addr1) && isExistRvalue(gen.addr2))
		{
		    reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("OR " + reg.name + "," + findreg(gen.addr2, regindex).name);
			reg = findreg(gen.addr1, regindex);
			regs[regindex].clearPush(gen.result);
		}
		else{
			reg = findreg(gen.addr1, regindex);
			codes.push_back("MOV " + reg.name + "," + gen.addr1);
			codes.push_back("OR " + reg.name + "," + gen.addr2);
			regs[regindex].clearPush(gen.result);
		}
		break;
    case 17://op = "==";
        reg =findreg(gen.addr1,regindex);
        codes.push_back("MOV "+reg.name+","+gen.addr1+" EQ "+gen.addr2);
        regs[regindex].clearPush(gen.result);
		break;
    case 18://op = "!=";
        reg =findreg(gen.addr1,regindex);
        codes.push_back("MOV "+reg.name+","+gen.addr1+" NE "+gen.addr2);
        regs[regindex].clearPush(gen.result);
		break;
    case 19://op = "<";
        reg =findreg(gen.addr1,regindex);
        codes.push_back("MOV "+reg.name+","+gen.addr1+" LT "+gen.addr2);
        regs[regindex].clearPush(gen.result);
		break;
    case 20://op = "<=";
        reg =findreg(gen.addr1,regindex);
        codes.push_back("MOV "+reg.name+","+gen.addr1+" LE "+gen.addr2);
        regs[regindex].clearPush(gen.result);
		break;
    case 21://op = ">";
        reg =findreg(gen.addr1,regindex);
        codes.push_back("MOV "+reg.name+","+gen.addr1+" GT "+gen.addr2);
        regs[regindex].clearPush(gen.result);
		break;
    case 22://op = ">=";
        reg =findreg(gen.addr1,regindex);
        codes.push_back("MOV "+reg.name+","+gen.addr1+" GE "+gen.addr2);
        regs[regindex].clearPush(gen.result);
		break;
    case 23://op ="if"
        reg=findreg(gen.addr1,regindex);
        codes.push_back("TEST "+reg.name+","+reg.name);
        codes.push_back("JZ " + findblocknameByGen(gen.result));
    case 24://op ="else"
        codes.push_back("JMP " + findblocknameByGen(gen.result));
    case 25://op = "while"
        reg=findreg(gen.addr1,regindex);
        codes.push_back("TEST "+reg.name+","+reg.name);
        codes.push_back("JZ " + findblocknameByGen(gen.result));
        break;
    case 26://op ="we"
        codes.push_back("JMP "+findblocknameByGen(gen.result));
        break;
	default:
		break;
	}
	return codes;
}



vector<string>cmerge(vector<string> a,vector<string> b)
{
    vector<string>vec;
		//... a,b赋值
		vec.insert(vec.end(),a.begin(),a.end());
		vec.insert(vec.end(),b.begin(),b.end());
    return vec;
}


void codeTable::initblockcodes()
{
	for (int blockindex = 0; blockindex < block.size(); blockindex++)
	{
		for (int i = 0; i < block[blockindex].gens.size(); i++)
		{
			block[blockindex].codes = cmerge(block[blockindex].codes, createcode(block[blockindex].gens[i]));
		}
	}
}
void codeTable::printcodes()
{
	for (int blockindex = 0; blockindex < block.size(); blockindex++)
	{
		cout << block[blockindex].name << ":"<<endl;;
		for (int i = 0; i < block[blockindex].codes.size(); i++)
		{
			cout << "      "<<block[blockindex].codes[i] << endl;
		}
	}
}



int main()
{
    GenStruct gen1;
    gen1.iGenStruct(0,9,"a","b","4");
    genStructs.push_back(gen1);
    gen1.iGenStruct(1,1,"a","b","t1");
    genStructs.push_back(gen1);
    gen1.iGenStruct(2,3,"t1","c","x");
    genStructs.push_back(gen1);
    gen1.iGenStruct(3,7,"-1","-1","7");
    genStructs.push_back(gen1);
    gen1.iGenStruct(4,3,"a","b","t2");
    genStructs.push_back(gen1);
    gen1.iGenStruct(5,2,"5","t2","x");
    genStructs.push_back(gen1);
    gen1.iGenStruct(6,3,"7","x","y");
    genStructs.push_back(gen1);
    gen1.iGenStruct(7,0,"-1","-1","-1");
    genStructs.push_back(gen1);
    codeTable table1;
    table1.initblock();
    table1.initblockcodes();
    table1.printcodes();
    return 0;
}
