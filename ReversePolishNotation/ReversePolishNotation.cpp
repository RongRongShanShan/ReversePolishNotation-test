#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <deque>
using namespace std;

//���ɵ��沨�����ʽ
string RPN;
//����Ľ��
double res;

//��������� (���Ƿ�����) 
char operators[] = { '+','-','*','/','(',')' };
//��ֵ�б� (���������ɱ��ʽ��ͬʱ��ɼ���)
deque<double> digitDeque;
//�����ջ
stack<char> operatorStack;

/*��ʼ��ȫ�ֱ����������ظ�ʹ��*/
void initGlobalVariables() {
	RPN = "";
	res = 0;
	digitDeque.clear();
	stack<char>().swap(operatorStack);
}

/*�������֣�����ֵ��ʾ�Ƿ���������*/
bool handleDigit(string& str, int& i, int& len) {
	string s = "";
	//С������ֵĴ���
	int dotCnt = 0;
	while (i < len && (isdigit(str[i]) || str[i] == '.')) {
		if (str[i] == '.') {
			++dotCnt;
		}
		s += str[i++];
	}
	if (dotCnt > 1) {
		RPN = "�������ʽ�쳣�������г�������С����";
		return false;
	}
	RPN += s + " ";
	//����stringstream����stringתdouble
	stringstream stream(s);
	double d;
	stream >> d;
	digitDeque.push_back(d);
	return true;
}

/*�ж��Ƿ�Ϊ������ʽ�����*/
bool isOperators(char c) {
	for (char o : operators) {
		if (c == o) {
			return true;
		}
	}
	RPN = "���ַǼ�����ʽ�����";
	return false;
}

/*��ȡ����������ȼ�*/
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

/*����,���뵱ǰҪ��ջ�������*/
void calculate(char& c) {
	//ȡ��β�������ֽ������� (ע��λ�ã���ȡ���������ڱ��ʽ�и���ǰ)
	double b = digitDeque.at(digitDeque.size()-1);
	digitDeque.pop_back();
	double a = digitDeque.at(digitDeque.size() - 1);
	digitDeque.pop_back();
	//����������ó����
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
	//��������ڶ�β
	digitDeque.push_back(ans);
}

/*���������*/
bool handleOperator(string& str, int& i, char& c) {
	if (!isOperators(c)) {
		return false;
	}
	//ջ��ֱ����ջ
	if (operatorStack.empty()) {
		operatorStack.push(c);
	}
	//������һֱ�ҵ�������
	else if (c == ')') {
		while (!operatorStack.empty() && operatorStack.top() != '(') {
			//�����ջ
			char op = operatorStack.top();
			RPN += op;
			RPN += " ";
			operatorStack.pop();
			//����
			calculate(op);
		}
		//Ϊ��˵��û�ҵ�������
		if (operatorStack.empty()) {
			RPN = "�����޷����";
			return false;
		}
		else {
			operatorStack.pop();
		}
	}
	//�����ȼ�����
	else {
		while (!operatorStack.empty() && getPriority(c) <= getPriority(operatorStack.top())) {
			//ջ��������ֱ����ջ
			if (operatorStack.top() == '(') {
				break;
			}
			//��ǰֵ���ȼ�С��ջ����ջ���
			char op = operatorStack.top();
			RPN += op;
			RPN += " ";
			operatorStack.pop();
			//����
			calculate(op);
		}
		//��ǰֵ�ϴ��ջ��Ϊ����ջ
		operatorStack.push(c);
	}
	++i;
}


/*�����沨��*/
bool RPNotation(string str) {
	//��ʼ��ȫ�ֱ���
	initGlobalVariables();
	int len = str.size();
	//����
	for (int i = 0; i < len; ) {
		char c = str[i];
		if (c == ' ') {
			++i;
			continue;
		}
		//����(����С��)
		else if (isdigit(c)) {
			if (!handleDigit(str, i, len)) {
				//����������⣬���̷���
				return false;
			}
		}
		//�����
		else {
			if (!handleOperator(str, i, c)) {
				return false;
			}
		}
	}
	//�������������з��������ջ
	while (!operatorStack.empty()) {
		if (operatorStack.top() == '(') {
			RPN = "����δƥ��";
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
		//��ʾ�����������ʽ
		cout << "�����������ʽ: ";
		string str;
		getline(cin, str);
		//�����沨�����ʽ
		if (RPNotation(str)) {
			cout << "�沨�����ʽ: " << RPN << endl;
			cout << "�� ��  �� ��: " << res << endl;
		}
		else {
			cout << RPN << endl;
		}
		cout << endl;
	}
}