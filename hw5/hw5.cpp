#include <iostream>
#include <stdio.h>
#include <utility>
#include "binomial_tree.h"
#include <string.h>

typedef std::pair<int,int> Job; 
//priority & id

using namespace std;

int main(){
	int c, w; // total computer, judge parameter
	int cm, id, p, cm1, cm2;
	char request[10];
	scanf("%d%d", &c, &w);
	printf("c = %d, w = %d\n", c, w);
	BinomialHeap<Job> *bptr;
	bptr = new BinomialHeap<Job>[c];
	
	while(scanf("%s", request) != EOF){
		//cout << request << endl;
		if(strcmp(request, "assign") == 0){
			scanf("%d%d%d", &cm, &id, &p);
			//printf("do assign %d %d %d\n", cm, id, p);
			Job temp(p, id);
			bptr[cm].insert(temp);
			printf("There are %d tasks on computer %d\n", bptr[cm].getsize(), cm);
		}else if(strcmp(request, "execute") == 0){
			scanf("%d", &cm);
			//printf("do execute %d\n", cm);
			Job max = bptr[cm].pop();
			printf("Computer %d executed task %d\n", cm, max.second);
			Job temp = bptr[cm].pop();
			while(max.first == temp.first){
				printf("Computer %d executed task %d\n", cm, temp.second);
				temp = bptr[cm].pop();
			}
		}else if(strcmp(request, "merge") == 0){
			scanf("%d%d", &cm1, &cm2);
			//printf("do merge %d %d\n", cm1, cm2);
			
			if(bptr[cm2].getsize() >= w){
				bptr[cm1].merge(bptr[cm2]);
				printf("The largest priority number is now %d on %d\n",	bptr[cm1].peek().first, cm1);
			}else{
				printf("Merging request failed.\n");
			}
			
		}else{
			cout << "error input\n";
		}
	}
	
	//delete bptr;
	
	return 0;
}