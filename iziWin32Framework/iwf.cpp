#include "iwf.h"

wchar_t* NazwaKlasy = L"Klasa Okienka";
MSG Komunikat;
HWND hwnd;
HINSTANCE hInstance;
HDC hdc;
PAINTSTRUCT ps;

unsigned long int lastId = 1;
unsigned long int lastIntervalId = 1;

void(*actions[100])();
void(*intervalActions[100])();


const wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	//mbstowcs_s(wc, c, cSize);
	size_t outSize;
	mbstowcs_s(&outSize, wc, cSize, c, cSize + 1);

	return wc;
}

