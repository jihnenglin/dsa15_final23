#include <stdio.h>
#include <iostream>
#include <vector>
/*
extern "C"{
#include "avl_ntudsa.h"
};
*/

using namespace std;

int int_compare(const void *pa, const void *pb, void *param){
	int a = *(const int *)pa;
	int b = *(const int *)pb;
	
	if(a < b) return -1;
	else if(a > b) return +1;
	else return 0;
}

int owner(int id, int *gp){
	while(gp[id] != id){
		id = gp[id];
	}
	return id;
}

void printOwnGame(int n, int o, int *gp){
	
}

int main(){
	// n is the number of friends & games
	// m is the number of incidents
	int n, m;  
	scanf("%d%d", &n, &m);
//printf("%d %d\n", n, m);
	//struct avl_table *forest[n + 1];
	//struct avl_table *friends[n + 1];
	
	//initialize game id partition
	int *gp = new int[n];
	for(int i = 0; i <= n; i++){
		gp[i] = i;
	}
	
	
	int temp; // to take in the price and incident num	
	
	//get n numbers of price
	//initialize the price
	vector<int> price;
	price.push_back(0);
	for(int i = 1; i <= n; i++){
//forest[i] = avl_create(int_compare, NULL, NULL);
		scanf("%d", &temp);
		price.push_back(temp);
//printf("%d ", price[i]);
//int *p = avl_probe(forest[i], temp);
	}
	cout << endl;
	
	int j, k; // to take in the condition
	
	for(int i = 0; i < m; i++){
		scanf("%d%d%d", &temp, &j, &k);
//printf("%d %d %d\n", temp, j, k);
		if(temp == 1){
			int taker = owner(j, gp);
			int giver = owner(k, gp);
			gp[giver] = taker;
			printOwnGame(taker);
			printf("%d take all game of %d\n", taker, giver);
		}else if(temp == 2){
		
		}else{
			
		}
	}
	
	delete [] gp;
	
	return 0;
}