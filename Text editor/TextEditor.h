#pragma once
#include "Utility.h"
#include "Document.h"
#include <vector>

class TextEditor
{
	vector<Document> Doc;
	int currentDoc;
public:
	TextEditor();
	void Run(string fileName);
	void movement(int arrow);
	void PrintShortcuts();
	void start();
	/*
	void CommandMode();
	void MergeDocuments(string Fname1, string Fname2);
	void OpenMultipleDocs(_____);
	*/
};