#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "pool.h"

#define in 1
#define out 0


class Transfer{
public:
	const std::string *from;
	const std::string *to;
	long long int money;
	int time;
	
	Transfer(const std::string& _from, const std::string& _to, long long int _money, int _time):
	from(&_from),to(&_to),money(_money),time(_time) {};	
};

class History{
public:
	bool type;//int:1 out:0
	Transfer *transfer;
	
	History(bool _type=false, Transfer* _transfer=NULL): type(_type),transfer(_transfer) {}

	bool operator<(const History& history2) const {
		return this->transfer->time < history2.transfer->time;}
};

class Account{
public:
	const std::string &id;
	std::string password;
	long long int money;
	std::vector<History> history;
	
	Account(const std::string &_id, std::string _password):id(_id), password(_password), money(0){
	};
	~Account() {};
	
	Transfer* transfer(Account *that,long long int _money, int _time);
	void         merge(Account *a2);
	void	    search(const std::string& id);
	
	bool operator<(const Account& account2) const{
		return this->id.compare(account2.id)<0;
	}
	bool operator>(const Account& account2) const{
		return this->id.compare(account2.id)>0;}
	static MemoryPool *pool;
	static void *operator new(size_t size){ return pool->alloc(size); }
	static void operator delete(void *p){ pool->dealloc(p);	}
};	

Transfer* Account::transfer(Account *that, long long int _money, int _time) {
	this->money -= _money;
	that->money += _money;	
	Transfer* log = new Transfer(this->id,that->id,_money,_time);
	this->history.push_back(History(out,log));
	that->history.push_back(History(in, log));
	return log;
}

void Account::merge(Account *a2) {
	this->money += a2->money;
	
	int i,j;
	for(j=0; j<int(a2->history.size()); j++) {
		if(a2->history[j].type==in) // id -> a2
			a2->history[j].transfer->to   = &(this->id);
		else // a2 -> id
			a2->history[j].transfer->from = &(this->id);
	}	
	
	i=this->history.size()-1, j=a2->history.size()-1;
	int k=i+j+1;
	this->history.resize(k+1);

	while(i>=0 && j>=0) {
		if( this->history[i] < a2->history[j] )
			this->history[k--] = a2->history[j--];
		else if( a2->history[j] < this->history[i] )
			this->history[k--] = this->history[i--];
		else {
			this->history[k--] = a2->history[j--];
			this->history[k--] = this->history[i--];	
		}	
	}
	while(i>=0)
		this->history[k--] = this->history[i--];
	while(j>=0)
		this->history[k--] = a2->history[j--];
	
	a2->history.resize(0);
}

void Account::search(const std::string& id) {
	bool record = false;
	Transfer *ptr;
	for(unsigned int i=0; i<history.size(); i++) {
		if(history[i].type==in) {
			ptr = history[i].transfer;
			if( ptr->from->compare(id)==0 ) {
				std::cout << "From " << *(ptr->from) << " " << ptr->money <<'\n';
				record = true;
			}
		}
		else {
			ptr = history[i].transfer;
			if( ptr->to->compare(id)==0 ) {
				std::cout << "To " << *(ptr->to) << " " << ptr->money <<'\n';
				record = true;
			}
		}
	}
	if(!record)
		std::cout << "no record" <<'\n';
}

int score(const std::string &u, const std::string &v){
	int L = std::min(u.size(), v.size());
	int dL = abs(int(u.size()) - int(v.size()));
	int s = 0;
	for(int i = 1; i <= dL; i++)
		s += i;
	for(int i = 0; i <= L - 1; i++)
	        s += (u[i] != v[i])? (L - i) : 0;
	return s;
}

