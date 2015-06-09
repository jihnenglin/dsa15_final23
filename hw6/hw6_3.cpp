#include <stdio.h>
#include <iostream>
#include <vector>

extern "C"{
#include "avl_ntudsa.h"
};


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
	int flag = 0;
	for(int i = 1; i <= n; i++){
		if(owner(i, gp) == o){
			cout << i << " ";
			flag = 1;
		}
	}
	cout << endl;
	if(flag == 0){
		cout << "Not found\n";
	}
	return;
}

void merge_tree(struct avl_table *taker, struct avl_node *node){
	if(node == NULL){
		return;
	}
	//printf("%d ", (node -> avl_data)); 
	int *p = avl_probe(taker, (node -> avl_data));
	if(node->avl_link[0] != NULL || node->avl_link[1] != NULL){
		merge_tree(taker, node->avl_link[0]);
		merge_tree(taker, node->avl_link[1]);
	}
}

void preorder_char_avl(const struct avl_node *node){
	if(node == NULL){
		return;
	}
	printf("%d ", (node -> avl_data)); 
	if(node->avl_link[0] != NULL || node->avl_link[1] != NULL){
		putchar('(');
		preorder_char_avl(node->avl_link[0]);
		putchar(',');
		putchar(' ');
		preorder_char_avl(node->avl_link[1]);
		putchar(')');
	}
}

int buyGame(const struct avl_node *node, long long int s){
	if(s >= (node->avl_data * node->avl_cnt + node->avl_sum[0] + node->avl_sum[1]) ){
		return (node->avl_cnt + node->avl_cnode[0] + node->avl_cnode[1]);
	}
	if(s > (node->avl_data * node->avl_cnt + node->avl_sum[0])){
		return (node->avl_cnt + node->avl_cnode[0] + buyGame(node->avl_link[1], s - (node->avl_data * node->avl_cnt + node->avl_sum[0])));
	}
	if(s > (node->avl_sum[0])){
		return node->avl_cnode[0] + (s - node->avl_sum[0]) / node->avl_data;
	}
	return buyGame(node->avl_link[0], s);
}

int main(){
	// n is the number of friends & games
	// m is the number of incidents
	int n, m;  
	scanf("%d%d", &n, &m);
//printf("%d %d\n", n, m);
	struct avl_table *forest[n + 1];
	
	//initialize game id partition
	int *gp = new int[n + 1];
	for(int i = 0; i <= n; i++){
		gp[i] = i;
	}
	
	
	int temp; // to take in the price and incident num	
	
	//get n numbers of price
	//initialize the price
	vector<int> price;
	price.push_back(0);
	for(int i = 1; i <= n; i++){
		forest[i] = avl_create(int_compare, NULL, NULL);
		scanf("%d", &temp);
		price.push_back(temp);
//printf("%d ", price[i]);
		int *p = avl_probe(forest[i], temp);
	}
	//cout << endl;
	
	int j, k; // to take in the condition
	long long int s; // to take in the dollar
	
	for(int i = 0; i < m; i++){
		scanf("%d", &temp);
//printf("%d %d %d\n", temp, j, k);
		if(temp == 1){
			//owner of j game want to take all game of owner of k game
			scanf("%d%d", &j, &k);
			int taker = owner(j, gp);
			int giver = owner(k, gp);
			gp[giver] = taker;
			
			//printf("%d take all game of %d\n", taker, giver);
			//cout << "Friend no." << taker << " : ";
			//printOwnGame(n, taker, gp);
			
			//merge forest
			if(taker != giver){
				if(forest[taker]->avl_count >= forest[giver]->avl_count){
					merge_tree(forest[taker], forest[giver]->avl_root);
					//cout << endl;
					//cout << "*** print price tree ***\n";
					//preorder_char_avl(forest[taker]->avl_root);
					//cout << endl << " **********\n";
					//cout << "check 1\n";
				}else{
					merge_tree(forest[giver], forest[taker]->avl_root);
					forest[taker] = forest[giver];
					//preorder_char_avl(forest[taker]->avl_root);
					//cout << endl;
					//cout << "check 2\n";
				}
			}
			
		}else if(temp == 2){
			//come to owner of game j with s dollars
			scanf("%d%lld", &j, &s);
			int u = owner(j, gp);
			cout << u << " ";
			
			int total_num = buyGame(forest[u]->avl_root, s);
			
			cout << total_num << "\n";
			
		}else{
			cout << "error input\n";
		}
	}
	
	delete [] gp;
	
	return 0;
}