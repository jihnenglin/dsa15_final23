#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#ifdef POOL
	#include "pool.h"
#endif
#define in 1
#define out 0

class History{
public:
	bool type;
	std::string id;
	long long int money;
	int time;
	History(bool _type, std::string _id, long long int _money, int _time):type(_type), id(_id), money(_money), time(_time){};
	bool operator<(const History& history2) const{
		return this->time < history2.time;}
	bool operator==(const History& history2) const{
		return this->time == history2.time;}
	bool operator>(const History& history2) const{
		return this->time > history2.time;}
};

class Account{
public:
	std::string id;
	std::string password;
	long long int money;
	std::vector<History*>* history;
	Account(std::string _id, std::string _password):id(_id), password(_password), money(0){
		history = new std::vector<History*>;
	};
	~Account(){delete history;};
	bool operator<(const Account& account2) const{
		if(this->id.compare(account2.id) < 0)
			return true;
		else
			return false;
	}
	bool operator>(const Account& account2) const{
		if(this->id.compare(account2.id) > 0)
			return true;
		else
			return false;
	}
#ifdef POOL
	static MemoryPool *pool;
	static void *operator new(size_t size) { return pool->alloc(size); }
	static void operator delete(void *p)	{ pool->dealloc(p);	}
#endif
};

//Only move the money and history of a2 to a1. The change of history and the deletion of a2 need to be performed by yourself.
void merge(Account* a1, Account* a2){
	a1->money += a2->money;
	std::vector<History*>* new_history = new std::vector<History*>;
	unsigned int i = 0, j = 0;
	while (i < (*a1->history).size() && j < (*a2->history).size()){
		if(*(*a1->history)[i] < *(*a2->history)[j]){
			(*new_history).push_back((*a1->history)[i]);
			i++;
		}
		else if(*(*a1->history)[i] == *(*a2->history)[j]){
			(*new_history).push_back((*a1->history)[i]);
			(*new_history).push_back((*a2->history)[j]);
			i++;
			j++;
		}
		else{
			(*new_history).push_back((*a2->history)[j]);
			j++;
		}
	}
	while(i < (*a1->history).size()){
		(*new_history).push_back((*a1->history)[i]);
		i++;
	}
	while(j < (*a2->history).size()){
		(*new_history).push_back((*a2->history)[j]);
		j++;
	}
	delete a1->history;
	a1->history = new_history;
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

