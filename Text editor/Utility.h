#pragma once
#include<conio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<windows.h>
using namespace std;
#define SPACE 5
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGREY 7
#define DARKGREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15
#define BLINK 1286
enum mode
{
	Read,
	Insert
};
void gotorowcol(int rpos, int cpos);
void SetClr(int tcl, int bcl);
bool getRowColbyLeftClick(int& rpos, int& cpos);
void DisableConsoleShortcutKeys();
void EnableConsoleShortcutKeys();
void getCursorPosition(int& x, int& y);
void setFixedConsoleSize(int width, int height, const wchar_t* fontName, int fontSize);
string xorEncryptDecrypt(const string& input, const string& key);