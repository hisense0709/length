// readFile_calcuExpress.cpp : 
// 本文件用c++语言编写，在visual studio 2008中编译通过

#include "StdAfx.h"
#include <stdlib.h>
#include <fstream>
#include<string>

#include <stdio.h>
#include <iostream>

#include <map>
using namespace std;


// 在字符串里找到从左边数第一个表示长度单位的字符串（如miles)
string find1stLenUnitStr(const char* s) {
  const char *pStart = 0;
	const char *stop = s + strlen(s);
	while(*s) {
		if (*s > 'z' || *s < 'a')
			s++;
		else {
			pStart = s;
			break;
		}
	}
	while(*s) {
		if (*s <= 'z' && *s >= 'a')
			s++;
		else {
			stop = s;
			break;
		}
	}

	string tmp(pStart, stop - pStart);
	return tmp;

}


// eval - 计算字符串表达式的值
//        表达式只由+-2种运算符及正数组成，没有括号，没有空格
// @param s - 表达式字符串的起始位置
// @param e - 表达式字符串的结束位置（不算在表达式内）
// @ param map<string, double> mapRules - 存储各种长度单位和米的转换关系
double eval(const char *s, const char *e, map<string, double> mapRules)
{
    const char *p;
	
    // 计算+-
    for (p = e - 1; p > s; --p) {
        switch (*p) {
        case '+':
            return eval(s, p, mapRules) + eval(p + 1, e, mapRules);
        case '-':
            return eval(s, p, mapRules) - eval(p + 1, e, mapRules);
        default:
            break;
        }
    }
	
    	
    // 计算数值部分
	double res = atof(s);
	string unit = find1stLenUnitStr(s);
	//string unit = find1stLenUnitStr(p);
	if (unit == "feet")
		res *= mapRules["foot"];
	else if (unit == "inches")
		res *= mapRules["inch"];
	else {
		char c = unit.at(unit.length() - 1);
		string singUnit;
		if ( c == 's') {
			// 去掉复数形式最后的"s"
			singUnit = unit.substr(0, unit.length() - 1);
			res *= mapRules[singUnit];
		}
		else
			res *= mapRules[unit];
	}
    return res;
}



int main()
{
	// 把标准输出重定向到output.txt文件中
	FILE * stream = freopen("output.txt","w",stdout); 
	if( stream == NULL ) {
		printf( "error on create output.txt file\n" );
		system("pause");
		return -1;
	}
	else {
		printf("winf0709@gmail.com\n");
		printf("\n");
	}
	string line;
	ifstream readf("input.txt",  ios::binary);
	 
	if (readf.is_open()){
		map<string, double> mapRules;
		while (!readf.eof()) {
			getline(readf, line,'\n');
			string::size_type pos = 0;
			pos = line.find("=");
			if (string::npos != pos) {// 包含“=”的行，即定义转换规则的行
				string unit = find1stLenUnitStr(line.c_str());
				string strNum(line.c_str() + pos + 1); 
				mapRules[unit] = atof(strNum.c_str());
			}
			else if (line == "\r")
				;
			else if (line != "\r"){// 不包含“=”的行，即需要计算的行	
				int size = line.size();
				if (size > 0) {
					double res = eval(line.c_str(), line.c_str() + line.length() -1,mapRules);
					printf("%.2f m\n", res);
				}
			
			}//else if (line != "\r")
		}// while (
		readf.close();
		fclose(stream);
		freopen("CON","w",stdout);
		cout << "Finished calculating the results and write to output.txt, the output content is :" << endl;
		system("cat output.txt");
	}// if (readf.is_open()){
	else {
		freopen("CON","w",stdout);
		fclose(stream);
		cout << "CAN NOT open input.txt file,pls check it if exist!" << endl;
	}
	

	system("pause");
    return 0;
}
