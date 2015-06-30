#include <iostream>
#include <string.h>
#include "sharedlog.h"
#include "recommend.h"
#include <unordered_map>

#include "md5.h"

#define MAXL 105

using namespace std;
#ifdef POOL
MemoryPool* Account::pool = new MemoryPool(sizeof(Account),5000);
//char membuffer[sizeof(Account)*100000];
//MemoryPool* Account::pool = new MemoryPool(sizeof(Account),sizeof(membuffer),(void*)membuffer);
#endif
typedef std::unordered_map<std::string,Account*> Accmap;
Accmap accmap;

bool find(const string& id) {
	return accmap.find(id)==accmap.end();
}

void inorder_recommend(const Accmap &accmap, Rank& r, const string& origin){
	for(Accmap::const_iterator it = accmap.begin(); it!=accmap.end(); ++it)
		r.update(it->first,score(it->first,origin));
}

void inorder_wild(Account *current, vector<Account *>* v, const string& wild){
	for(Accmap::const_iterator it = accmap.begin(); it!=accmap.end(); ++it) {
		if(match_wild(it->first, wild)&&it->second!=current)
			v->push_back(it->second);
	}
}

bool accptrcmp(Account *a1,Account *a2) {
	return *a1 < *a2;
}


int main(){
	char request[MAXL],buf[4][MAXL]; 
	string id1,id2,p,p2; // input id and password
	long long int money; // input money
	vector<Transfer*> transfer_log; //all transfer log
	
	Account *current = NULL;
	int time = 0;
	
	Accmap::const_iterator it1,it2;
	accmap.max_load_factor(0.5);
	accmap.reserve(5003);

	while(scanf("%s", request) != EOF){
		if(strcmp(request, "login") == 0){
			scanf("%s%s",buf[0],buf[1]);
			id1 = string(buf[0]);
			p   = string(buf[1]);
			
			it1 = accmap.find(id1);
			if(it1 == accmap.end())
				cout << "ID " << id1 << " not found" << endl;
			else if(it1->second->password.compare(md5(p)) != 0)
				cout << "wrong password" << endl;
			else{
				current = it1->second;
				cout << "success" << endl;
			}
		}else if(strcmp(request, "create") == 0){
			scanf("%s%s",buf[0],buf[1]);
			id1 = string(buf[0]);
			p   = string(buf[1]);
			
			it1 = accmap.find(id1);
			if(it1 == accmap.end()){
				Account* account = new Account(*(new string(id1)), md5(p));
				accmap.insert({id1,account});
				cout << "success" << endl;
			}
			else{
				cout << "ID " << id1 << " exists, ";
				//recommend 10 unused IDs
				Rank rank;
				Recommend recommend(find);
				recommend.getRank(rank, id1);
				rank.print();
			}

		}else if(strcmp(request, "delete") == 0){			
			scanf("%s%s",buf[0],buf[1]);
			id1 = string(buf[0]);
			p   = string(buf[1]);
			
			it1 = accmap.find(id1);
			if(it1 == accmap.end())
				cout << "ID " << id1 << " not found" << endl;
			else if(it1->second->password.compare(md5(p)) != 0)
				cout << "wrong password" << endl;
			else{
				delete it1->second;
				accmap.erase(it1);
				cout << "success" << endl;
			}
		}else if(strcmp(request, "merge") == 0){
			scanf("%s%s%s%s",buf[0],buf[1],buf[2],buf[3]);
			id1 = string(buf[0]);
			p   = string(buf[1]);
			id2 = string(buf[2]);
			p2  = string(buf[3]);

			it1 = accmap.find(id1);
			it2 = accmap.find(id2);
			if(it1 == accmap.end())
				cout << "ID " << id1 << " not found" << endl;
			else if(it2 == accmap.end())
				cout << "ID " << id2 << " not found" << endl;
			else {
				Account* account1 = it1->second;
				Account* account2 = it2->second;
				if(account1->password.compare(md5(p)) != 0)
					cout << "wrong password1" << endl;
				else if(account2->password.compare(md5(p2)) != 0)
					cout << "wrong password2" << endl;
				else{
					if(account1!=account2) {
						account1->merge(account2);
						delete account2;
						accmap.erase(it2);
					}
					cout << "success, " << account1->id << " has " << account1->money << " dollars" << endl;
				}
			}
		}else if(strcmp(request, "deposit") == 0){
			scanf("%lld",&money);
			current->money += money;
			cout << "success, " << current->money << " dollars in current account" << endl;
		}else if(strcmp(request, "withdraw") == 0){
			scanf("%lld",&money);
			if(money > current->money)
				cout << "fail, " << current->money << " dollars only in current account" << endl;
			else{
				current->money -= money;
				cout << "success, " << current->money << " dollars left in current account" << endl;
			}
		}else if(strcmp(request, "transfer") == 0){
			scanf("%s%lld",buf[0],&money);
			id1 = string(buf[0]);
			
			it1 = accmap.find(id1);
			if(it1 == accmap.end()){
				cout << "ID " << id1 << " not found, ";
				//recommend 10 existing IDs
				Rank rank;
				inorder_recommend(accmap, rank, id1);
				rank.print();
			}
			else if(money > current->money)
				cout << "fail, " << current->money << " dollars only in current account" << endl;
			else{
				transfer_log.push_back(current->transfer(it1->second,money,time++));
				cout << "success, " << current->money << " dollars left in current account" << endl;
			}
		}else if(strcmp(request, "find") == 0){
			scanf("%s",buf[0]);
			id1 = string(buf[0]);
			vector<Account *> wild;
			inorder_wild(current, &wild, id1);
			std::sort(wild.begin(),wild.end(),accptrcmp);
			if(wild.size() != 0){
				cout << wild[0]->id;
				for(unsigned int i = 1; i < wild.size(); i++)
					cout << ',' << wild[i]->id;
			}
			cout << endl;
		}else if(strcmp(request, "search") == 0){
			scanf("%s",buf[0]);
			id1 = string(buf[0]);
			current->search(id1);
		}else if(strcmp(request, "log")==0) {
			for(unsigned int i=0; i<transfer_log.size();i++)
				cout << "From " << *(transfer_log[i]->from) << " To " << *(transfer_log[i]->to) << ' ' << transfer_log[i]->money <<"\n";		
		}else{
			cout << "error input\n";
		}
	}
	return 0;
}
