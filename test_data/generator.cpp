#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <vector> 
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <string>
#include <time.h>

#define SIZE 5000000
//#define SIZE 10

using namespace std;

char toAlphaNum(long a) {
	if(a<10)
		return '0'+(char)a;
	else if(a<36)
		return 'A'+(char)(a-10);
	else
		return 'a'+(char)(a-36);
}



string itoa(long a) {
	char buffer[100];
	int i=0;
	do {
		buffer[i++] = toAlphaNum(a%62);
		a /= 62;
	}while(a>0);
	string str;
	while(i>0)
		str += buffer[--i];
	return str;
}

string random(long len) {
	char buffer[100];
	int  r,i;
	for(i=0; i<len; i++) {
		r = rand()%62;
		buffer[i]=toAlphaNum(r);
	}
	buffer[i] = 0;
	return string(buffer);
}


int main () {

	srand(time(NULL));
	std::vector<std::string> account;
	std::string tmp;
	for(long i=0; i<SIZE; i++) {
		tmp = random(92)+itoa(i);
		account.push_back(tmp);
	}
	// obtain a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle (account.begin(), account.end(), std::default_random_engine(seed));

	for (std::string& x: account)
		cout << "create " << x << ' ' << "pwd\n" ;
	//std::cout << '\n';
	return 0;
}