#include "element2.h"

Element::Element(const Element& b){
		type = b.type;
		token.clear();
		token = b.token;
}

int Element::isCharOptr(char* a, int i){
	map<char, int> chart;
	chart['s'] = SIN; chart['c'] = COS; chart['e'] = EXP;
	chart['l'] = LOG; chart['p'] = POW;
	
	if((a[i] == 's' && a[i + 1] == 'i') || a[i] == 'c' || a[i] == 'e' ||
		a[i] == 'l' || a[i] == 'p'){ 
		type = chart[a[i]];
		token.push_back(a[i]);
		token.push_back(a[i + 1]);
		token.push_back(a[i + 2]);
		return i + 3;
	}else if(a[i] == 's'){
		type = SQRT;
		token.push_back(a[i]);
		token.push_back(a[i + 1]);
		token.push_back(a[i + 2]);
		token.push_back(a[i + 3]);
		return i + 4;
	}else if(a[i] == 'f'){
		type = FABS;
		token.push_back(a[i]);
		token.push_back(a[i + 1]);
		token.push_back(a[i + 2]);
		token.push_back(a[i + 3]);
		return i + 4;
	}
	cout << "error in isCharOptr!!!\n";
	return false;
}

int Element::OneCharOptr_set_type(char* a, int i){
	map<char, int> chart;
	chart['*'] = MUL; chart['/'] = DIV; 
	chart['('] = PL; chart[')'] = PR; 
	
	return chart[a[i]];
}

int Element::set_element(char *a, int i){
	if(isdigit(a[i])){   //input digit
		type = NUM;
		token.push_back(a[i]);
		i++;
		while(isdigit(a[i]) || a[i] == '.'){
			token.push_back(a[i]);
			i++;
		}
		return i;
	}else if(isblank(a[i])){    //is a space
		// do not thing
		return i + 1;
	}else if(isalpha(a[i])){  //input two character operator
		return isCharOptr(a, i);
	}else{  //input one char operator
		token.push_back(a[i]);
		type = OneCharOptr_set_type(a, i);
		return i + 1;
	}
}

void Element::print_token(){
	if(type == NUM){
		double num = get_number();
		printf("%lf", num);
	}else{
		for(int i = 0; i < (int)token.size(); i++){
			cout << token[i];
		}
	}
}

int Element::get_type(){
	return type;
}

int Element::get_precedence(){
	int chart[16] = {0, 3, 3, 4, 4, 5, 5, 2, 2, 5,5,5,5,5,5,5};
	return chart[type]; 
}

double Element::get_number(){
	if(type != NUM){
		cout << "input not number" << endl;
		return 0;
	}
	
	double num = 0;
	int n = 0;
	while(n < (int)token.size() && token[n] != '.'){
		n++;
	}
	
	int i, j;
	if(n == (int)token.size()){
		for(i = 0; i < (int)token.size(); i++){
			num += ((token[i] - '0') * pow(10, n - 1 - i));
		}
	}else{
		for(i = 0; i < n; i++){
			num += ((token[i] - '0') * pow(10, n - 1 - i));
		}
		for(j = n + 1; j < (int)token.size(); j++){
			num += ((token[j] - '0') * pow(10, n - j));
		}
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