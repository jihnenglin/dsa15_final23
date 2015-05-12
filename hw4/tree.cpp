#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <math.h>
#include <algorithm>

#define MAX_FEATURE (1024+8)

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
		bool isId;
		double thrd;  // threshold
		double con;   // confusion
		int label;
};

class Cmp{  // for compair
	public:
		int label;
		double feature;
};

int mycmp(const Cmp &a, const Cmp &b){
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

void printCmp(vector<Cmp> cp){
	cout << "*************CP*************\n";
	for(int i = 0; i < (int)cp.size(); i++){
		cout << cp[i].label << " " << cp[i].feature << endl;
	}
	cout << "*************END*************\n";
}

void getIdtemp(vector<Cmp> cp, Id &idtemp, int tY, int tN){
	sort(cp.begin(), cp.end(), mycmp);
	printCmp(cp);
	int rY = tY, rN = tN; // init rY, rN
	int lY = 0, lN = 0;   // init lY, lN
	double min_tconfusion = 10000; // init mini total confusion
	double thrd = 0; //init thrd
	int i = 0; double tconf;
	while(i < (int)cp.size()){
		tconf = total_confusion(lY, lN, rY, rN); 
	printf("data[%d] : %dY%dN %dY%dN, confusion = %lf \n", i, lY, lN, rY, rN, tconf);
		if(tconf <= min_tconfusion){
			thrd = cp[i].feature;
			min_tconfusion = tconf;
		}
		if(cp[i].label == -1){
			rN -= 1;
			lN += 1;
		}else{
			rY -= 1;
			lY += 1;
		}
		i++;
		
		while(i < (int)cp.size() && cp[i].feature == cp[i - 1].feature){
//printf("data[%d] : %dY%dN %dY%dN, confusion = %lf \n", i, lY, lN, rY, rN, total_confusion(lY, lN, rY, rN));
	
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
		thrd = cp[i-1].feature +1;
		min_tconfusion = tconf;
	}
	idtemp.thrd = thrd;
	idtemp.con = min_tconfusion;
	//get label
	int N = 0, Y = 0;
	for(i = 0; i < (int)cp.size(); i++){
	
			if(cp[i].label == -1)
				N++;
			else
				Y++;

	} 
	if(Y >= N){
		idtemp.label = 1;
	}else{
		idtemp.label = -1;
	}
}

void find_ct(vector<Data*> dataSet, int idNum, vector<Id> &ids, int level){
	//count AYBN
	int tY = 0, tN = 0, i, j;
	for(i = 0; i < (int)dataSet.size(); i++){
		if(dataSet[i]->label == 1){tY++;
		}else{tN++;}
	}
//cout << "check 4\n";
	//get every ids' threshold and total confusion
	int flag = 0; double ftemp;
	
	for(i = 0; i <= idNum; i++){
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
		//cout<<"check p\n";
		getIdtemp(cp, idtemp, tY, tN);
		//cout<<"check b\n" << dataSet.size() << endl;
		
		if(dataSet.size() > 1){
			ftemp = dataSet[0]->array[i];
			for(j = 1; j < (int)dataSet.size(); j++){
				if(ftemp != dataSet[j]->array[i]){
					flag = 1;
				}
			}
		}else{
			flag = 1;
		}
		idtemp.isId = flag;
		
		if(idtemp.isId){
			printf("ID %d, thrd = %lf, confusion = %lf\n", i, idtemp.thrd, idtemp.con);
		}
		
		ids.push_back(idtemp);
	}
}

void printData(vector<Data*> dataSet, int idNum){
	cout << "----------------PrintAllData--------------\n";
	for(int i = 0; i < (int)dataSet.size(); i++){
		cout << dataSet[i]->label << " ";
		for(int j = 0; j <= idNum; j++){
			cout << j << ":"<< dataSet[i]->array[j] << " "; 
		}
		cout << endl;
	}
	cout << "-------------------------------------\n";
}

bool errorData(vector<Data*> dataSet, int idNum){
	int flag = 0;
	for(int i = 0; i < (int)dataSet.size() - 1; i++){
		if(dataSet[i]->label != dataSet[i + 1]->label){
			flag = 1;
			break;
		}
	}
	int idFlag = 1;
	for(int j = 1; j <= idNum; j++){
		for(int i = 0; i < (int)dataSet.size() - 1; i++){
			if(dataSet[i]->array[j] != dataSet[i+1]->array[j]){
				flag = 0;
				break;
			}
		}
	}
	return flag && idFlag;
}

Tree* buildTree(vector<Data*> dataSet, int idNum, int eps, int level){
	if(dataSet.size() == 0){
		return NULL;
	}
printData(dataSet, idNum);	
	int tY = 0, tN = 0, i;
	for(i = 0; i < (int)dataSet.size(); i++){
		if(dataSet[i]->label == 1){tY++;
		}else{tN++;}
	}
	double c = confusion(tY, tN);
	
	Tree* temp;
	temp = new Tree;
	
	printf("Level %d, total Y = %d, total N = %d, confusion = %lf\n", level, tY, tN, c);
	
	if(c <= eps || errorData(dataSet, idNum)){        		         //leaf
		temp->left = NULL;
		temp->right = NULL;
		if(tY >= tN){
			temp->label = 1;
		}else{
			temp->label = -1;
		}

	}else{

	//calculate total confusion and threshold for each feature
	vector<Id> ids;
	find_ct(dataSet, idNum, ids, level);
	
	//find least total confusion
	double ltC = 10000;
	int idGet = 1;
	for(i = 1; i <= idNum; i++){
		if(ids[i].isId && ltC > ids[i].con){
			ltC = ids[i].con;
			idGet = i;
		}
	}
	cout << "Level " << level << ", ID = " << idGet << ", threshold = " << ids[idGet].thrd << endl;

	//cout << "check 3\n";	
	//make the tree
			                        	//internal
		//build two data set
		vector<Data*> ldata, rdata;
		for(i = 0; i < (int)dataSet.size(); i++){
			if(dataSet[i]->array[idGet] >= ids[idGet].thrd){
				rdata.push_back(dataSet[i]);
			}else{
				ldata.push_back(dataSet[i]);
			}
		}
		temp->id = idGet;
		temp->thrd = ids[idGet].thrd;
		temp->left = buildTree(ldata, idNum, eps, level + 1);
		temp->right = buildTree(rdata, idNum, eps, level + 1);		
	}
	return temp;
}

void print_space(fstream &file, int level){
	for(int i = 0; i < level; i++){
		file << "\t";
	}
	return;
}

void printTree(Tree* root, fstream &file, int level){
	print_space(file, level);
	if(root->isLeaf()){
		file << "return " << root->label << ";\n";
		return;
	}else{
		file << "if(attr[" << root->id << "] >= " << root->thrd << "){\n";
		printTree(root->right, file, level + 1);
		print_space(file, level);
		file << "}else{\n";
		printTree(root->left, file, level + 1);
		print_space(file, level);
		file << "}\n";
		return;
	}
}

int main(int argc, char **argv){
	ifstream df;
	df.open(argv[1]);
	if(!df)  {
			cerr << "Can't open file!\n";
			exit(1); 
    }
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
//cout << line << endl;
		char *cstring, *tmp;
		Data* dtemp = new Data;

		cstring = new char[line.size() + 1];
		strncpy(cstring, line.c_str(), line.size()+1);

		tmp =  strtok(cstring, ": ");
		dtemp->label = atoi(tmp);
//cout << tmp << endl;
		tmp = strtok(NULL, ": ");

	
		while(tmp != NULL) {
	
		  int id = atoi(tmp);
//cout << id << " ";
		  if(id > idNum){
			  idNum = id;
		  }
		  tmp = strtok(NULL, ": ");
//cout << tmp << endl;
		  dtemp->array[id] = atof(tmp);
		  tmp = strtok(NULL, ": ");
		}

		delete[] cstring;

		dataSet.push_back(dtemp);

	}
	//cout << idNum <<endl;

	Tree* root;
	//make decision tree
	root = buildTree(dataSet, idNum, epsilon, 0);
	
	//output tree
	fstream file;
	file.open("tree_pred_func.cpp",ios::out);
	file << "int tree_predict(double *attr){\n";
	printTree(root, file, 1);
	file << "}\n";
	//delete root;
	return 0;
}