#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <assert.h>
#include <string.h>
#include "data.h"
#define MAX_USER 23907634
//#define MAX_USER 34895628


//enum {get, clicked, impressed, profit, quit};
enum {GET, CLICKED, IMPRESSED, PROFIT, QUIT};



typedef struct datas{
	int Click, Impression, AdID, AdvertiserID, Depth, Position, QueryID,
		KeywordID, TitleID, DescriptionID, UserID;
	char DisplayURL[21];
} Datas;

using namespace std;

int ReadCommends(){
	char optr[5][10] = {"get", "clicked", "impressed", "profit", "quit"};
	char temp[10];
	scanf("%s", temp);
	for(int i = 0; i < 5; i++){
		if(strcmp(optr[i], temp) == 0){
			//printf("%s  %d\n", optr[i], i);
			return i;
		}
	}
	return -1;
}

void processGET(USER *userArray){
	int u, a, q, p, d;
	scanf("%d%d%d%d%d", &u, &a, &q, &p, &d);
	//printf("%d %d %d %d %d\n", u, a, q, p, d);
	userArray[u].get(a, q, p, d);
}

void processCLICKED(USER *userArray){
	int u;
	scanf("%d", &u);
	//cout << "user " << u << endl;
	userArray[u].oneClick();
}

void processPROFIT(USER *userArray){
	int a;
	double rateIN;
	cin >> a >> rateIN;
//	cout << a << " " << rateIN << endl;
	int i;
	vector<int> userOK;
	for(i = 0; i < MAX_USER; i++){
		if(userArray[i].checkRate(a, rateIN)){
			userOK.push_back(i);
		}
	}
	
	sort(userOK.begin(), userOK.end());
	
	cout << "********************" << endl;
	for(i = 0; i < (int)userOK.size(); i++){
		cout << userOK[i] << endl;
	}
	cout << "********************" << endl;
}

vector<int> conjunction(vector<int> &AdIDarray1, vector<int> &AdIDarray2){
	vector<int> temp;
	int i = 0, j = 0;
	while((i < (int)AdIDarray1.size()) && (j < (int)AdIDarray2.size())){
		if(AdIDarray1[i] < AdIDarray2[j]){
			i++;
		}else if(AdIDarray1[i] > AdIDarray2[j]){
			j++;
		}else{
			if(temp.size() == 0 || AdIDarray1[i] != temp.back()){
				temp.push_back(AdIDarray1[i]);
			}
			i++; j++;
		}
	}
	return temp;
}

int dummycmp(Dummy d1, Dummy d2){
	if(strcmp(d1.DisplayURL, d2.DisplayURL) < 0)
		return true;
	else if(strcmp(d1.DisplayURL, d2.DisplayURL) > 0)
		return false;
	else if(d1.AdvertiserID < d2.AdvertiserID)
		return true;
	else if(d1.AdvertiserID > d2.AdvertiserID)
		return false;
	else if(d1.KeywordID < d2.KeywordID)
		return true;
	else if(d1.KeywordID > d2.KeywordID)
		return false;
	else if(d1.TitleID < d2.TitleID)
		return true;
	else if(d1.TitleID > d2.TitleID)
		return false;
	else if(d1.DescriptionID < d2.DescriptionID)
		return true;
	else if(d1.DescriptionID > d2.DescriptionID)
		return false;
	
	return 10;
}

void printDummy(Dummy &d){
	printf("\t%s %d %d %d %d\n", 
	d.DisplayURL, d.AdvertiserID, d.KeywordID, d.TitleID, d.DescriptionID);
	return;
}

bool sameDummy(Dummy &d1, Dummy &d2){
	if(strcmp(d1.DisplayURL, d2.DisplayURL) != 0)
		return false;
	else if(d1.AdvertiserID != d2.AdvertiserID)
		return false;
	else if(d1.KeywordID != d2.KeywordID)
		return false;
	else if(d1.TitleID != d2.TitleID)
		return false;
	else if(d1.DescriptionID != d2.DescriptionID)
		return false;
	return true;
}

void conjDummy(vector<Dummy> &d1, vector<Dummy> &d2){
	int count = 0, i = 0, j = 0;
	vector<Dummy> temp;
	cout << "%%%%%%%%%%%%%%%%%%%" << endl;
	while(i < (int)d1.size() && j < (int)d2.size()){
		if(sameDummy(d1[i], d2[j])){
			if(temp.size() == 0 || !sameDummy(temp.back(), d1[i])){
				count++;
				temp.push_back(d1[i]);
				printDummy(d1[i]);
			}
			i++; j++;
		}else if(dummycmp(d1[i], d2[j]) == true){
			i++;
		}else if(dummycmp(d1[i], d2[j]) == false){
			j++;
		}
	}
	cout << "The same : " << count << endl;
	cout << "%%%%%%%%%%%%%%%%%%%" << endl;
	return ;
}

void printdisjunctionDummy(vector<Dummy> &d1, vector<Dummy> &d2){
	int i = 0, j = 0;
	vector<Dummy> t;
	while(i < (int)d1.size() || j < (int)d2.size()){
		if(i < (int)d1.size() && j < (int)d2.size()){
			if(sameDummy(d1[i], d2[j])){
				if(t.size() == 0 || !sameDummy(t.back(), d1[i])){
					printDummy(d1[i]);
					t.push_back(d1[i]);
				}
				i++; j++;
			}else if(dummycmp(d1[i], d2[j])){
				if(t.size() == 0 || !sameDummy(t.back(), d1[i])){
					printDummy(d1[i]);
					t.push_back(d1[i]);
				}
				i++;
			}else if(dummycmp(d1[i], d2[j]) == false){
				if(t.size() == 0 || !sameDummy(t.back(), d2[j])){
					printDummy(d2[j]);
					t.push_back(d2[j]);
				}
				j++;
			}
		}else if(i < (int)d1.size() && j >= (int)d2.size()){
			if(t.size() == 0 || !sameDummy(t.back(), d1[i])){
				printDummy(d1[i]);
				t.push_back(d1[i]);
			}
			i++;
		}else if(i >= (int)d1.size() && j < (int)d2.size()){
			if(t.size() == 0 || !sameDummy(t.back(), d2[j])){
				printDummy(d2[j]);
				t.push_back(d2[j]);
			}
			j++;
		}
	}
	return;
}
	
void printSortedDummy(vector<Dummy> &d){
	cout << "--------------------" << endl;
	vector<Dummy> temp;
	for(int i = 0; i < (int)d.size(); i++){
		if(temp.size() == 0 || !sameDummy(temp.back(), d[i])){
			printDummy(d[i]);
		}
	}
	cout << "--------------------" << endl;
}	

void processIMPRESSED(USER *userArray){
	int u1, u2;
	cin >> u1 >> u2;
	vector<int> AdIDarray1;
	vector<int> AdIDarray2;
	vector<int> matchAdID;

	userArray[u1].impressedAdID(AdIDarray1);
	userArray[u2].impressedAdID(AdIDarray2);

	sort(AdIDarray1.begin(), AdIDarray1.end());
	sort(AdIDarray2.begin(), AdIDarray2.end());

	matchAdID = conjunction(AdIDarray1, AdIDarray2);
	
//cout << matchAdID.size() << endl;
//cout << "check after sort" << endl;
	vector<Dummy> dummyArray1;
	vector<Dummy> dummyArray2;
	
	cout << "********************" << endl;
	for(int i = 0; i < (int)matchAdID.size(); i++){
	
	cout << matchAdID[i] << endl;
		userArray[u1].printdummy(dummyArray1, matchAdID[i]);
		userArray[u2].printdummy(dummyArray2, matchAdID[i]);
		sort(dummyArray1.begin(), dummyArray1.end(), dummycmp);
//printSortedDummy(dummyArray1);

		sort(dummyArray2.begin(), dummyArray2.end(), dummycmp);
//printSortedDummy(dummyArray2);

//cout << "dummy1 : " << dummyArray1.size() << endl;
//cout << "dummy2 : " << dummyArray2.size() << endl;
//conjDummy(dummyArray1,dummyArray2);
		
		printdisjunctionDummy(dummyArray1,dummyArray2);
		dummyArray1.clear();
		dummyArray2.clear();
	}
	cout << "********************" << endl;
	return;
}

int main(int argc, char **argv){
	FILE *fp;
	fp = fopen("/tmp2/KDDCup2012/track2/kddcup2012track2.txt", "r");
	//fp = fopen("/nfs/undergrad/01/b01502069/.DSA/HW2/demotrack.txt", "r");
	
	//fp = fopen(argv[1], "r");
	assert(fp != NULL);

	USER *userArray = new USER[MAX_USER];
	Datas temp; Data dtemp;
	int n = 0;

	while(fscanf(fp, "%d%d%s%d%d%d%d%d%d%d%d%d", &temp.Click, &temp.Impression, temp.DisplayURL, &temp.AdID,
			&temp.AdvertiserID, &temp.Depth, &temp.Position, &temp.QueryID,	&temp.KeywordID, &temp.TitleID,
			&temp.DescriptionID, &temp.UserID) != EOF){
	
		dtemp.setData(temp.Click, temp.Impression, temp.AdID, temp.AdvertiserID, temp.Depth, temp.Position, temp.QueryID, 
		temp.KeywordID, temp.TitleID, temp.DescriptionID, temp.DisplayURL);
					
		userArray[temp.UserID].insert(dtemp);
		
		n++;
	}
	
	fclose(fp);
//	cout << n << endl;
//	cout << "Done Reading" << endl;

	//operate input line
	int commendTemp;
	while((commendTemp = ReadCommends()) != QUIT){
		switch(commendTemp){
			case GET:
				processGET(userArray);
				break;
			case CLICKED:
				processCLICKED(userArray);
				break;
			case IMPRESSED:
				processIMPRESSED(userArray);
				break;
			case PROFIT:
				processPROFIT(userArray);
				break;
			case QUIT:
				//cout << "quit" << endl;
				break;
		}
	}
	
	return 0;
}

/*
total 149639105

以下為 kddcup2012track2.txt 每個資料的最大值
Click = 39900
Impression = 764404
DisplayURL = 18445899849997222722
AdID = 22238277
AdvertiserID = 39191
Depth = 3
Position = 3
QueryID = 26243605
KeywordID = 1249783
TitleID = 4051439
DescriptionID = 3171828
UserID = 23907634

想請問一下:
hw2.pdf裡關於porfit的部分有寫: "Note that 0/0 is defined as 0."
所以如果profit的theta給0，就連impression為0的user也要考慮
亦即要印出所有的UserID嗎?

要adid交集，property 聯集

 同場加映最小值：
Click = 0
Impression = 1
DisplayURL = 482436910553333
AdID = 1000031
AdvertiserID = 78
Depth = 1
Position = 1
QueryID = 0
KeywordID = 0
TitleID = 0
DescriptionID = 0
UserID = 0
*/