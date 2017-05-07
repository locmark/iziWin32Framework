#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <objidl.h>
#include <gdiplus.h>
#include "iwf.h"


class button {
	HWND localHwnd;
	unsigned int _id;
public:
	void Init(char* text, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	void Delete();
	void OnClick(void(*function)());
	void Click();
	void SetText(std::string text);
};