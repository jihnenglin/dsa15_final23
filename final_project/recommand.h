#include <iostream>
#include <string>
#include <algorithm>

bool match_wild(const std::string& str, const std::string& wild);

class Rank {

public:
	Rank() : _size(0) {}
	
	int  update(const std::string &newstr, int nscore); //return the pos of new string 0~10
	int  size() const {return _size;} //current size of rank
	void print(); //print by rank
	void clear(); //reset size
	static const int maxnum = 10;
	
private:
	std::string buffer[maxnum];
	int score[maxnum];
	int rank[maxnum];	
	int _size;
};

class Recommand {
	
public:
	Recommand(bool (*func) (const std::string&)) : valid(func) {};
	void getRank(Rank&, std::string& origin);
	
private:	
	static const int MAXLENGTH = 100;
	bool unlock[MAXLENGTH];
	bool (*valid) (const std::string&); //return true if target string is vaild
	std::string* origin;
	Rank* rank;
	int len,minlen;
	int score;
	
	void plus(char &, char);
	void setUnlock(int, int);
	void testSet();
};

void Rank::clear() {
	for(int i=0; i<maxnum; i++)
		buffer[i].clear();
	_size = 0;
}

int Rank::update(const std::string &newstr, int newscore)
{
	//update stroage array
	if(_size<maxnum) {
		score[_size]  = newscore;
		buffer[_size] = newstr;
		rank[_size]   = _size;
		_size++;
	}
	else {
		int last = rank[maxnum-1];
		if(newscore>score[last])
			return maxnum;
		else if(newscore==score[last] && newstr.compare(buffer[last])>0)
			return maxnum;
		else { //replace the last string
			buffer[last] = newstr;
			score[last]  = newscore;			
		}	
	}

	//update rank
	int i = _size-1;
	while(i>0) {		
		if(score[rank[i-1]]>score[rank[i]] ||
		   buffer[rank[i-1]].compare(buffer[rank[i]])<0)
			break;
		std::swap(rank[i-1],rank[i]);
		i--;
	}
	return i;
}

void Rank::print()//[ordered best ids]
{
	if(_size<1) return;
	std::cout<<buffer[rank[0]];
	for(int i=1; i<_size; i++)
		std::cout<<','<<buffer[rank[i]];
	std::cout<<std::endl;
}


void Recommand::plus(char & c, char e) {
	do {
		if(c=='9')
			c = 'A';
		else if(c=='Z')
			c = 'a';
		else
			++c;
	}while(c==e);
}

void Recommand::getRank(Rank& r, std::string& o) {
	
	rank   = &r;
	origin = &o;
	
	int sum = 0, delta = 1, diff;
	unsigned int i;
	int start_len, end_len;
	start_len = end_len = origin->length();	

	score = 1;
	while(true) {
		
		if(score >= sum + delta ) {
			sum += delta;
			if(start_len>1)
				start_len--;
			if(end_len < MAXLENGTH)
				end_len++;
			delta++;		
		}
		diff = (origin->length()-start_len)*(origin->length()-start_len+1)/2;
		for(len=start_len; len<(int)origin->length(); len++) {
			
			minlen = len;
			for(i=0; i<MAXLENGTH; i++)
				unlock[i] = i>=origin->length();
			setUnlock(len,score-diff);
			diff += len-origin->length();
		}
		minlen = origin->length();
		for(;len<=end_len;len++) {
			diff += len-origin->length();
			for(i=0; i<MAXLENGTH; i++)
				unlock[i] = i>=origin->length();
			setUnlock(len,score-diff);
		}
		if(rank->size()==rank->maxnum) return;
		score++;	
	}
}

void Recommand::setUnlock(int max, int point) {
	if(max>0 && max*(max+1)/2 >= point ) // max>0 && max*(max+1)/2 >= point
		if(point<max)
			setUnlock(point,point);
		else {
			unlock[minlen-max] = true;
			setUnlock(max-1,point-max);
			unlock[minlen-max] = false;
			setUnlock(max-1,point);			
		}
	else if(point==0) {
		testSet();
	}
}

void Recommand::testSet() {
	std::string str;
	str.resize(len);
	unsigned int i;
	for(i=0; i<(unsigned int)len; i++) {
		if(!unlock[i])
			str[i] = (*origin)[i];
		else
			str[i] = i<origin->length() && (*origin)[i]=='0' ? '1' : '0' ;
	}
	
	unsigned int start;
	
	start = len-1;
	while(!unlock[start])
		if((start--)==0) { // all lock
			if(valid(str)) {
				//std::cout<<str<<std::endl;			
				rank->update(str,score);
			}
			return;
		}
	
	while(true) {
		if(valid(str)) {
			//std::cout<<str<<std::endl;			
			if(rank->update(str,score) >= rank->maxnum)
				return;	//no need for further testing in this set
		}
		i = start;
		plus(str[i],i<origin->length() ? (*origin)[i] : '\0');
		
		while(str[i] > 'z') {
			str[i] = i<origin->length() && (*origin)[i]=='0' ? '1' : '0' ;
			do
				if(i<=0) return;
			while(!unlock[--i]);
			plus(str[i],i<origin->length() ? (*origin)[i] : '\0');
		}	
	}
}

bool match_wild(const std::string& str, const std::string& wild) {
	char *p = (char*)str.c_str(); //postfix 
	char *s = (char*)wild.c_str(); //states
	
	char *stack[256];
	int i = 0;
	
	while(1) {
		//printf("p:[%c] s:[%c]\n",*p,*s);
		if(*p==0) {
			if(*s==0||*s=='*')
				return true;
		}
		else if(*s!=0) {
			if(*s=='*') {
				stack[i++] = p+1;
				stack[i++] = s;
				s = s+1;
				continue;
			}			
			else if(*s=='?'){
				p++;s++;
				continue;
			}
			
			else if(*p==*s) {
				p++;s++;
				continue;
			}
		}
		if(i<=0)
			return false;
		s = stack[--i];
		p = stack[--i];
	}
}