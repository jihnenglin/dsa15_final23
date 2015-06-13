#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#define in 1
#define out 0

class History{
private:
	bool type;
	std::string id;
public:
	History(bool _type, std::string _id):type(_type), id(_id){};
};

class Account{
public:
	std::string id;
	std::string password;
	long long int money;
	std::vector<History>* history;
	Account(std::string _id, std::string _password):id(_id), password(_password), money(0){
		history = new std::vector<History>;
	};
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
};

int score(std::string &u, std::string &v){
	int L = std::min(u.size(), v.size());
	int dL = abs(int(u.size()) - int(v.size()));
	int s = 0;
	for(int i = 1; i <= dL; i++)
		s += i;
	for(int i = 0; i <= L - 1; i++)
	        s += (u[i] != v[i])? (L - i) : 0;
	return s;
}

