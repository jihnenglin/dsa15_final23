#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <math.h>
#include <iostream>
#include <map>
enum operators{NUM, MUL, DIV, MOD, ADD, SUB, BA, BE,
				BO, BL, BR, PL, PR, UM, UP, BN, LA, 
				LO, NOT};

using namespace std;

class Element{
public:
	Element(){};
	Element(const Element&);
	~Element(){};
	
	int set_element(char *, int);
	void print_token();
	int get_type();
	int get_precedence();
	int get_number();
	void set_type(int);
	void set_token(char*, int, int);
	
private:
	vector<char> token;
	int type;
	
	bool isTwoCharOptr(char*, int);
	int OneCharOptr_set_type(char*, int);
	
};

#endif