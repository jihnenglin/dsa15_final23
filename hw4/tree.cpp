#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <math.h>
#include <algorithm>

#define MAX_FEATURE (1024+3)

using namespace std;

class Data{
	public:
		int label;
		double* array;
		Data(){
			array = new double[MAX_FEATURE];
			for(int i = 0; i < MAX_FEATURE; i++){
				array[i] = 0;
			}
		};
		
		Data(Data &a){
			label = a.label;
			array = new double[MAX_FEATURE];
			for(int i = 0; i < MAX_FEATURE; i++){
				array[i] = a.array[i];
			}
		};
		
		~Data(){
			delete [] array;
		}
};

class Tree{
	public:
		int id;
		double thrd;
		int label;
		Tree* left;//if larger than threshold
		Tree* right;
		bool isLeaf(){
			return (left == NULL && right == NULL);
		}
};

class Id{
	public:
		double thrd;  // threshold
		double con;   // confusion
		int label;
};

class Cmp{  // for compair
	public:
		int label;
		double feature;
};

bool mycmp(Cmp a, Cmp b){
	return (a.feature < b.feature);
}

double confusion(int a, int b){
	double sum = a + b;
	if(sum == 0) return 0;
	return (1 - pow((a/sum),2) - pow((b/sum),2));
}

double total_confusion(int a, int b, int c, int d){
	double sum = a + b + c + d;
	return (((a + b) / sum) * confusion(a, b) + ((c + d) / sum) * confusion(c, d));
}

void getIdtemp(vector<Cmp> cp, Id &idtemp, int tY, int tN){
	sort(cp.begin(), cp.end(), mycmp);
	int rY = tY, rN = tN; // init rY, rN
	int lY = 0, lN = 0;   // init lY, lN
	double min_tconfusion = total_confusion(lY, lN, rY, rN); // init mini total confusion
	int thrd = cp[0].feature; //init thrd
	int i = 0; int tconf;
	while(i < (int)cp.size()){
		tconf = total_confusion(lY, lN, rY, rN); 
		if(tconf < min_tconfusion){
			thrd = cp[i].feature;
			min_tconfusion = tconf;
		}
		i++;
		while(i < (int)cp.size() && cp[i - 1].feature == cp[i].feature){
			if(cp[i].label == -1){
				rN -= 1;
				lN += 1;
			}else{
				rY -= 1;
				lY += 1;
			}
			i++;
		}
		
	}	
	tconf = total_confusion(lY, lN, rY, rN); 
	if(tconf < min_tconfusion){
		thrd = cp[i - 1].feature;
		min_tconfusion = tconf;
	}
	idtemp.thrd = thrd;
	idtemp.con = min_tconfusion;
	//get label
	int N = 0, Y = 0;
	for(i = 0; i < (int)cp.size(); i++){
		if(cp[i].feature > thrd){
			if(cp[i].label == -1)
				N++;
			else
				Y++;
		}
	} 
	if(Y > N){
		idtemp.label = 1;
	}else{
		idtemp.label = -1;
	}
}

void find_ct(vector<Data*> dataSet, int idNum, vector<Id> &ids){
	//count AYBN
	int tY = 0, tN = 0, i, j;
	for(i = 0; i < (int)dataSet.size(); i++){
		if(dataSet[i]->label == 1){tY++;
		}else{tN++;}
	}
	//get every ids' threshold and total confusion
	for(i = 0; i < idNum; i++){
		//construct a vector with label and id inside
		vector<Cmp> cp;
		for(j = 0; j < (int)dataSet.size(); j++){
			Cmp temp;
			temp.label = dataSet[j]->label;
			temp.feature = dataSet[j]->array[i];
			cp.push_back(temp);
		}
		//get  id's feature
		Id idtemp;
		getIdtemp(cp, idtemp, tY, tN);
		ids.push_back(idtemp);
	}
}

Tree* buildTree(vector<Data*> dataSet, int idNum, int eps){
	int i;

	//calculate total confusion and threshold for each feature
	vector<Id> ids;
	find_ct(dataSet, idNum, ids);
	
	//find least total confusion
	double ltC = ids[0].con;
	int idGet = 0;
	for(i = 0; i <= idNum; i++){
		if(ltC > ids[i].con){
			ltC = ids[i].con;
			idGet = i;
		}
	}
	
	//make the tree
	Tree* temp;
	temp = new Tree;
	if(ltC <= eps){        		         //leaf
		temp->left = NULL;
		temp->right = NULL;
		temp->label = ids[idGet].label;
		
	}else{	                        	//internal
		//build two data set
		vector<Data*> ldata, rdata;
		for(i = 0; i < (int)dataSet.size(); i++){
			if(dataSet[i]->array[idGet] > ids[idGet].thrd){
				rdata.push_back(dataSet[i]);
			}else{
				ldata.push_back(dataSet[i]);
			}
		}
		temp->id = idGet;
		temp->thrd = ids[idGet].thrd;
		temp->left = buildTree(ldata, idNum, eps);
		temp->right = buildTree(rdata, idNum, eps);		
	}
	return temp;
}

void print_space(fstream &file, int level){
	for(int i = 0; i < level; i++){
		file << " ";
	}
	return;
}

void printTree(Tree* root, fstream &file, int level){
	print_space(file, level);
	if(root->isLeaf()){
		file << "return " << root->label << ";\n";
		return;
	}else{
		file << "if(attr[" << root->id << "] > " << thrd << "){";
		printTree(root->right, file, level + 1);
		print_space(file, level);
		file << "}\n";
		print_space(file, level);
		file << "else{\n";
		printTree(root->left, file, level + 1);
		print_space(file, level);
		file << "}\n";
		return;
	}
}

int main(int argc, char **argv){
	ifstream df;
	df.open(argv[1]);
	
	stringstream ss;
	int epsilon;
	ss << argv[2];
	ss >> epsilon;
	
	string line;
	//int total = 0;
	vector<Data*> dataSet;
	int idNum = 0;
	
	while(getline(df, line)){
	//	total++;
		char *cstring, *tmp;
		Data* dtemp = new Data;
		
		cstring = new char[line.size() + 1];
		strncpy(cstring, line.c_str(), line.size()+1);
		
		tmp =  strtok(cstring, ": ");
		dtemp->label = atoi(tmp);
		tmp = strtok(NULL, ": ");

		while(tmp != NULL) {
		  int id = atoi(tmp);
		  if(id > idNum){
			  idNum = id;
		  }
		  tmp = strtok(NULL, ": ");
		  double features = atof(tmp);
		  dtemp->array[id] = features;
		  tmp = strtok(NULL, ": ");
		}
		
		delete[] cstring;
		dataSet.push_back(dtemp);
		delete dtemp;
	}
	
	Tree* root;
	//make decision tree
	root = buildTree(dataSet, idNum, epsilon);
	
	//output tree
	fstream file;
	file.open("tree_pred_func.cpp",ios::out);
	file << "int tree_predict(double *attr){\n";
	printTree(root, file, 1);
	file << "}\n";
	//delete root;
	return 0;
}