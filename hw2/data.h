#ifndef DATA

#define DATA

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#define MAX_StrLen 25

using namespace std;


typedef struct aqid{
	int AdID, QueryID;
} AQID;

typedef struct dummy{
	int AdvertiserID, KeywordID, TitleID, DescriptionID;
	unsigned long long DisplayURL;
} Dummy;

/*
class Dummy{
	public:
		int AdvertiserID, KeywordID, TitleID, DescriptionID;
		char *DisplayURL;
		Dummy(){
			DisplayURL = new chae[MAX_StrLen];
		};
		~Dummy(){
			delete [] DisplayURL;
		};
		Dummy(Dummy &d){
			AdvertiserID = d.AdvertiserID;
			KeywordID = d.KeywordID;
			TitleID = d.TitleID;
			DescriptionID = d.DescriptionID;
			strcpy(DisplayURL, d.DisplayURL);
		}
};
*/
class Data{
private:
	int Click, Impression, AdID, AdvertiserID, Depth, Position, QueryID,
		KeywordID, TitleID, DescriptionID;
	unsigned long long DisplayURL;
public:
	Data(){};
	void setData(int Click, int Impression, int AdID, int AdvertiserID, int Depth, int Position, int QueryID,
		int KeywordID, int TitleID, int DescriptionID, unsigned long long DisplayURL){
		this->Click = Click; this->Impression = Impression; this->AdID = AdID; this->AdvertiserID = AdvertiserID;
		this->Depth = Depth; this->Position = Position; this->Position = Position; this->QueryID = QueryID;
		this->KeywordID = KeywordID; this->TitleID = TitleID; this->DescriptionID = DescriptionID;
		this->DisplayURL  = DisplayURL;	
	};
	unsigned long long getDisplayURL(){
		return DisplayURL;
	}
	int getClick(){
		return Click;
	}
	int getImpression(){
		return Impression;
	}
	int getAdID(){
		return AdID;
	}
	int getQueryID(){
		return QueryID;
	}
	int getKeywordID(){
		return KeywordID;
	}
	int getTitleID(){
		return TitleID;
	}
	int getDescriptionID(){
		return DescriptionID;
	}
	int getAdvertiserID(){
		return AdvertiserID;
	}
	bool match(int a, int q, int p, int d){
		if(AdID == a && QueryID == q && Position == p && Depth == d) return true;
		else return false;
	}

	bool checkRate(int a, double rateIN){
		if(this->AdID != a){
			return false;
		}
		double rate = this->Click / (double)this->Impression;
//cout << rate << endl;
		if(rate < rateIN)
			return false;	
		return true;
	};
	
	bool isAdID(int a){
		return (a == this->AdID);
	}
	
	void impressedAdID(vector<int> &AdIDarray){
		if(this->Impression > 0){
			AdIDarray.push_back(this->AdID);
		}
		return;
	};
		
};

int compare(AQID ptr1, AQID ptr2){
		if(ptr1.AdID < ptr2.AdID)
			return true;
		else if(ptr1.AdID > ptr2.AdID)
			return false;
		else if(ptr1.QueryID < ptr2.QueryID)
			return true;
		else if(ptr1.QueryID > ptr2.QueryID)
			return false;
		return false;
};

class USER{
private:
	std::vector<Data> dptr;
	std::vector<Data>:: iterator it;
	double CTR; //click-through-rate (total click / total impression)
public:
	USER(){};
	void insert(Data d){
		dptr.push_back(d);
	}
	void get(int a, int q, int p, int d){
		int tClick = 0, tImperssion = 0, i;
		//cout << "for user i :" << dptr.size() << endl;
		for(i = 0; i < (int)dptr.size(); i++){
			if(dptr[i].match(a, q, p, d)){
				tClick += dptr[i].getClick();
				tImperssion += dptr[i].getImpression();
			}
		}
		cout << "********************" << endl;
		cout << tClick << " " << tImperssion << endl;
		cout << "********************" << endl;
	};
	
	void printADID(vector<AQID> aptr){
		vector<AQID>::iterator it ;
		cout << "********************" << endl;
		for(it = aptr.begin(); it != aptr.end(); it++){
			cout << it->AdID << " " << it->QueryID << endl;
		}
		cout << "********************" << endl;
	};

	void oneClick(){
		vector<AQID> aptr;
		int i; AQID temp;
		for(i = 0; i < (int)dptr.size(); i++){
			if(dptr[i].getClick() > 0){
				//cout << i << " ";
				temp.AdID = dptr[i].getAdID();
				temp.QueryID = dptr[i].getQueryID();
				aptr.push_back(temp);
			}
		}
		sort(aptr.begin(), aptr.end(), compare);
		printADID(aptr);
	};
/*
	bool checkRate(int a, double rateIN){
		int i;
		for(i = 0; i < (int)dptr.size(); i++){
			if(dptr[i].checkRate(a, rateIN))
				return true;
		}
		return false;	
	};
*/

	bool checkRate(int a, double rateIN){
		int i; int tClick = 0, tImperssion = 0;
		for(i = 0; i < (int)dptr.size(); i++){
			if(dptr[i].getAdID() == a){
				tClick += dptr[i].getClick();
				tImperssion += dptr[i].getImpression();
			}
		}
		
		double rate = tClick / (double)tImperssion;
		if(rate >= rateIN){
			return true;
		}
		return false;	
	};
	

	void impressedAdID(vector<int> &AdIDarray){
//cout << "data unmbers for user : " << dptr.size() << endl;
		for(int i = 0; i < (int)dptr.size(); i++){
			dptr[i].impressedAdID(AdIDarray);
		}
//cout << "check 2" << endl;
	};
	
	void printdummy(vector<Dummy> &dummyArray, int AdID){
		Dummy temp;
		for(int i = 0; i < (int)dptr.size(); i++){
			if(dptr[i].getAdID() == AdID){
				temp.DisplayURL = dptr[i].getDisplayURL();
				temp.AdvertiserID = dptr[i].getAdvertiserID();
				temp.KeywordID = dptr[i].getKeywordID();
				temp.TitleID = dptr[i].getTitleID();
				temp.DescriptionID = dptr[i].getDescriptionID();
			
				dummyArray.push_back(temp);
			}
		}
	};
};

#endif

