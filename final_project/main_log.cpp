#include <iostream>
#include <string.h>
#include "sharedlog.h"
#include "recommend.h"
extern "C"{
#include "rb.h"
};
#include "md5.h"
#define MAXL 105

using namespace std;


int compare(const void *pa, const void *pb, void *param){
	if(*(const Account*)pa < *(const Account*)pb)
		return -1;
	else if(*(const Account*)pa > *(const Account*)pb)
		return 1;
	else
		return 0;
}
#ifdef POOL
MemoryPool* Account::pool	= new MemoryPool(sizeof(Account),5000);
MemoryPool* tree_pool		= new MemoryPool(sizeof(rb_node),5000);

void *rb_poolalloc (struct libavl_allocator *allocator, size_t size)
{	return tree_pool->alloc(size);	}
void rb_poolfree (struct libavl_allocator *allocator, void *block)
{	tree_pool->dealloc(block);		}
struct libavl_allocator rb_allocator_pool =
{	rb_poolalloc,rb_poolfree		};
struct rb_table* tree = rb_create(compare, NULL, &rb_allocator_pool);
#else
struct rb_table* tree = rb_create(compare, NULL, NULL);
#endif

bool find(const string& id){
	string nullstr("");
	Account tmp(id, nullstr);
	return rb_find(tree, &tmp)==NULL;
}

void inorder_recommend(const struct rb_node *node, Rank& r, const string& origin){
	if(node == NULL) return;
	if(node->rb_link[0] != NULL)
		inorder_recommend(node->rb_link[0], r, origin);
	r.update(((Account *)node->rb_data)->id, score(((Account *)node->rb_data)->id, origin));
	if(node->rb_link[1] != NULL)
		inorder_recommend(node->rb_link[1], r, origin);
}

void inorder_wild(const struct rb_node *node, vector<Account *>* v, const string& wild, const Account* current){
	if(node == NULL) return;
	if(node->rb_link[0] != NULL)
		inorder_wild(node->rb_link[0], v, wild, current);
	if(match_wild(((Account *)node->rb_data)->id, wild) && node->rb_data!=current)
		v->push_back((Account *)node->rb_data);
	if(node->rb_link[1] != NULL)
		inorder_wild(node->rb_link[1], v, wild, current);
}

int main(){
	char request[MAXL]; 
	char buf[4][MAXL];
	string id1,id2,p,p2; // input id and password
	long long int money; // input money
	vector<Transfer*> transfer_log; //all transfer log
	string nullstr("");
	
	Account *current = NULL;
	int time = 0;

	while(scanf("%s", request) != EOF){
		if(strcmp(request, "login") == 0){
			//cin>>id1>>p;
			scanf("%s%s",buf[0],buf[1]);
			id1 = string(buf[0]);
			p   = string(buf[1]);
			Account tmp(id1, nullstr);

			Account* account = (Account *)rb_find(tree, &tmp);
			if(account == NULL)
				cout << "ID " << tmp.id << " not found" << endl;
			else if(account->password.compare(md5(string(p))) != 0)
				cout << "wrong password" << endl;
			else{
				current = account;
				cout << "success" << endl;
			}
		
		}else if(strcmp(request, "create") == 0){
			//cin>>id1>>p;
			scanf("%s%s",buf[0],buf[1]);
			id1 = string(buf[0]);
			p   = string(buf[1]);
			Account tmp(id1, nullstr);
			if(rb_find(tree, &tmp) == NULL){
				Account* account = new Account(*(new string(id1)), md5(p));
				rb_probe(tree, account);
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
			//cin>>id1>>p;
			scanf("%s%s",buf[0],buf[1]);
			id1 = string(buf[0]);
			p   = string(buf[1]);
			Account tmp (id1, nullstr);
			
			Account* account = (Account *)rb_find(tree, &tmp);
			if(account == NULL)
				cout << "ID " << tmp.id << " not found" << endl;
			else if(account->password.compare(md5(string(p))) != 0)
				cout << "wrong password" << endl;
			else{
				account = (Account *) rb_delete(tree, account);
				delete account;
				cout << "success" << endl;
			}
		}else if(strcmp(request, "merge") == 0){
			//cin>>id1>>p>>id2>>p2;
			scanf("%s%s%s%s",buf[0],buf[1],buf[2],buf[3]);
			id1 = string(buf[0]);
			p   = string(buf[1]);
			id2 = string(buf[2]);
			p2  = string(buf[3]);
			Account tmp1(id1, nullstr);
			Account tmp2(id2, nullstr);

			Account* account1 = (Account *)rb_find(tree, &tmp1);
			Account* account2 = (Account *)rb_find(tree, &tmp2);
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
				account2 = (Account *) rb_delete(tree, account2);
				delete account2;
				cout << "success, " << account1->id << " has " << account1->money << " dollars" << endl;
			}
		}else if(strcmp(request, "deposit") == 0){
			//cin>>money;
			scanf("%lld",&money);
			current->money += money;
			cout << "success, " << current->money << " dollars in current account" << endl;
		}else if(strcmp(request, "withdraw") == 0){
			//cin>>money;
			scanf("%lld",&money);
			if(money > current->money)
				cout << "fail, " << current->money << " dollars only in current account" << endl;
			else{
				current->money -= money;
				cout << "success, " << current->money << " dollars left in current account" << endl;
			}
		}else if(strcmp(request, "transfer") == 0){
			//cin>>id1>>money;
			scanf("%s%lld",buf[0],&money);
			id1 = string(buf[0]);
			
			Account tmp(id1, nullstr);
			Account* account = (Account *)rb_find(tree, &tmp);
			if(account == NULL){
				cout << "ID " << tmp.id << " not found, ";
				//recommend 10 existing IDs
				Rank rank;
				inorder_recommend(tree->rb_root, rank, tmp.id);
				rank.print();
			}
			else if(money > current->money)
				cout << "fail, " << current->money << " dollars only in current account" << endl;
			else{
				transfer_log.push_back(current->transfer(account,money,time++));
				cout << "success, " << current->money << " dollars left in current account" << endl;
			}
		}else if(strcmp(request, "find") == 0){
			//cin>>id1;
			scanf("%s",buf[0]);
			id1 = string(buf[0]);
			vector<Account *> wild;
			inorder_wild(tree->rb_root, &wild, id1, current);
			if(wild.size() != 0){
				cout << wild[0]->id;
				for(unsigned int i = 1; i < wild.size(); i++)
					cout << ',' << wild[i]->id;
			}
			cout << endl;
		}else if(strcmp(request, "search") == 0){
			//cin>>id1;
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
