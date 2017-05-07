#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <objidl.h>
#include <gdiplus.h>
#include "iwf.h"

class textbox {
	HWND localHwnd;
	unsigned int _id;
public:
	void Init(char* text, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	void Delete();
	void OnChange(void(*function)());
	void Click();
	void SetText(char* text);
	void GetText(char text[], unsigned int size);
};
