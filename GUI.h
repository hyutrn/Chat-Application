#include <windows.h>
#include <iostream>
#include <string.h>
#include <cstdarg>
#include <cstring>
#include <iomanip>
#include <conio.h>
#include <vector>

void setColor(int color);
void gotoXY(int x, int y);
void drawRectangle(int left, int top, int right, int bottom);
void hideCursor(bool isHiden);
int getTerminalWidth();
int getTerminalHeight();
int centerWindow(int width);
int centerBox(int boxWidth, int width);
void loadingEffect(std::string text);
void notiBox(std::string text);
int optionMenu(int height, std::string chatLabel, std::string menuLabel, int numberMenu, ...);
void clearBuffer(int length);
