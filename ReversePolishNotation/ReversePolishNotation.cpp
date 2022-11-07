#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <deque>
using namespace std;

//生成的逆波兰表达式
string RPN;
//计算的结果
double res;

//运算符集合 (检查非法符号) 
char operators[] = { '+','-','*','/','(',')' };
//数值列表 (用于在生成表达式的同时完成计算)
deque<double> digitDeque;
//运算符栈
stack<char> operatorStack;

/*初始化全局变量，便于重复使用*/
void initGlobalVariables() {
	RPN = "";
	res = 0;
	digitDeque.clear();
	stack<char>().swap(operatorStack);
}

/*处理数字，返回值表示是否正常处理*/
bool handleDigit(string& str, int& i, int& len) {
	string s = "";
	//小数点出现的次数
	int dotCnt = 0;
	while (i < len && (isdigit(str[i]) || str[i] == '.')) {
		if (str[i] == '.') {
			++dotCnt;
		}
		s += str[i++];
	}
	if (dotCnt > 1) {
		RPN = "算数表达式异常，数字中出现两个小数点";
		return false;
	}
	RPN += s + " ";
	//利用stringstream流将string转double
	stringstream stream(s);
	double d;
	stream >> d;
	digitDeque.push_back(d);
	return true;
}

/*判断是否为计算表达式运算符*/
bool isOperators(char c) {
	for (char o : operators) {
		if (c == o) {
			return true;
		}
	}
	RPN = "出现非计算表达式运算符";
	return false;
}

/*获取运算符的优先级*/
int getPriority(char c) {
	switch (c)
	{
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	default:
		return 3;
	}
}

/*运算,输入当前要退栈的运算符*/
void calculate(char& c) {
	//取队尾两个数字进行运算 (注意位置，后取出的数字在表达式中更靠前)
	double b = digitDeque.at(digitDeque.size()-1);
	digitDeque.pop_back();
	double a = digitDeque.at(digitDeque.size() - 1);
	digitDeque.pop_back();
	//根据运算符得出结果
	double ans;
	switch (c)
	{
	case '+':
		ans = a + b;
		break;
	case '-':
		ans = a - b;
		break;
	case '*':
		ans = a * b;
		break;
	case '/':
		ans = a / b;
	default:
		break;
	}
	//将结果放在队尾
	digitDeque.push_back(ans);
}

/*运算符处理*/
bool handleOperator(string& str, int& i, char& c) {
	if (!isOperators(c)) {
		return false;
	}
	//栈空直接入栈
	if (operatorStack.empty()) {
		operatorStack.push(c);
	}
	//右括号一直找到左括号
	else if (c == ')') {
		while (!operatorStack.empty() && operatorStack.top() != '(') {
			//输出退栈
			char op = operatorStack.top();
			RPN += op;
			RPN += " ";
			operatorStack.pop();
			//运算
			calculate(op);
		}
		//为空说明没找到左括号
		if (operatorStack.empty()) {
			RPN = "括号无法配对";
			return false;
		}
		else {
			operatorStack.pop();
		}
	}
	//按优先级处理
	else {
		while (!operatorStack.empty() && getPriority(c) <= getPriority(operatorStack.top())) {
			//栈首左括号直接入栈
			if (operatorStack.top() == '(') {
				break;
			}
			//当前值优先级小，栈首退栈输出
			char op = operatorStack.top();
			RPN += op;
			RPN += " ";
			operatorStack.pop();
			//运算
			calculate(op);
		}
		//当前值较大或栈已为空入栈
		operatorStack.push(c);
	}
	++i;
}


/*生成逆波兰*/
bool RPNotation(string str) {
	//初始化全局变量
	initGlobalVariables();
	int len = str.size();
	//遍历
	for (int i = 0; i < len; ) {
		char c = str[i];
		if (c == ' ') {
			++i;
			continue;
		}
		//数字(包括小数)
		else if (isdigit(c)) {
			if (!handleDigit(str, i, len)) {
				//如果发现问题，立刻返回
				return false;
			}
		}
		//运算符
		else {
			if (!handleOperator(str, i, c)) {
				return false;
			}
		}
	}
	//遍历结束后，所有符号输出退栈
	while (!operatorStack.empty()) {
		if (operatorStack.top() == '(') {
			RPN = "括号未匹配";
			return false;
		}
		char op = operatorStack.top();
		RPN += op;
		RPN += " ";
		operatorStack.pop();
		calculate(op);
	}
	res = digitDeque.at(0);
	return true;
}

void main() {
	while (true) {
		//提示输入算术表达式
		cout << "输入算术表达式: ";
		string str;
		getline(cin, str);
		//生成逆波兰表达式
		if (RPNotation(str)) {
			cout << "逆波兰表达式: " << RPN << endl;
			cout << "计 算  结 果: " << res << endl;
		}
		else {
			cout << RPN << endl;
		}
		cout << endl;
	}
}