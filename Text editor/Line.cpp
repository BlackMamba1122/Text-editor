#include "Line.h"

Line::Line() : line()		{}

Line::Line(bool _isPara) : line()	{}

Line::Line(const Line& A,bool _isPara): line(A.line)	{}

void Line::push_back(const char& word)
{
	line.push_back(word);
}
void Line::push_back(const string& wordStr)
{
	for (char c : wordStr) {
		line.push_back(c);
	}
}

void Line::save(ofstream& file)
{
	for (const char& c : line) {
		file << c;
	}
}

void Line::save(ofstream& file, string pass)
{
	string s;
	for (const char& c : line) {
		s += c;
	}
		file << xorEncryptDecrypt(s,pass);

}

void Line::InsertCharAt(std::list<char>::iterator loc, char c)
{
	line.insert(loc, c);
}

void Line::CharRemoverAt(std::list<char>::iterator loc)
{
	line.erase(std::prev(loc));
}

void Line::printWord()
{
	for (char c : line)
		cout << c;
}

std::list<char>::iterator Line::getHead()
{
	return line.begin();
}

std::list<char>::iterator Line::getTail()
{
	return line.end();
}

int Line::size()
{
	return line.size();
}

void Line::joinline(list<char> list2)
{
	line.splice(line.end(), list2);
}

void Line::seprateline(list<char>& list2, std::list<char>::iterator loc)
{
	list2.splice(list2.begin(), line, loc, line.end());
}

void Line::ToUpper(std::list<char>::iterator wordIt)
{
	auto temp = wordIt;
	while (*temp != ' ' )
	{
		*temp = std::toupper(*temp);
		if (temp == line.begin())
			break;
		temp--;
	}
	temp = wordIt;
	temp++;
	while (temp != line.end() && *temp != ' ')
	{
		*temp = std::toupper(*temp);
		temp++;
	}
}

void Line::ToLower(std::list<char>::iterator wordIt)
{
	auto temp = wordIt;
	while (*temp != ' ')
	{
		*temp = std::tolower(*temp);
		if (temp == line.begin())
			break;
		temp--;

	}
	temp = wordIt;
	temp++;
	while (temp != line.end() && *temp != ' ')
	{
		*temp = std::tolower(*temp);
		temp++;
	}
}

void Line::findAll(string s)
{
	auto it = line.begin();
	auto temp = it;
	while (it != line.end())
	{
		bool dont = true;
		auto tit = it;
		auto sit = s.begin();
		while (tit != line.end() && sit != s.end() && *tit == *sit) {
			++tit;
			++sit;
		}
		if (sit == s.end())
		{
			for (auto tempIt = temp; tempIt != it; ++tempIt) {
				std::cout << *tempIt;
			}
				SetClr(0, 14); // Black text on yellow background
			for (size_t i = 0; i < s.size(); ++i) {
				std::cout << *it;
				++it;
				temp = it;
				dont = false;
			}
				SetClr(15, 0); // White text on black background
		}
		if (it != line.end() && dont)
			++it;
	}
}
void Line::findWord(string s,bool sensitive)
{
	bool found = true;
	auto it = line.begin();
	auto temp = it;
	while (it != line.end())
	{
		bool dont = true;
		auto tit = it;
		auto sit = s.begin();
		if (!found && *prev(it) == ' ')
			found = true;
		if (sensitive)
		{
			while (tit != line.end() && sit != s.end() && toupper(*tit) == toupper(*sit) && found) {
				++tit;
				++sit;
			}
		}
		else
		{
			while (tit != line.end() && sit != s.end() && *tit == *sit && found) {
				++tit;
				++sit;
			}
		}
		found = false;
		if (sit == s.end())
		{
			if (!(tit == line.end() || *((tit)) == ' '))
			{
				if (it != line.end())
					++it;
				continue;
			}
			for (auto tempIt = temp; tempIt != it; ++tempIt) {
				std::cout << *tempIt;
			}
			SetClr(0, 14); // Black text on yellow background
			for (size_t i = 0; i < s.size(); ++i) {
				std::cout << *it;
				++it;
				temp = it;
				dont = false;
			}
			SetClr(15, 0); // White text on black background
		}
		if(it != line.end() && dont)
			++it;
	}
}
bool Line::replace(string r,string w,bool single)
{
	auto it = line.begin();
	auto temp = it;
	while (it != line.end())
	{
		auto tit = it;
		auto sit = r.begin();
		while (tit != line.end() && sit != r.end() && *tit == *sit) {
			++tit;
			++sit;
		}
		if (sit == r.end())
		{
			it = line.erase(it, std::next(it, r.size()));

			it = line.insert(it, w.begin(), w.end());
			if (single)
				return true;
		}
		if (it != line.end())
			++it;
	}
	return false;
}

void Line::PrefixPostfix(string r, string w, bool pre)
{
	bool found = true;
	auto it = line.begin();
	auto temp = it;
	while (it != line.end())
	{
		auto tit = it;
		auto sit = r.begin();
		if (!found && *prev(it) == ' ')
			found = true;
		while (tit != line.end() && sit != r.end() && *tit == *sit && found) {
			++tit;
			++sit;
		}
		found = false;
		if (sit == r.end())
		{
			if (!(tit == line.end() || *((tit)) == ' '))
			{
				if (it != line.end())
					++it;
				continue;
			}
			if(!pre)
				it = line.insert(it, w.begin(), w.end());
			else
				tit = line.insert(tit, w.begin(), w.end());
		}
		if (it != line.end())
			++it;
	}
}

int Line::WordCount() {
	int count = 0;
	bool inWord = false;

	for (char c : line) {
		if (std::isalnum(c)) {
			if (!inWord) {
				inWord = true;
				count++;
			}
		}
		else 
			inWord = false;
	}
	return count;
}


int Line::substrCount(string s)
{
	auto it = line.begin();
	auto temp = it;
	int count = 0;
	while (it != line.end())
	{
		auto tit = it;
		auto sit = s.begin();
		while (tit != line.end() && sit != s.end() && *tit == *sit) {
			++tit;
			++sit;
		}
		if (sit == s.end())
			count++;
		if (it != line.end())
			++it;
	}
	return count;
}

int Line::SpecialCount()
{
	int count = 0;
	for (char c : line)
		if (!std::isalnum(c))
			count++;
	return count;
}
int Line::CharCount()
{
	int count = 0;
	for (char c : line)
		if (std::isalnum(c))
			count++;
	return count;
}

int Line::SentenceCount()
{
	int count = 0;
	for (auto c : line)
	{
		if (c == '.' || c == '!' || c == '?')
			count++;
	}
	return count;
}

void Line::WordLen(int& l, int& s) {
	int currentLength = 0;
	bool inWord = false;

	for (char c : line) {
		if (std::isalnum(c)) {
			currentLength++;
			inWord = true;
		}
		else {
			if (inWord) {
				if (currentLength > l) {
					l = currentLength;
				}
				if (currentLength < s) {
					s = currentLength;
				}
				currentLength = 0;
			}
			inWord = false;
		}
	}
	if (inWord) {
		if (currentLength > l) {
			l = currentLength;
		}
		if (currentLength < s) {
			s = currentLength;
		}
	}
}

bool Line::CursorReplaceNext(string r, string w, bool first, std::list<char>::iterator temp2, bool single)
{
	std::list<char>::iterator it;
	if (!first)
		it = line.begin();
	else
		it = temp2;
	auto temp = it;
	while (it != line.end())
	{
		auto tit = it;
		auto sit = r.begin();
		while (tit != line.end() && sit != r.end() && *tit == *sit) {
			++tit;
			++sit;
		}
		if (sit == r.end())
		{
			it = line.erase(it, std::next(it, r.size()));

			it = line.insert(it, w.begin(), w.end());
			if (single)
				return true;
		}
		if (it != line.end())
			++it;
	}
	return false;
}

bool Line::CursorReplacePrev(string r, string w, bool first, std::list<char>::iterator temp2, bool single)
{
	std::list<char>::iterator it;

	if (!first)
		it = std::prev(line.end());
	else
		it = temp2;

	while (true)
	{
		auto tit = it;
		auto sit = r.begin();

		while (sit != r.end() && *tit == *sit)
		{
			++sit;
			if (tit == line.begin())
				break;
			--tit;
		}

		if (sit == r.end())
		{
			auto erase_start = tit;
			auto erase_end = it;

			it = line.erase(next(erase_start), next(erase_end));

			it = line.insert(it, w.begin(), w.end());

			if (single)
				return true;
		}

		if (it == line.begin())
			break;

		--it;
	}

	return false;
}