#pragma once
//bscs23169
#include "Utility.h"
#include <list>
#include "Line.h"
class Document {
public:
	std::list<Line>::iterator linesIt;
	std::list<char>::iterator charIt;
	list<Line> lines;
	bool saved = false;
	string DName;
	string pass;
	bool passProt = false;
	int cri = 0, cci = SPACE;
	mode Mode = Read;
	Document();
	Document(string fname);
	void Load();
	void Save();
	void push_back(const string& wordStr);
	void print();
	void printLineNum();
	auto getHead();
	auto getTail();
	void setlinesIt();
	void setcharIt();
	void setlinesIt(int n);
	void setcharIt(int n, bool jump = true);
	bool lineEndFront();
	bool charEndFront();
	bool lineEndBack();
	bool charEndBack();
	void insertChar(char ch);
	void removeChar();
	void insertLine();
	void removeLine();
	void toUpper();
	void toLower();
	void findWord(string s, bool sensitive=0);
	void findAll(string s);
	void Replace(string r,string w,bool single=0);
	void PrefixPostfix(string r, string w, bool pre = 0);
	double AvgWordCount();
	int substrCount(string s);
	int SpecialCount();
	int sentenceCount();
	int paraCount();
	int wordLen(bool option);
	void CursorReplaceNext(string r, string w, bool single = 0);
	void CursorReplacePrev(string r, string w, bool single = 0);
	int ParaSentenceCount();
	void setPass(string pas);
	//void UpdaterEncodedFiles();
	//void WordGame(int& cr, int& cc); //Feature no. 16
	//void ExitDocument();
	//void DeleteAllLines();
//bscs23169
};
