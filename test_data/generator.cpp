#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <vector> 
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <string>
#include <time.h>

#define SIZE 5000000


using namespace std;

string itoa(long a) {
	char buffer[100];
	int i=0;
	do {
		buffer[i++] = '0'+a%10;
		a /= 10;
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
		if(r<10)
			buffer[i] = '0'+(char)r;
		else {
			r-=26;
			if(r<26)
				buffer[i] = 'A'+(char)r;
			else
				buffer[i] = 'a'+(char)(r-26);
		}
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
		cout << "create" << x << ' ' << "pwd\n" ;
	//std::cout << '\n';
	return 0;
}