#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "avl.h"
#include "rb.h"
#include "bst.h"

#define MAXL 130

void preorder_char_avl(const struct avl_node *node){
	if(node == NULL){
		return;
	}
	printf("%s ", ((char *)node -> avl_data)); 
	if(node->avl_link[0] != NULL || node->avl_link[1] != NULL){
		putchar('(');
		preorder_char_avl(node->avl_link[0]);
		putchar(',');
		putchar(' ');
		preorder_char_avl(node->avl_link[1]);
		putchar(')');
	}
}

void preorder_char_bst(const struct bst_node *node){
	if(node == NULL){
		return;
	}
	printf("%s ", ((char *)node -> bst_data)); 
	if(node->bst_link[0] != NULL || node->bst_link[1] != NULL){
		putchar('(');
		preorder_char_bst(node->bst_link[0]);
		putchar(',');
		putchar(' ');
		preorder_char_bst(node->bst_link[1]);
		putchar(')');
	}
}

void preorder_char_rb(const struct rb_node *node){
	if(node == NULL){
		return;
	}
	printf("%s ", ((char *)node -> rb_data)); 
	if(node->rb_link[0] != NULL || node->rb_link[1] != NULL){
		putchar('(');
		preorder_char_rb(node->rb_link[0]);
		putchar(',');
		putchar(' ');
		preorder_char_rb(node->rb_link[1]);
		putchar(')');
	}
}

int char_compare(const void *pa, const void *pb, void *param){
	const char *a = (const char *)pa;
	const char *b = (const char *)pb;
	
	return strcmp(a,b);
	return strcmp((const char *)pa,(const char *)pb);
}


int main(){
	struct avl_table *avl_tree;
	struct bst_table *bst_tree;
	struct rb_table *rb_tree;
	avl_tree = avl_create(char_compare, NULL, NULL);
	bst_tree = bst_create(char_compare, NULL, NULL);
	rb_tree = rb_create(char_compare, NULL, NULL);
	
	int i;
	for(i = 0; i < 16; i++){
		char *element = (char*)malloc(sizeof(char) * MAXL);
		scanf("%s", element);
		printf("%s\n", element);
		void **p = avl_probe(avl_tree, element);
		void **b = bst_probe(bst_tree, element);
		void **r = rb_probe(rb_tree, element);
	}
	
	printf("AVL TREE : \n");
	preorder_char_avl(avl_tree->avl_root);
	printf("\n");
	printf("BST TREE : \n");
	preorder_char_bst(bst_tree->bst_root);
	printf("\n");
	printf("RB TREE : \n");
	preorder_char_rb(rb_tree->rb_root);
	printf("\n");
	
	return 0;
}
