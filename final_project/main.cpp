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

	while(scanf("%s", request) != EOF){
		//cout << request << endl;
		if(strcmp(request, "login") == 0){
			scanf("%s%s", id1, p);
#ifdef D_INPUT
			printf("login %s %s\n", id1, p);
#endif
			// do something here
				// printf("ID %s not found\n");
				// printf("wrong password\n");
				// printf("success\n");
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
			}
			
		}else if(strcmp(request, "delete") == 0){
			scanf("%s%s", id1, p);
#ifdef D_INPUT
			printf("delete %s %s\n", id1, p);
#endif
			// do something here
		}else if(strcmp(request, "merge") == 0){
			scanf("%s%s%s%s", id1, p, id2, p2);
#ifdef D_INPUT
			printf("merge %s %s %s %s\n", id1, p, id2, p2);
#endif
			// do something here
		}else if(strcmp(request, "deposit") == 0){
			scanf("%lld", &money);
#ifdef D_INPUT
			printf("deposit %lld\n", money);
#endif
			// do something here
		}else if(strcmp(request, "withdraw") == 0){
			scanf("%lld", &money);
#ifdef D_INPUT
			printf("withdraw %lld\n", money);
#endif
			// do something here
		}else if(strcmp(request, "transfer") == 0){
			scanf("%s%lld", id1, &money);
#ifdef D_INPUT
			printf("transfer %s %lld\n", id1, money);
#endif
			// do something here
		}else if(strcmp(request, "find") == 0){
			scanf("%s", id1);
#ifdef D_INPUT
			printf("find %s\n", id1);
#endif
			// do something here
		}else if(strcmp(request, "search") == 0){
			scanf("%s", id1);
#ifdef D_INPUT
			printf("search %s\n", id1);
#endif
			// do something here
		}else{
			cout << "error input\n";
		}
	}
	
	//delete bptr;
	
	return 0;
}
