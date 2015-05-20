#include <iostream>
#include "binomial_tree.h"

using namespace std;

int main(){
	BinomialHeap<int> a;
	BinomialHeap<int> b(10);
	
	a.insert(10);
	b.merge(a);
	
	int c = b.pop();
	cout << c << endl;
	return 0;
}