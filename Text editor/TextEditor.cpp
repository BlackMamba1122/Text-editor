#include "TextEditor.h"
#include <conio.h>
#include <iomanip>
TextEditor::TextEditor()
{
	currentDoc = 0;
    Doc.reserve(100);
}

void TextEditor::Run(string fileName)
{
    //setFixedConsoleSize(65,40,L"Consolas",20);
	Doc.emplace_back(fileName);
    DisableConsoleShortcutKeys();
    Doc[currentDoc].Load();
    Doc[currentDoc].setlinesIt();
    Doc[currentDoc].setcharIt();
    Doc[currentDoc].print();
    gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int lastRow = csbi.srWindow.Bottom;
    auto PrintInsert = [&]() {
    SetClr(WHITE,CYAN);
        gotorowcol(lastRow, 0);
        std::cout << "INSERT";
        gotorowcol(lastRow, 20);
        cout << Doc[currentDoc].cri+1 <<" : " << Doc[currentDoc].cci+1-SPACE;
        SetClr(WHITE, BLACK);
        gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
        };
    PrintInsert();
    bool HelpMenuEnable = false;
    while (1)
    {
        DisableConsoleShortcutKeys();
        if (_kbhit())
        {
            char ch = _getch();
            if (HelpMenuEnable)
            {
                ch = 0;
                HelpMenuEnable = false;
                bool temp = Doc[currentDoc].saved;
                Doc[currentDoc].print();
                Doc[currentDoc].saved = temp;
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            bool isCtrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
            bool isAltPressed = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
            bool isF12Pressed = (GetAsyncKeyState(VK_F12) & 0x8000) != 0;
            if (ch == 1)
            { //esc
                break;
            }
            else if (ch == -32 && !isF12Pressed)
            { // arrow keys
                char arrowKey = _getch();
                switch (arrowKey)
                {
                case 72: // up
                    movement(1);
                    break;
                case 80: // down
                    movement(2);
                    break;
                case 75: // left
                    movement(3);
                    break;
                case 77: // right
                    movement(4);
                    break;
                default:
                    break;
                }
            }
            else if (ch >= 32 && ch <= 126 && !isAltPressed) // printable
            {
                Doc[currentDoc].insertChar(ch);
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && ch == 8 && !isAltPressed) // backspace
            {
                Doc[currentDoc].removeChar();
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && ch == 13 && !isAltPressed) // enter
            {
                Doc[currentDoc].insertLine();
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (isCtrlPressed && ch == 21 && !isAltPressed) // ctrl+u   //uppercase
            {
                Doc[currentDoc].toUpper();
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (isCtrlPressed && ch == 12 && !isAltPressed) // ctrl+l   //lowercase
            {
                Doc[currentDoc].toLower();
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (isCtrlPressed && ch == 8 && !isAltPressed)  //ctrl+h    //help
            {
                HelpMenuEnable = true;
                PrintShortcuts();
            }
            else if (isCtrlPressed && ch == 6 && !isAltPressed)  //ctrl+f    //case insensi find
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                string s;
                gotorowcol(lastRow, 30);
                cin >> s;
                Doc[currentDoc].findWord(s,1);
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 102 || ch == 70) && isAltPressed)  //alt+f   //case sensi find
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                string s;
                gotorowcol(lastRow, 30);
                cin >> s;
                Doc[currentDoc].findWord(s);
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && ch == 7 && isAltPressed)  //ctrl+g     // sentence find
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                string s;
                gotorowcol(lastRow, 30);
                getline(cin, s);
                Doc[currentDoc].findAll(s);
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 103 || ch == 71) && isAltPressed)  //alt+g         // substr find
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                string s;
                gotorowcol(lastRow, 30);
                cin >> s;
                Doc[currentDoc].findAll(s);
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (isCtrlPressed && ch == 18 && !isAltPressed)  //ctrl+r         // repla
            {
                EnableConsoleShortcutKeys();
                string r,w;
                gotorowcol(lastRow, 30);
                getline(cin, r);
                gotorowcol(lastRow, 40);
                getline(cin, w);
                Doc[currentDoc].Replace(r, w,1);
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 114 || ch == 82) && isAltPressed)  //alt+r         // repla
            {
                EnableConsoleShortcutKeys();
                string r, w;
                gotorowcol(lastRow, 30);
                getline(cin, r);
                gotorowcol(lastRow, 40);
                getline(cin, w);
                Doc[currentDoc].Replace(r, w);
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }

            else if (!isCtrlPressed && (ch == 112 || ch == 80) && isAltPressed)  //alt+p         // pre
            {
                EnableConsoleShortcutKeys();
                string r, w;
                gotorowcol(lastRow, 30);
                cin >> r;
                gotorowcol(lastRow, 40);
                cin >> w;
                if (isAltPressed)
                    Doc[currentDoc].PrefixPostfix(r, w);
                else
                    Doc[currentDoc].PrefixPostfix(r, w, 1);
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (isCtrlPressed && ch == 16 && !isAltPressed)  //ctrl+p         // post
            {
                EnableConsoleShortcutKeys();
                string r, w;
                gotorowcol(lastRow, 30);
                cin >> r;
                gotorowcol(lastRow, 40);
                cin >> w;
                if (isAltPressed)
                    Doc[currentDoc].PrefixPostfix(r, w);
                else
                    Doc[currentDoc].PrefixPostfix(r, w, 1);
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (isCtrlPressed && ch == 7 && !isAltPressed)  //ctrl+g        // avg word len
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                double count = 0;
                gotorowcol(lastRow, 30);
                count = Doc[currentDoc].AvgWordCount();
                gotorowcol(lastRow, 60);
                cout << "Count: " <<std::setprecision(2)<<std::fixed<< count;
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);

            }
            else if (!isCtrlPressed && (ch == 100 || ch == 68) && isAltPressed)  //alt+d         // substr count
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                string s;
                int count = 0;
                gotorowcol(lastRow, 30);
                cin >> s;
                count=Doc[currentDoc].substrCount(s);
                gotorowcol(lastRow, 60);
                cout << "Count: " << count;
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
                
            }
            else if (!isCtrlPressed && (ch == 99 || ch == 67) && isAltPressed)  //alt+c         // special count
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                int count = 0;
                count=Doc[currentDoc].SpecialCount();
                gotorowcol(lastRow, 60);
                cout << "Count: " << count;
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 44 || ch == 60) && isAltPressed)  //alt+<         // senten count
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                int count = 0;
                count = Doc[currentDoc].sentenceCount();
                gotorowcol(lastRow, 60);
                cout << "Count: " << count;
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 46 || ch == 62) && isAltPressed)  //alt+>         // para count
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                int count = 0;
                count = Doc[currentDoc].paraCount();
                gotorowcol(lastRow, 60);
                cout << "Count: " << count;
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 76 || ch == 108) && isAltPressed)  //alt+l         // count largest word
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                int count = 0;
                count = Doc[currentDoc].wordLen(1);
                gotorowcol(lastRow, 60);
                cout << "Count: " << count;
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 83 || ch == 115) && isAltPressed)  //alt+s         // count smal word
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                int count = 0;
                count = Doc[currentDoc].wordLen(0);
                gotorowcol(lastRow, 60);
                cout << "Count: " << count;
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 59 || ch == 58) && isAltPressed)  //alt+;         // repla cursor    // next all
            {
                EnableConsoleShortcutKeys();
                string r, w;
                gotorowcol(lastRow, 30);
                getline(cin, r);
                gotorowcol(lastRow, 40);
                getline(cin, w);
                Doc[currentDoc].CursorReplaceNext(r, w, 1);
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 39 || ch == 34) && isAltPressed)  //alt+'         // repla cursor    // next one
            {
                EnableConsoleShortcutKeys();
                string r, w;
                gotorowcol(lastRow, 30);
                getline(cin, r);
                gotorowcol(lastRow, 40);
                getline(cin, w);
                Doc[currentDoc].CursorReplaceNext(r, w,0);
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 91 || ch == 123) && isAltPressed)  //alt+[         // repla cursor    // prev one
            {
                EnableConsoleShortcutKeys();
                string r, w;
                gotorowcol(lastRow, 30);
                getline(cin, r);
                gotorowcol(lastRow, 40);
                getline(cin, w);
                Doc[currentDoc].CursorReplacePrev(r, w, 1);
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 93 || ch == 125) && isAltPressed)  //alt+]         // repla cursor    // prev all
            {
                EnableConsoleShortcutKeys();
                string r, w;
                gotorowcol(lastRow, 30);
                getline(cin, r);
                gotorowcol(lastRow, 40);
                getline(cin, w);
                Doc[currentDoc].CursorReplacePrev(r, w, 0);
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (!isCtrlPressed && (ch == 75 || ch == 107) && isAltPressed)  //alt+k         // para largest sentence count
            {
                EnableConsoleShortcutKeys();
                HelpMenuEnable = true;
                int count = 0;
                gotorowcol(lastRow, 30);
                count = Doc[currentDoc].ParaSentenceCount();
                gotorowcol(lastRow, 60);
                cout << "Count: " << std::setprecision(2) << std::fixed << count;
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (isCtrlPressed && ch == 19 && !isAltPressed)  //ctrl+s         // save
            {
                Doc[currentDoc].Save();
            }
            else if (isF12Pressed)  //f12
            {
                EnableConsoleShortcutKeys();
                string oldP, newP;
                if (Doc[currentDoc].passProt)
                {
                    gotorowcol(lastRow, 30);
                    cout << "OldP: ";
                    getline(cin, oldP);
                    if ((Doc[currentDoc].pass != oldP))
                    {
                        Doc[currentDoc].print();
                        gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
                    }

                }
                gotorowcol(lastRow, 45);
                cout << "Pass: ";
                getline(cin, newP);
                Doc[currentDoc].setPass(newP);
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (isCtrlPressed && ch == 15 && !isAltPressed)  //ctrl+o         // open
            {
                system("cls");
                cout << "Write name of the file and \n Click to Open\n      1. New \n      2. Existing \n      3. Already Opened \n";
                string fileName;
                char choice = 0;
                while (1)
                {
                    choice = 0;
                    while (choice != '1' && choice != '2' && choice != '3') {
                        choice = _getch();
                    }
                    if (choice == '1' || choice == '2')
                    {
                        cin >> fileName;
                        fileName += ".mydc";
                    }
                    std::ifstream file(fileName);
                    if (choice == '1' && file.good()) {
                        cout << "File Name already exist\n";
                        continue;
                    }
                    else if (choice == '1' && !file.good())
                    {
                        ofstream newfile(fileName);
                        Doc.emplace_back(fileName);
                        currentDoc = Doc.size() - 1;
                        Doc[currentDoc].Load();
                        Doc[currentDoc].setlinesIt();
                        Doc[currentDoc].setcharIt();
                        newfile.close();
                    }
                    else if (choice == '2' && !file.good()) {
                        cout << "File doesn't exist\n";
                        continue;
                    }
                    else if (choice == '2' && file.good()) {
                        Doc.emplace_back(fileName);
                        currentDoc = Doc.size() - 1;
                        Doc[currentDoc].Load();
                        Doc[currentDoc].setlinesIt();
                        Doc[currentDoc].setcharIt();
                    }
                    else if (choice == '3')
                    {
                        cout << "Enter No. of file to load\n";
                        int dcnum = 0;
                        do
                        {
                            for (int i = 0; i < Doc.size(); i++)
                            {
                                cout << "   " << i + 1 << ".  " << Doc[i].DName << endl;
                            }
                            cin >> dcnum;
                        } while (dcnum < 1 || dcnum > Doc.size());
                        currentDoc = dcnum-1;
                    }
                    file.close();
                    break;
                }
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
            else if (isCtrlPressed && ch == 13 && !isAltPressed)  //ctrl+m         // merge
            {
                system("cls");
                cout << "Click to Chose Merge\n      1. New \n      2. Existing \n";
                string fileName;
                char choice = 0;
                while (1)
                {
                    choice = 0;
                    while (choice != '1' && choice != '2') {
                        choice = _getch();
                    }
                    if (choice == '1')
                    {
                        cin >> fileName;
                        fileName += ".mydc";
                    }
                    std::ifstream file(fileName);
                    if (choice == '1' && file.good()) {
                        cout << "File Name already exist\n";
                        continue;
                    }
                    else if (choice == '1' && !file.good())
                    {
                        ofstream newfile(fileName);
                        Doc.emplace_back(fileName);
                        currentDoc = Doc.size() - 1;
                        newfile.close();
                    }
                    int c = 1;
                    if (choice == '1')
                        c++;
                    int num=0;
                    cout << "Enter No. of file to Merge\n";
                    do
                    {
                        for (int i = 0; i < Doc.size(); i++)
                        {
                            cout << "   " << i + 1 << ".  " << Doc[i].DName << endl;
                        }
                        cin >> num;
                        if (num > 0 && num <= Doc.size())
                        {
                            if (choice == '1')
                            {
                                Doc[Doc.size()-1].lines.splice(Doc[Doc.size() - 1].lines.end(), Doc[num-1].lines);
                                currentDoc = Doc.size() - 1;
                            }
                            else
                                Doc[currentDoc].lines.splice(Doc[currentDoc].lines.end(), Doc[num - 1].lines);
                            c--;
                        }
                    } while (c!=0);
                    file.close();
                    break;
                }
                Doc[currentDoc].print();
                gotorowcol(Doc[currentDoc].cri, Doc[currentDoc].cci);
            }
    if(!HelpMenuEnable)
        PrintInsert();
        }
    }
}

void TextEditor::movement(int arrow)
{
    switch (arrow)
    {
    case 1: // up
        Doc[currentDoc].setlinesIt(1);
        break;
    case 2: // down
        Doc[currentDoc].setlinesIt(-1);
        break;
    case 3: // left
        Doc[currentDoc].setcharIt(1);
        break;
    case 4: // right
        Doc[currentDoc].setcharIt(-1);
        break;
    default:
        break;
    }
}

void TextEditor::PrintShortcuts() {
    system("cls");
    std::cout << "==== Shortcut Key Instructions ====" << std::endl;

    // Finding Features
    std::cout << "Ctrl + F             : Find a word (Case Sensitive) // Key: 6\n";
    std::cout << "Alt + F              : Find a word (Case Insensitive) // Key: N/A\n";
    std::cout << "Ctrl + G             : Find a sentence // Key: N/A\n";
    std::cout << "Alt + G              : Find a substring // Key: 4\n";

    // Replace Features
    std::cout << "Ctrl + R             : Replace the first word only // Key: N/A\n";
    std::cout << "Alt + R              : Replace the whole file // Key: N/A\n";

    // Upper Lower Features
    std::cout << "Ctrl + U             : Upper Case // Key: N/A\n";
    std::cout << "Ctrl + L             : Lower Case // Key: N/A\n";

    // Add Prefix/Postfix
    std::cout << "Alt + P              : Add Prefix to a word // Key: 16\n";
    std::cout << "Ctrl + P             : Add Postfix to a word // Key: N/A\n";

    // Average Word Length
    std::cout << "Ctrl + G             : Find average word length // Key: 7\n";

    // Substring Count
    std::cout << "Alt + D              : Find substring count // Key: N/A\n";

    // Special Character Count
    std::cout << "Alt + C              : Find special character count // Key: N/A\n";
    // Sentence and Paragraph Counts
    std::cout << "Alt + <              : Find number of sentences // Key: 19\n";
    std::cout << "Alt + >              : Find paragraph count // Key: N/A\n";

    // Word Length
    std::cout << "Alt + L              : Find largest word length // Key: 12\n";
    std::cout << "Alt + S              : Find smallest word length // Key: N/A\n";

    // Word Game
    std::cout << "Ctrl + W             : Find word for maximum points // Key: N/A\n";

    // Cursor-Specific Replace
    std::cout << "Alt + ;              : Replace next match // Key: N/A\n";
    std::cout << "Alt + '              : Replace all next matches // Key: N/A\n";
    std::cout << "Alt + [              : Replace previous match // Key: N/A\n";
    std::cout << "Alt + ]              : Replace all previous matches // Key: N/A\n";

    // Paragraph Counts
    std::cout << "Alt + K              : Find largest paragraph word length // Key: N/A\n";
    std::cout << "Ctrl + S             : Save\n";
    std::cout << "F12                  : Password Protected\n";
    std::cout << "Ctrl + O             : Open File\n";
   // ------------------------------------------------------------------------------------------------------------
    std::cout << "Ctrl + M             : Merge File\n";

    std::cout << "===================================" << std::endl;
}
void TextEditor::start()
{
    const char* t = R"===(
    .__   __.   ______   .___________. _______   ______   ______   .__   __.              
    |  \ |  |  /  __  \  |           ||   ____| /      | /  __  \  |  \ |  |    _     _   
    |   \|  | |  |  |  | `---|  |----`|  |__   |  ,----'|  |  |  | |   \|  |  _| |_ _| |_ 
    |  . `  | |  |  |  |     |  |     |   __|  |  |     |  |  |  | |  . `  | |_   _|_   _|
    |  |\   | |  `--'  |     |  |     |  |____ |  `----.|  `--'  | |  |\   |   |_|   |_|  
    |__| \__|  \______/      |__|     |_______| \______| \______/  |__| \__|              
     )===";
    cout << t << endl;
    cout << "Write name of the file and \n Click to Open\n      1. New \n      2. Existing\n";
    string fileName;
    char choice=0;
    while (1)
    {
        cin >> fileName;
        fileName += ".mydc";
        choice = 0;
        while (choice != '1' && choice != '2') {
            choice = _getch();
        }
        std::ifstream file(fileName);
        if (choice == '1' && file.good()) {
            cout << "File Name already exist\n";
            continue;
        }
        else if(choice == '1' && !file.good())
        {
            ofstream newfile(fileName);
            newfile.close();
        }
        else if (choice == '2' && !file.good()) {
            cout << "File doesn't exist\n";
            continue;
        }
        file.close();
        break;
    }
    Run(fileName);
}