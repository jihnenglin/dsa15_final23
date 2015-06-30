#include <iostream>   
#include <algorithm>
#include <vector>  
#include <ctime>    
#include <cstdlib>

#define MAXL  100
#define SIZE 			10000//10000
#define LINE_RAND_CD 	0//100000
#define LINE_TRANSFER 	0//200000
#define LINE_MERGE 		0//10000
#define LINE_FIND       10000
//#define SIZE 10

const std::string find_wildcard("*");

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
	char buffer[MAXL+1];
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
	char buffer[MAXL+1];
	int  r,i;
	for(i=0; i<len; i++) {
		r = rand()%62;
		buffer[i]=toAlphaNum(r);
	}
	buffer[i] = 0;
	return string(buffer);
}


int main () {
	/*create SIZE account*/
	srand(time(NULL));
	std::vector<std::string> account;
	std::string tmp;
	for(long i=0; i<SIZE; i++) {
		//tmp = random(92)+itoa(i);
		tmp = random(rand()%MAXL+1); //has a little chance to collision 
		account.push_back(tmp);
	}
	random_shuffle(account.begin(), account.end());
	for(std::string& x: account)
		cout << "create " << x << ' ' << "pwd\n" ;
	
	/* delete SIZE account 
	random_shuffle(account.begin(), account.end());
	for(long i=0; i<SIZE; i++) {
		cout << "delete " << account.back() << ' ' << "pwd\n" ;
			account.pop_back();
	}*/
	
	
	/*random walk of create and delete
	random_shuffle(account.begin(), account.end());
	for(long i=0; i<LINE_RAND_CD; i++) {
		if(rand()%2 && account.size()>0) {
			swap(account.back(),account[rand()%account.size()]);
			cout << "delete " << account.back() << ' ' << "pwd\n" ;
			account.pop_back();
		}
		else {
			tmp = random(rand()%MAXL+1);
			account.push_back(tmp);
			cout << "create " << account.back() << ' ' << "pwd\n" ;
		}
	}
	*/
	
	/*transfer*/
	int pos;
	int money = 1;
	for(long i=0; i<LINE_TRANSFER; i++) {
		pos = rand()%(account.size()-1);
		cout<< "login " << account.back() << ' ' << "pwd\n";
		cout<< "deposit "<<money <<'\n'; 
		cout<< "transfer "<<account[pos]<<" 1" << "\n";
	}
	/*merge all*/
	
	for(long i=0; i<LINE_MERGE; i++) {
		if(account.size()<2) break;
		pos = rand()%(account.size()-1);
		if(account.back().compare(account[pos])==0) {
			swap(account[pos],account[account.size()-1]);
			account.pop_back();
			i--;continue;
		}
		cout<< "merge "<< account[pos] << " pwd "<<account.back()<<" pwd\n";
		account.pop_back();
	}
	
	/*find*/
	cout<< "login " << account.front() << ' ' << "pwd\n";
	if(find_wildcard.size()==0) {
		for(long i=0; i<LINE_FIND; i++) {
			bool star_able = true;
			tmp = random(rand()%101);
			//cout<<"origin "<<tmp<<"\n";
			for(unsigned int j=0; j<tmp.size();j++) {
				if(rand()%2) {
					if(star_able&&rand()%2){
						tmp.insert(j,"*");
						j++;//position+1
					}
					else
						tmp[j] = '?';
					star_able = false;
				}
				else {
					star_able = true;				
				}		
			}
			if(tmp.size()==0||(star_able&&rand()%2))
				tmp += '*';
			cout<<"find "<<tmp<<"\n";	
		}
	}else {
		for(long i=0; i<LINE_FIND; i++)
			cout<<"find "<<find_wildcard<<"\n";	
	}
	
	
	return 0;
}