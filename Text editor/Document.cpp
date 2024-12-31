#include "Document.h"
#
Document::Document() : DName("") {}

Document::Document(string fname) : DName(fname)	{}


void Document::Load() {
    ifstream file(DName);
    bool fileEmpty = true;
    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        return;
    }
    string lineStr;
    bool mode=true;
    bool pas=true;
    while (getline(file, lineStr)) {
        if (mode)
        {
            if (lineStr == "Protected")
                passProt = true;
            else
                passProt = false;
            mode = false;
            continue;
        }
        if (pas)
        {
            if (passProt)
            {
                pass = lineStr;
                do
                {
                    system("cls");
                    gotorowcol(0, 0);
                    cout << "Enter Password";
                    getline(cin, lineStr);
                    if (lineStr != xorEncryptDecrypt(pass, "1234"))
                        cout << "Wrong Pass";
                } while (lineStr != xorEncryptDecrypt(pass, "1234"));
                pass = lineStr;
                pas = false;
                continue;
            }
            else
                pas = false;
        }
        fileEmpty = false;
        Line line;
        if (passProt)
            lineStr = xorEncryptDecrypt(lineStr,pass);
        for (char c : lineStr) {
            line.push_back(c);
        }
        lines.push_back(line);
    }
    if (fileEmpty)
        lines.push_back(Line());
    file.close();
}

void Document::Save() {
    ofstream file(DName);
    if (!file.is_open()) {
        cerr << "Could not open the file for saving!" << endl;
        return;
    }
    if(passProt)
    {
        file << "Protected\n";
        file << xorEncryptDecrypt(pass, "1234") << endl;
    }
    else
        file << "NotProtected\n";

    if(!passProt)
        for (auto line : lines) {
            line.save(file);
            file << '\n';
        }
    else
        for (auto line : lines) {
            line.save(file,pass);
            file << '\n';
        }
    file.close();
}



void Document::push_back(const string& wordStr)
{
    Line newLine;
    newLine.push_back(wordStr);
    lines.push_back(newLine);
}

void Document::print()
{
    system("cls");
    saved = false;
    int t = 0;
    for (auto l : lines)
    {
        gotorowcol(t++, SPACE);
        l.printWord();
        cout << endl;
    }
    printLineNum();
}

void Document::printLineNum()
{
    int t = 0;
    auto l = lines.begin();
    while (l != lines.end())
    {
        if (l == linesIt)
        {
            gotorowcol(t, 0);
            cout << "    ";
            gotorowcol(t++, 2);
            SetClr(YELLOW, BLACK);
            cout << t;
            cout << endl;
        }
        else
        {
            gotorowcol(t, 0);
            cout << "     ";
            gotorowcol(t++, 1);
            SetClr(BLUE, BLACK);
            cout << t;
            cout << endl;
        }
        ++l;
    }
    SetClr(WHITE, BLACK);
}

auto Document::getHead()
{
    return lines.begin();
}

auto Document::getTail()
{
    return lines.end();
}

void Document::setlinesIt()
{
    linesIt = getHead();
}

void Document::setcharIt()
{
    charIt = linesIt->getHead();
}

void Document::setlinesIt(int n)
{
    if (n > 0)
        for (int i = 0; i < n; i++)
        {
            if (lineEndBack())
                break;
            --linesIt;
            gotorowcol(--cri, cci);
            charIt = linesIt->getHead();
            int temp = cci;
            temp -= SPACE;
            if (linesIt->size() == 0)
                temp = 0;
            temp *= -1;
            cci = SPACE;
            setcharIt(temp,0);
        }
    else
        for (int i = n; i < 0; i++)
        {
            if (lineEndFront())
                break;
            ++linesIt;
            gotorowcol(++cri, cci);
            charIt = linesIt->getHead();
            int temp = cci;
            temp -= SPACE;
            if (linesIt->size() == 0)
                temp = 0;
            temp *= -1;
            cci = SPACE;
            setcharIt(temp,0);
        }
    printLineNum();
}

void Document::setcharIt(int n , bool jump)
{
    if (n > 0)
        for (int i = 0; i < n; i++)
        {
            if (charEndBack() && jump)
            {
                if (linesIt != lines.begin())
                {
                    --linesIt;
                    setcharIt();
                    cri--;
                    cci = SPACE;
                    setcharIt(-linesIt->size());
                    printLineNum();
                }
                break;
            }
            --charIt;
            gotorowcol(cri, --cci);
            if (charEndBack())
                break;
        }
    else
        for (int i = n; i < 0; i++)
        {
            if (charEndFront() && jump)
            {
                if (linesIt != std::prev(lines.end()))
                {
                    ++linesIt;
                    cri++;
                    cci = SPACE;
                    setcharIt();
                    printLineNum();
                }
                break;
            }
            ++charIt;
            gotorowcol(cri, ++cci);
            if (charEndFront())
                break;
        }
            gotorowcol(cri, cci);
}

bool Document::lineEndBack()
{
    if (linesIt == getHead())
        return true;
    else
        return false;
}

bool Document::charEndBack()
{
    if (charIt == linesIt->getHead())
        return true;
    else
        return false;
}
bool Document::lineEndFront()
{
    if (std::next(linesIt) == getTail())
        return true;
    else
        return false;
}

bool Document::charEndFront()
{
    if (charIt == linesIt->getTail())
        return true;
    else
        return false;
}

void Document::insertChar(char ch)
{
    if (cci > 30)
        insertLine();
    linesIt->InsertCharAt(charIt, ch);
    cci++;
}

void Document::removeChar()
{
    if (charIt != linesIt->getHead())
    {
        linesIt->CharRemoverAt(charIt);
        cci--;
    }
    else if (linesIt != lines.begin())
    {
        removeLine();
        cri--;
    }
}

void Document::insertLine()
{
    Line newLine;
    if (charIt != linesIt->getTail())
        linesIt->seprateline(newLine.line, charIt);
    lines.insert(std::next(linesIt),newLine);
    ++linesIt;
    setcharIt();
    cri++;
    cci = SPACE;
}

void Document::removeLine()
{
    auto temp=linesIt;
    linesIt--;
    int size = linesIt->size();
    linesIt->joinline(temp->line);
    lines.erase(temp);
    setcharIt();
    setcharIt(-size);
}

void Document::toUpper()
{
    if (linesIt->getHead() == charIt)
        linesIt->ToUpper(charIt);
    else
        linesIt->ToUpper(std::prev(charIt));
}
void Document::toLower()
{
    if(linesIt->getHead() == charIt)
        linesIt->ToLower(charIt);
    else
        linesIt->ToLower(std::prev(charIt));

}
void Document::findWord(string s,bool sensitive)
{
    int t = 0;
    auto it = lines.begin();
    while (it != lines.end())
    {
        gotorowcol(t, SPACE);
        it->findWord(s,sensitive);
        cout << endl;
        ++it;
    }
}
void Document::findAll(string s)
{
    int t = 0;
    auto it = lines.begin();
    while (it != lines.end())
    {
        gotorowcol(t++, SPACE);
        it->findAll(s);
        cout << endl;
        ++it;
    }
}

void Document::Replace(string r, string w,bool single)
{
    auto it = lines.begin();
    while (it != lines.end())
    {
        bool found=it->replace(r,w,single);
        ++it;
        if (single && found)
            break;
    }
    setcharIt();
    int temp = cci;
    temp -= SPACE;

    cci = SPACE;
    setcharIt(-temp);
}

void Document::PrefixPostfix(string r, string w, bool pre)
{
    auto it = lines.begin();
    while (it != lines.end())
    {
        it->PrefixPostfix(r, w, pre);
        ++it;
    }
}

double Document::AvgWordCount()
{
    int charCount=0, WordCount=0;
    auto it = lines.begin();
    while (it != lines.end())
    {
        WordCount+= it->WordCount();
        charCount+= it->CharCount();
        ++it;
    }
    return static_cast<double>(charCount) / WordCount;
}

int Document::substrCount(string s)
{
    int count = 0;
    auto it = lines.begin();
    while (it != lines.end())
    {
        count+=it->substrCount(s);
        ++it;
    }
    return count;
}

int Document::SpecialCount()
{
    int count = 0;
    auto it = lines.begin();
    while (it != lines.end())
    {
        count += it->SpecialCount();
        ++it;
    }
    return count;
}

int Document::sentenceCount()
{
    int count = 0;
    auto it = lines.begin();
    while (it != lines.end())
    {
        count += it->SentenceCount();
        ++it;
    }
    return count;
}

int Document::paraCount()
{
    int count = 0;
    auto it = lines.begin();
    if (it->size() > 0)
        count++;
    while (it != prev(lines.end()))
    {
        if (it->size() == 0 && next(it)->size() > 0)
            count++;
        ++it;
    }
    return count;

}

int Document::wordLen(bool option)
{
    int l=0, s=INT_MAX;
    auto it = lines.begin();
    while (it != lines.end())
    {
        it->WordLen(l, s);
        ++it;
    }
    if (s == INT_MAX) {
        s = 0;
    }
    if (option)
        return l;
    else
        return s;
}

void Document::CursorReplaceNext(string r, string w, bool single)
{
    auto it = linesIt;
    bool first = true;
    while (it != lines.end())
    {
        bool found = it->CursorReplaceNext(r, w,first,charIt,single);
        ++it;
        first = false;
        if (single && found)
            break;
    }
    setcharIt();
    int temp = cci;
    temp -= SPACE;
    cci = SPACE;
    setcharIt(-temp);
}

void Document::CursorReplacePrev(string r, string w, bool single)
{
    auto it = linesIt;
    bool first = true;
    std::reverse(r.begin(), r.end());
    std::reverse(w.begin(), w.end());
    while (1)
    {
        bool found = it->CursorReplacePrev(r, w, first, charIt, single);
        first = false;
        if (single && found)
            break;
        if (it == lines.begin())
            break;
        --it;
    }
    setcharIt();
    int temp = cci;
    temp -= SPACE;
    cci = SPACE;
    setcharIt(-temp);
}

int Document::ParaSentenceCount()
{
    int WordCount = 0;
    int max = 0;
    auto it = lines.begin();
    while (it != lines.end())
    {
        WordCount += it->WordCount();
        if (it->size() == 0)
        {
            if (max < WordCount)
                max = WordCount;
            WordCount = 0;
        }
        ++it;
    }
    if (max < WordCount)
        max = WordCount;
    return max;

}

void Document::setPass(string pas)
{
    passProt = true;
    pass = pas;
}
