#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"
extern "C"{
#include "avl.h"
};
#define MAXL 105

using namespace std;

int compare(const void *pa, const void *pb, void *param){
	const Account *a = (const Account *)pa;
	const Account *b = (const Account *)pb;

	if(*a < *b)
		return -1;
	else if(*a > *b)
		return 1;
	else
		return 0;
}

int main(){
	char id1[MAXL], id2[MAXL], p[MAXL], p2[MAXL], request[MAXL]; // input id and password
	long long int money; // input money

	struct avl_table *tree;
	tree = avl_create(compare, NULL, NULL);
	Account *current = NULL;
	int time = 0;

	while(scanf("%s", request) != EOF){
		if(strcmp(request, "login") == 0){
			scanf("%s%s", id1, p);
			Account* tmp = new Account(string(id1), string(p));

			Account* account = (Account *)avl_find(tree, tmp);
			if(account == NULL)
				cout << "ID " << tmp->id << " not found" << endl;
			else if(account->password.compare(string(p)) != 0)
				cout << "wrong password" << endl;
			else{
				current = account;
				cout << "success" << endl;
			}
			delete tmp;
		}else if(strcmp(request, "create") == 0){
			scanf("%s%s", id1, p);
			Account* account = new Account(string(id1), string(p));

			if(avl_find(tree, account) == NULL){
				avl_probe(tree, account);
				cout << "success" << endl;
			}
			else{
				cout << "ID " << account->id << " exists, " << endl;
				//recommend 10 unused IDs
				delete account;
			}

		}else if(strcmp(request, "delete") == 0){
			scanf("%s%s", id1, p);
			Account* tmp = new Account(string(id1), string(p));

			Account* account = (Account *)avl_find(tree, tmp);
			if(account == NULL)
				cout << "ID " << tmp->id << " not found" << endl;
			else if(account->password.compare(string(p)) != 0)
				cout << "wrong password" << endl;
			else{
				delete account;
				cout << "success" << endl;
			}
			delete tmp;
		}else if(strcmp(request, "merge") == 0){
			scanf("%s%s%s%s", id1, p, id2, p2);
			Account* tmp1 = new Account(string(id1), string(p));
			Account* tmp2 = new Account(string(id2), string(p2));

			Account* account1 = (Account *)avl_find(tree, tmp1);
			Account* account2 = (Account *)avl_find(tree, tmp2);
			if(account1 == NULL)
				cout << "ID " << tmp1->id << " not found" << endl;
			else if(account2 == NULL)
				cout << "ID " << tmp2->id << " not found" << endl;
			else if(account1->password.compare(string(p)) != 0)
				cout << "wrong password1" << endl;
			else if(account2->password.compare(string(p2)) != 0)
				cout << "wrong password2" << endl;
			else{
				merge(account1, account2);
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

			Account* account = (Account *)avl_find(tree, tmp);
			if(account == NULL){
				cout << "ID " << tmp->id << " not found" << endl;
				//recommend 10 existing IDs
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
#ifdef D_INPUT
			printf("find %s\n", id1);
#endif
			// do something here
		}else if(strcmp(request, "search") == 0){
			scanf("%s", id1);
			for(unsigned int i = 0; i < (*current->history).size(); i++){
				if((*current->history)[i]->id.compare(id1) == 0 && (*current->history)[i]->type == in)
					cout << "From " << (*current->history)[i]->id << " " << (*current->history)[i]->money <<endl;
				else if((*current->history)[i]->id.compare(id1) == 0 && (*current->history)[i]->type == out)
					cout << "To " << (*current->history)[i]->id << " " << (*current->history)[i]->money <<endl;
			}
		}else{
			cout << "error input\n";
		}
	}

	//delete bptr;

	return 0;
}
