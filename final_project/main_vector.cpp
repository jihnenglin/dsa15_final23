#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"
#include "recommend.h"

#include <vector>
#include "md5.h"
#define MAXL 105

typedef std::vector<Account*> Datas;

using namespace std;

int search(vector<History*>* history, int& time){
	int left = 0, right = (int) (*history).size() - 1;
	while(left <= right){
        	int mid = left + (right - left) / 2;
        	if((*history)[mid]->time == time)
            		return mid;
        	else if((*history)[mid]->time > time)
            		right = mid - 1;
        	else
            		left = mid + 1;
	}
	return -1;
}

Account* vector_find(Datas &list, const Account* account){
	for(int i = 0; i < (int)list.size(); i++){
		if(list[i]->id.compare(account->id) == 0){
			return list[i];
		}
	}
	return NULL;
}

Account* vector_delete(Datas &list, Account* account){
	int i = 0;
	for(i = 0; i < (int)list.size(); i++){
		if(list[i]->id.compare(account->id) == 0){
			break;
		}
	}
	
	list.erase(list.begin() + 1 + i);
	return account;
}

bool find(const string& id){
	Account* tmp = new Account(id, string(""));
	if(vector_find(tree, tmp) == NULL){
		delete tmp;
		return true;
	}
	else{
		delete tmp;
		return false;
	}
}

void inorder_recommend(Datas &list, Rank& r, const string& origin){
	for(i = 0; i < (int)list.size(); i++){
		r.update(list[i]->id, score(list[i]->id, origin));
	}
}

void inorder_wild(Datas &list, vector<Account *>* v, const string& wild){
	for(i = 0; i < (int)list.size(); i++){
		if(match_wild(list[i]->id, wild))
			v->push_back(list[i]);
	}
}

int main(){
	char id1[MAXL], id2[MAXL], p[MAXL], p2[MAXL], request[MAXL]; // input id and password
	long long int money; // input money

	Datas list;
	int time = 0;
	Account* current = NULL;
	while(scanf("%s", request) != EOF){
		if(strcmp(request, "login") == 0){
			scanf("%s%s", id1, p);
			Account* tmp = new Account(string(id1), md5(string(p)));

			Account* account = vector_find(list, tmp);
			if(account == NULL)
				cout << "ID " << tmp->id << " not found" << endl;
			else if(account->password.compare(md5(string(p))) != 0)
				cout << "wrong password" << endl;
			else{
				current = account;
				cout << "success" << endl;
			}
			delete tmp;
		}else if(strcmp(request, "create") == 0){
			scanf("%s%s", id1, p);
			Account* account = new Account(string(id1), md5(string(p)));

			if(vector_find(list, account) == NULL){
				list.push_back(account);
				cout << "success" << endl;
			}else{
				cout << "ID " << account->id << " exists, " << endl;
				//recommend 10 unused IDs
				Rank rank;
				Recommend recommend(find);
				recommend.getRank(rank, account->id);
				rank.print();
				delete account;
			}

		}else if(strcmp(request, "delete") == 0){
			scanf("%s%s", id1, p);
			Account* tmp = new Account(string(id1), md5(string(p)));

			Account* account = vector_find(list, tmp);
			if(account == NULL)
				cout << "ID " << tmp->id << " not found" << endl;
			else if(account->password.compare(md5(string(p))) != 0)
				cout << "wrong password" << endl;
			else{
				account = vector_delete(list, account);
				delete account;
				cout << "success" << endl;
			}
			delete tmp;
		}else if(strcmp(request, "merge") == 0){
			scanf("%s%s%s%s", id1, p, id2, p2);
			Account* tmp1 = new Account(string(id1), md5(string(p)));
			Account* tmp2 = new Account(string(id2), md5(string(p2)));

			Account* account1 = vector_find(list, tmp1);
			Account* account2 = vector_find(list, tmp2);
			if(account1 == NULL)
				cout << "ID " << tmp1->id << " not found" << endl;
			else if(account2 == NULL)
				cout << "ID " << tmp2->id << " not found" << endl;
			else if(account1->password.compare(md5(string(p))) != 0)
				cout << "wrong password1" << endl;
			else if(account2->password.compare(md5(string(p2))) != 0)
				cout << "wrong password2" << endl;
			else{
				for(unsigned int j = 0; j < (*account2->history).size(); j++){
        			Account* tmp = new Account((*account2->history)[j]->id, string(""));
        			Account* account = vector_find(list, tmp);
			        if(account == NULL){
						continue;
					}
					int k = search(account->history, (*account2->history)[j]->time);
			       	if(k == -1){
						//cout << "search error" <<endl;
						(*account->history)[k]->id = account1->id;
					}
					delete tmp;
				}
				merge(account1, account2);
				account2 = vector_delete(list, account2);
				delete account2;
				cout << "success, " << account1->id << " has " << account1->money << " dollars" << endl;
			}
			delete tmp1;
			delete tmp2;
		}else if(strcmp(request, "deposit") == 0){
			scanf("%lld", &money);
			current->money += money;
			cout << "success, " << current->money << " dollars in current account" << endl;
		}else if(strcmp(request, "withdraw") == 0){
			scanf("%lld", &money);
			if(money > current->money)
				cout << "fail, " << current->money << " dollars only in current account" << endl;
			else{
				current->money -= money;
				cout << "success, " << current->money << " dollars left in current account" << endl;
			}
		}else if(strcmp(request, "transfer") == 0){
			scanf("%s%lld", id1, &money);
			Account* tmp = new Account(string(id1), string(""));

			Account* account = vector_find(list, tmp);
			if(account == NULL){
				cout << "ID " << tmp->id << " not found" << endl;
				//recommend 10 existing IDs
				Rank rank;
				inorder_recommend(list, rank, tmp->id);
				rank.print();
			}
			else if(money > current->money)
				cout << "fail, " << current->money << " dollars only in current account" << endl;
			else{
				current->money -= money;
				History* history_out = new History(out, account->id, money, time);
				(*current->history).push_back(history_out);
				account->money += money;
				History* history_in = new History(in, current->id, money, time);
				(*account->history).push_back(history_in);
				time++;
				cout << "success, " << current->money << " dollars left in current account" << endl;
			}
			delete tmp;
		}else if(strcmp(request, "find") == 0){
			scanf("%s", id1);
			vector<Account *>* wild = new vector<Account *>;
			inorder_wild(list, wild, string(id1));
			if(wild->size() != 0){
				cout << (*wild)[0]->id;
				for(unsigned int i = 1; i < wild->size(); i++)
					cout << ',' << (*wild)[i]->id;
			}
			cout << endl;
			delete wild;
		}else if(strcmp(request, "search") == 0){
			scanf("%s", id1);
			bool record = false;
			for(unsigned int i = 0; i < (*current->history).size(); i++){
				if((*current->history)[i]->id.compare(id1) == 0 && (*current->history)[i]->type == in){
					cout << "From " << (*current->history)[i]->id << " " << (*current->history)[i]->money <<endl;
					record = true;
				}else if((*current->history)[i]->id.compare(id1) == 0 && (*current->history)[i]->type == out){
					cout << "To " << (*current->history)[i]->id << " " << (*current->history)[i]->money <<endl;
					record = true;
				}
			}
			if(!record){
				cout << "no record" << endl;
			}
		}else{
			cout << "error input\n";
		}
	}

	return 0;
}
