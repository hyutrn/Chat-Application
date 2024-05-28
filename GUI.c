#include "GUI.h"

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoXY(int x, int y)
{
    COORD coordinate;
	coordinate.X = x;
	coordinate.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

void drawRectangle(int left, int top, int width, int height)
{
    gotoXY(left, top);
	std::cout << char(201);
	for (int i = 0; i < width; ++i) {
		std::cout << char(205);
	}
	std::cout << char(187);

	for (int i = 0; i <= height; ++i) {
		gotoXY(left, top + i + 1);
		std::cout << char(186);
	}	
	gotoXY(left, top + height + 1);
	std::cout << char(200);
	for (int i = 0; i < width; ++i) {
		std::cout << char(205);
	}
	std::cout << char(188);
	for (int i = 0; i <= height-1; ++i) {
		gotoXY(left+width+1, top +i+1);
		std::cout << char(186);
	}
}

void hideCursor(bool isHiden)
{
    CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	cursor.bVisible = !isHiden;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

int getTerminalWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwSize.X;
}

int getTerminalHeight()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwSize.Y;
}

int centerWindow(int width)
{
    int terminalWidth = (getTerminalWidth() / 2);
	int terminalHeight = getTerminalHeight() / 2;
	int left = terminalWidth - (width / 2);
	return left;
}

int centerBox(int boxWidth, int width)
{
    int terminalWidth = (getTerminalWidth() / 2);
	int left = terminalWidth - (boxWidth / 2);
	int windowWidth = terminalWidth - (width / 2);
	return windowWidth;
}

void loadingEffect(std::string text)
{
    system("cls");
	int width = 46;
	int height = 5;
	int top = 7;
	int left = centerWindow(width);
	char keyPressed;
	drawRectangle(left, top, width, height);
	do {
		gotoXY(left + 2, top + 1);
		std::cout << text;
		for (int i = 0; i < 44; i++) {
			Sleep(10);
			gotoXY(left + 2 + i, top + 2);
			std::cout << char(220);
		}
		gotoXY(left + 2, top + 4);
		std::cout << "Set up finished ! Press 'r' to continue...";
		if (_kbhit()) {
			keyPressed = _getch();
			if (keyPressed == 'r' || keyPressed == 'R') {
				system("cls");
				break;
			}
		}
	} while (true);
}

void notiBox(std::string text)
{
    system("cls");
	setColor(7);
	hideCursor(true);
	int width = text.length() + 26 + 20;
	int height = 10;
	int top = 7;
	int left = centerWindow(width);
	int leftBox = centerBox(width, text.length()+26);
	char keyPressed;
	drawRectangle(left, top, width, height);
	do {
		gotoXY(leftBox, top + 2);
		std::cout << text + "! Press 'r' to continue...";
		if (_kbhit()) {
			keyPressed = _getch();
			if (keyPressed == 'r' || keyPressed == 'R') {
				system("cls");
				break;
			}
		}
	} while (true);
}

int findMaxString(std::vector<std::string> vector) {
	int max = 0;
	for (int i = 0; i < vector.size(); i++) {
		if (vector[i].length() > max) {
			max = vector[i].length();
		}
	}
	return max;
}

int optionMenu(int height, std::string chatLabel, std::string menuLabel, int numberMenu,...)
{
    system("cls");
    hideCursor(true);
	std::vector<int> initMenuColor;
	std::vector<std::string> menuList;
	va_list args;
	int counter = 1;	
	char key;
	va_start(args, numberMenu);

	for (int i = 0; i < numberMenu; i++) {
		char* str = va_arg(args, char*);
		std::string menuItem(str);
		menuList.push_back(menuItem);
	}

	va_end(args);
	/*initMenuColor.resize(menuList.size());*/

	for (int i = 0; i < numberMenu + 1; i++) {
		initMenuColor.push_back(7);
	}

	int alignLeft = findMaxString(menuList);
	int width = 46;
	int top = 7;
	int left = centerWindow(width);
	drawRectangle(left, top, width, height);

	while (true) {
		int geti = 0;
		setColor(7);
		gotoXY(centerWindow(chatLabel.length() - 1), 3);
		std::cout << chatLabel;
		gotoXY(centerWindow(menuLabel.length() - 1), 5);
		std::cout << menuLabel;

		for (int i = 0; i < menuList.size(); i++) {
			gotoXY(centerWindow(alignLeft - 1), top + 3 + i);
			setColor(initMenuColor[i]);
			std::cout << menuList[i];
			geti = i;
		}
		
		gotoXY(centerWindow(alignLeft - 1), top + 3 + geti + 2);
		setColor(initMenuColor[numberMenu]);
		std::cout << "Exit";

		key = _getch();

		if (key == 'w' && (counter >= 2 && counter <= numberMenu + 1)) {
			counter--;
		}

		if (key == 's' && (counter >= 1 && counter <= numberMenu)) {
			counter++;
		}

		if (key == '\r') {
			return counter;
			break;
		}

		for (int i = 0; i < initMenuColor.size(); i++) {
			initMenuColor[i] = 7;
		}
		
		for (int i = 0; i < initMenuColor.size(); i++) {
			if (counter == i + 1) {
				initMenuColor[i] = 3;
				break;
			}
		}
	}
}

void clearBuffer(int length)
{
    std::cout << "\r";
    for (size_t i = 0; i < length; ++i) {
        std::cout << " ";
    }
}
