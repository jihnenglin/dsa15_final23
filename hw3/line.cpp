#include "line.h"

Line::Line(const Line &line2){
	a.clear();
	a = line2.a;
}

int Line::set_line(char*temp, int i){
	Element e;//temp element
	i = e.set_element(temp, i);
	a.push_back(e);
	return i;
}

int Line::set_line_minus(char*temp, int i){
	Element e;//temp element
	if(a.empty() || (a.back().get_type() != NUM && a.back().get_type() != PR)){
		e.set_type(UM);
		e.set_token(temp, i, 1);
	}else{
		e.set_type(SUB);
		e.set_token(temp, i, 1);
	}
	a.push_back(e);
	return i + 1;
}

int Line::set_line_plus(char*temp, int i){
	Element e;//temp element
	if(a.empty() || (a.back().get_type() != NUM && a.back().get_type() != PR)){
		e.set_type(UP);
		e.set_token(temp, i, 1);
	}else{
		e.set_type(ADD);
		e.set_token(temp, i, 1);
	}
	a.push_back(e);
	return i + 1;
}

bool Line::get_line(){
	char *temp = new char[MAXL];//to store input
	a.clear();//clean the vector
	int i = 0;//index
	
	if(fgets(temp, MAXL, stdin) != NULL){
		while(temp[i] != '\n'){
			//line function to set line
			if(isblank(temp[i])){
				i++;
			}else if(temp[i] == '-'){
				i = set_line_minus(temp, i);
			}else if(temp[i] == '+'){
				i = set_line_plus(temp, i);
			}else{
				i = set_line(temp, i);
			}
		}
		delete [] temp;
		return true; //continue 
	}else{
		delete [] temp;
		return false; //EOF	
	}
}

void Line::print_line(){
	//cout << "Number of element : " << a.size() << endl;
	for(int i = 0; i < (int)a.size(); i++){
		cout << ' ';
		a[i].print_token();
		//cout << ',';
	}
	cout << endl;
}

bool Line::check_precedence(Element a, Element b){
	//cout << a.get_precedence() << " " << b.get_precedence() << endl;
	return a.get_precedence() > b.get_precedence();
}

void Line::print_status(vector<Element> &a, stack<Element> s){
	cout << '\t' << "current output:";
	for(int i = 0; i < (int)a.size(); i++){
		cout << ' '; a[i].print_token(); cout << ',';
	}
	cout << endl;
	
	vector<Element> temp;
	while(!s.empty()){
		temp.push_back(s.top());
		s.pop();
	}
	
	cout << '\t' << "current stack:";
	for(int i = 0; i < (int)temp.size(); i++){
		cout << ' '; 
		temp[(temp.size() - 1 - i)].print_token();
		cout << ',';
	}
	cout << endl;
}

void Line::to_postfix(){
	stack<Element> s;
	vector<Element> post;
	cout << "# transform from indix to postfix" << endl;
	for(int i = 0; i < (int)a.size(); i++){
		cout << "encounter "; a[i].print_token(); cout << ": ";
		//check encounter is 
		int type = a[i].get_type();
		if(type == NUM){
			//number then push to post
			post.push_back(a[i]);
			cout << "push to output\n";
		}else{
			//sign push to stack
			if(type == PR){
				while(s.top().get_type() != PL){
					post.push_back(s.top());
					s.pop();
				}
				s.pop();
				cout << "flush the stack to output until meeting '('\n";
			}else if(type == PL){
				s.push(a[i]);
				cout << "push to stack\n";
			}else if(s.empty() || check_precedence(s.top(), a[i])){
				s.push(a[i]);
				cout << "push to stack\n";
			}else{
				while(!s.empty() && !check_precedence(s.top(), a[i])){
					post.push_back(s.top());
					s.pop();
				}
				s.push(a[i]);
				cout << "push to stack\n";
				cout << "\t*** stack.top() has greater precedence ***\n";
			}
		}
		print_status(post, s);
	}
	
	while(!s.empty()){
		post.push_back(s.top());
		s.pop();
	}
	cout << "encounter NOTHING: flush the stack to output\n";
	print_status(post, s);
	
	cout << "# postfix expression transforming complete" << endl;
	a = post;
	cout << "Postfix Exp:";
	print_line();
}

int Line::unary_operation(int type, int num){
	switch(type){
		case UM:
			return -num;
		case UP:
			return num;
		case BN:
			return ~num;
		case NOT:
			return !num;
	}
	cout << "error in Uoper\n";
	return 0;
}

int Line::binary_operation(int type, int a, int b){
	switch(type){
		case MUL:
			return a * b;
		case DIV:
			return a / b;
		case MOD:
			return a % b;
		case ADD:
			return a + b;
		case SUB:
			return a - b;
		case BA:
			return a & b;
		case BE:
			return a ^ b;
		case BO:
			return a | b;
		case BL:
			return a << b;
		case BR:
			return a >> b;
		case LO:
			return a || b;
		case LA:
			return a && b;
	}
	cout << "error in Boptr\n";
	return 0;
}

void Line::result(){
	stack<int> temp;//to put number
	for(int i = 0; i < (int)a.size(); i++){
		int type = a[i].get_type();
		//cout << type << endl;
		if(type == NUM){
			temp.push(a[i].get_number());
		}else{
			if(type == UM || type == UP
				|| type == BN || type == NOT){
				int num = temp.top(); temp.pop();
				temp.push(unary_operation(type, num));
			}else{
				int y = temp.top(); temp.pop();
				int x = temp.top(); temp.pop();
				temp.push(binary_operation(type, x, y));
			}
		}
	}
	if(temp.size() == 1){
		printf("RESULT: %d\n", temp.top()); 
	}else{
		cout << "error in result\n";
	}
}