#include "Utility.h"
void gotorowcol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOutput, scrn);
}

void SetClr(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}

bool getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.EventType == MOUSE_EVENT) // Check if it's a mouse event
		{
			if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
				rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				return false; // Left click detected
			}
			else if (InputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				return true; // Right click detected
			}
		}
	} while (true);
}

void DisableConsoleShortcutKeys() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;

	if (GetConsoleMode(hStdin, &mode)) {
		mode &= ~(ENABLE_QUICK_EDIT_MODE | ENABLE_INSERT_MODE | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
		SetConsoleMode(hStdin, mode);
	}
}
void EnableConsoleShortcutKeys() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;

	if (GetConsoleMode(hStdin, &mode)) {
		mode |= (ENABLE_QUICK_EDIT_MODE | ENABLE_INSERT_MODE | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
		SetConsoleMode(hStdin, mode);
	}
}
void getCursorPosition(int& x, int& y) {
	// Get the console handle
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Declare a CONSOLE_SCREEN_BUFFER_INFO structure
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

	// Get console screen buffer information
	if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) {
		// Extract the cursor position
		x = consoleInfo.dwCursorPosition.X;
		y = consoleInfo.dwCursorPosition.Y;
	}
	else {
		// Handle errors (optional)
		std::cerr << "Error retrieving console cursor position!" << std::endl;
		x = y = -1; // Invalid position
	}
}
void setFixedConsoleSize(int width, int height, const wchar_t* fontName, int fontSize) {
	// Get the console handle
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND hWndConsole = GetConsoleWindow();

	// Set font size and font name
	CONSOLE_FONT_INFOEX fontInfo = { 0 };
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize.X = 0;             // Font width (auto-adjusted)
	fontInfo.dwFontSize.Y = fontSize;     // Font height
	fontInfo.FontFamily = FF_DONTCARE;    // Font family
	fontInfo.FontWeight = FW_NORMAL;      // Font weight
	wcscpy_s(fontInfo.FaceName, fontName); // Font name
	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	// Set console buffer size to match the desired width and height
	COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
	SetConsoleScreenBufferSize(hConsole, bufferSize);

	// Set the console window size (outer part)
	SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	// Prevent resizing of the console window
	LONG style = GetWindowLong(hWndConsole, GWL_STYLE);
	SetWindowLong(hWndConsole, GWL_STYLE, style & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);

	// Adjust the console window to fit the buffer size
	RECT rect;
	GetWindowRect(hWndConsole, &rect);  // Get current window size
	MoveWindow(hWndConsole, rect.left, rect.top,
		(width * fontSize) + 16, // Width (includes borders)
		(height * fontSize) + 39, // Height (includes title bar)
		TRUE);
}
string xorEncryptDecrypt(const string& input, const string& key) {
	string result = input;
	for (size_t i = 0; i < input.size(); ++i) {
		result[i] ^= key[i % key.size()]; // XOR each character with the key
	}
	return result;
}