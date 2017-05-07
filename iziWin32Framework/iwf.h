#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <objidl.h>
#include <gdiplus.h>



extern wchar_t* NazwaKlasy;
extern MSG Komunikat;
extern HWND hwnd;
extern HINSTANCE hInstance;
extern HDC hdc;
extern PAINTSTRUCT ps;

extern unsigned long int lastId;
extern unsigned long int lastIntervalId;

extern void(*actions[100])();
extern void(*intervalActions[100])();


const wchar_t *GetWC(const char *c);
