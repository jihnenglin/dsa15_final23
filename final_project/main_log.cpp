#include <iostream>
#include <string.h>
#include "sharedlog.h"
#include "recommend.h"
extern "C"{
#include "avl.h"
};
#include "md5.h"
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

struct avl_table* tree = avl_create(compare, NULL, NULL);

bool find(const string& id){
	string nullstr("");
	Account tmp(id, nullstr);
	return avl_find(tree, &tmp)==NULL;
}
/*
void inorder_recommend(struct avl_node* node, Rank& r, const string& origin){
	struct avl_node *stack[32];
	int size = 0;
	while(size>0 || node) {
		if(node) {
			stack[size++] = node;
			node = node->avl_link[0];		
		} else {
			node = stack[--size];
			r.update(((Account *)node->avl_data)->id, score(((Account *)node->avl_data)->id, origin));
			node = node->avl_link[1];
		}
	}	
}

void inorder_wild(struct avl_node *node, vector<Account *>* v, const string& wild, const unsigned int& pos){
	struct avl_node *stack[32];
	int size = 0;
	while(size>0 || node) {
		if(node) {
			stack[size++] = node;
			node = node->avl_link[0];		
		} else {
			node = stack[--size];
			if(match_wild(((Account *)node->avl_data)->id, wild))
				v->push_back((Account *)node->avl_data);
			node = node->avl_link[1];
		}
	}
}
*/

void inorder_recommend(const struct avl_node *node, Rank& r, const string& origin){
	if(node == NULL) return;
	if(node->avl_link[0] != NULL)
		inorder_recommend(node->avl_link[0], r, origin);
	r.update(((Account *)node->avl_data)->id, score(((Account *)node->avl_data)->id, origin));
	if(node->avl_link[1] != NULL)
		inorder_recommend(node->avl_link[1], r, origin);
}

void inorder_wild(const struct avl_node *node, vector<Account *>* v, const string& wild, const unsigned int pos){
 	if(node == NULL) return;
 	const string &id = ((Account *)node->avl_data)->id;
	const int ret = id.compare(0, pos<id.size()?pos:id.size(), wild, 0, pos<id.size()?pos:id.size());
	if(node->avl_link[0] != NULL && ret >= 0)
 		inorder_wild(node->avl_link[0], v, wild, pos);
 	if(match_wild(id, wild))
		v->push_back((Account *)node->avl_data);
 	if(node->avl_link[1] != NULL && ret <= 0)
 		inorder_wild(node->avl_link[1], v, wild, pos);
}

int main(){
	char request[MAXL]; 
	string id1,id2,p,p2; // input id and password
	long long int money; // input money
	vector<Transfer*> transfer_log; //all transfer log
	string nullstr("");
	
	Account *current = NULL;
	int time = 0;

	while(scanf("%s", request) != EOF){
		if(strcmp(request, "login") == 0){
			cin>>id1>>p;
			Account tmp(id1, nullstr);

			Account* account = (Account *)avl_find(tree, &tmp);
			if(account == NULL)
				cout << "ID " << tmp.id << " not found" << endl;
			else if(account->password.compare(md5(string(p))) != 0)
				cout << "wrong password" << endl;
			else{
				current = account;
				cout << "success" << endl;
			}
		}else if(strcmp(request, "create") == 0){
			cin>>id1>>p;
			
			Account tmp(id1, nullstr);
			if(avl_find(tree, &tmp) == NULL){
				Account* account = new Account(*(new string(id1)), md5(p));
				avl_probe(tree, account);
				cout << "success" << endl;
			}
			else{
				cout << "ID " << id1 << " exists, ";
				//recommend 10 unused IDs
				Rank rank;
				Recommend recommend(find);
				recommend.getRank(rank, tmp.id);
				rank.print();
			}

		}else if(strcmp(request, "delete") == 0){			
			cin>>id1>>p;
			Account tmp (id1, nullstr);
			
			Account* account = (Account *)avl_find(tree, &tmp);
			if(account == NULL)
				cout << "ID " << tmp.id << " not found" << endl;
			else if(account->password.compare(md5(string(p))) != 0)
				cout << "wrong password" << endl;
			else{
				account = (Account *) avl_delete(tree, account);
				delete account;
				cout << "success" << endl;
			}
		}else if(strcmp(request, "merge") == 0){
			cin>>id1>>p>>id2>>p2;
			Account tmp1(id1, nullstr);
			Account tmp2(id2, nullstr);

			Account* account1 = (Account *)avl_find(tree, &tmp1);
			Account* account2 = (Account *)avl_find(tree, &tmp2);
			if(account1 == NULL)
				cout << "ID " << tmp1.id << " not found" << endl;
			else if(account2 == NULL)
				cout << "ID " << tmp2.id << " not found" << endl;
			else if(account1->password.compare(md5(p)) != 0)
				cout << "wrong password1" << endl;
			else if(account2->password.compare(md5(p2)) != 0)
				cout << "wrong password2" << endl;
			else{
				account1->merge(account2);
				account2 = (Account *) avl_delete(tree, account2);
				delete account2;
				cout << "success, " << account1->id << " has " << account1->money << " dollars" << endl;
			}
		}else if(strcmp(request, "deposit") == 0){
			cin>>money;
			current->money += money;
			cout << "success, " << current->money << " dollars in current account" << endl;
		}else if(strcmp(request, "withdraw") == 0){
			cin>>money;
			if(money > current->money)
				cout << "fail, " << current->money << " dollars only in current account" << endl;
			else{
				current->money -= money;
				cout << "success, " << current->money << " dollars left in current account" << endl;
			}
		}else if(strcmp(request, "transfer") == 0){
			cin>>id1>>money;
			Account tmp(id1, nullstr);

			Account* account = (Account *)avl_find(tree, &tmp);
			if(account == NULL){
				cout << "ID " << tmp.id << " not found, ";
				//recommend 10 existing IDs
				Rank rank;
				inorder_recommend(tree->avl_root, rank, tmp.id);
				rank.print();
			}
			else if(money > current->money)
				cout << "fail, " << current->money << " dollars only in current account" << endl;
			else{
				transfer_log.push_back(current->transfer(account,money,time++));
				cout << "success, " << current->money << " dollars left in current account" << endl;
			}
		}else if(strcmp(request, "find") == 0){
			cin>>id1;
			vector<Account *>* wild = new vector<Account *>;
			unsigned int pos = string(id1).find_first_of("*?");
			inorder_wild(tree->avl_root, wild, id1,pos==string::npos?(wild->size()-1):pos);
			if(wild->size() != 0){
				cout << (*wild)[0]->id;
				for(unsigned int i = 1; i < wild->size(); i++)
					cout << ',' << (*wild)[i]->id;
			}
			cout << endl;
			delete wild;
		}else if(strcmp(request, "search") == 0){
			cin>>id1;
			current->search(id1);
		}else{
			cout << "error input\n";
		}
	}
	return 0;
}