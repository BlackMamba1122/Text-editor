#pragma once
//bscs23169
#include "Utility.h"
#include <list>
class Line
{
public:
	list<char>line;
	Line();
	Line(bool _isPara);
	Line(const Line& A,bool _isPara);
	void push_back(const char& word);
	void push_back(const string& wordStr);
	void save(ofstream& file);
	void save(ofstream& file,string pass);
	void InsertCharAt(std::list<char>::iterator loc, char c);
	void CharRemoverAt(std::list<char>::iterator loc);
	void printWord();
	std::list<char>::iterator getHead();
	std::list<char>::iterator getTail();
	int size();
	void joinline(list<char> list2);
	void seprateline(list<char>&list2, std::list<char>::iterator loc);
	void ToUpper(std::list<char>::iterator wordIt);
	void ToLower(std::list<char>::iterator wordIt);
	void findAll(string s);
	void findWord(string s,bool sensitive=0);
	bool replace(string r,string w,bool single=0);
	void PrefixPostfix(string r,string w,bool pre=0);
	int WordCount();
	int substrCount(string s);
	int SpecialCount();
	int CharCount();
	int SentenceCount();
	void WordLen(int& l, int& s);
	bool CursorReplaceNext(string r,string w,bool first, std::list<char>::iterator temp2, bool single = 0);
	bool CursorReplacePrev(string r,string w,bool first, std::list<char>::iterator temp2, bool single = 0);
};
