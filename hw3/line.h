#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <vector>
#include <stack>
#include <map>
#include <math.h>
#include "element.h"

#define MAXL 1000000
using namespace std;

class Line{
public:
	Line(){};
	Line(const Line&);
	~Line(){};
	
	bool get_line();
	void print_line();
	void to_postfix();
	void result();

private:
	vector<Element> a;
	int set_line(char*, int);
	int set_line_minus(char*, int);
	int set_line_plus(char*, int);
	bool check_precedence(Element, Element);
	int unary_operation(int, int);
	int binary_operation(int, int, int);
	void print_status(vector<Element>&, stack<Element>);
};

#endif