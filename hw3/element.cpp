#include "element.h"

Element::Element(const Element& b){
		type = b.type;
		token.clear();
		token = b.token;
}

bool Element::isTwoCharOptr(char* a, int i){
	if(a[i] == '<'){ 
		type = BL;
		return true;
	}else if(a[i] == '>'){
		type = BR;
		return true;
	}else if(a[i] == '&' && a[i + 1] == '&'){
		type = LA;
		return true;
	}else if(a[i] == '|' && a[i + 1] == '|'){
		type = LO;
		return true;
	}
	return false;
}

int Element::OneCharOptr_set_type(char* a, int i){
	map<char, int> chart;
	chart['*'] = MUL; chart['/'] = DIV; chart['%'] = MOD;
	chart['&'] = BA; chart['^'] = BE; chart['|'] = BO;
	chart['('] = PL; chart[')'] = PR; chart['~'] = BN;
	chart['!'] = NOT;
	
	return chart[a[i]];
}

int Element::set_element(char *a, int i){
	if(isdigit(a[i])){   //input digit
		type = NUM;
		token.push_back(a[i]);
		i++;
		while(isdigit(a[i])){
			token.push_back(a[i]);
			i++;
		}
		return i;
	}else if(isblank(a[i])){    //is a space
		// do not thing
		return i + 1;
	}else if(isTwoCharOptr(a, i)){  //input two character operator
		token.push_back(a[i]);
		token.push_back(a[i + 1]);
		return i + 2;
	}else{  //input one char operator
		token.push_back(a[i]);
		type = OneCharOptr_set_type(a, i);
		return i + 1;
	}
}

void Element::print_token(){
	for(int i = 0; i < (int)token.size(); i++){
		cout << token[i];
	}
}

int Element::get_type(){
	return type;
}

int Element::get_precedence(){
	int chart[19] = {0, 3, 3, 3, 4, 4, 6, 7, 8, 5, 5, 11, 1,
	2, 2, 2, 10, 9, 2};
	return chart[type]; 
}

int Element::get_number(){
	if(type != NUM){
		cout << "input not number" << endl;
		return 0;
	}
	
	int num = 0;
	for(int i = 0; i < (int)token.size(); i++){
		num += (token[i] - '0') * pow(10, token.size() - 1 - i);
	}
	
	return num;
}

void Element::set_type(int typ){
	type = typ;
}

void Element::set_token(char* a, int start, int len){
	token.clear();
	for(int i = 0; i < len; i++){
		token.push_back(a[start + i]);
	}
}