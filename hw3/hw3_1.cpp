#include <iostream>
#include "line.h"

using namespace std;

int main(){
	Line t1;
	
	while(t1.get_line()){
		//t1.print_line();
		t1.to_postfix();
		//t1.print_line();
		t1.result();
	}
	
	return 0;
}