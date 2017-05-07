#include "button.h"


void button::Init(char* text, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
	_id = lastId++;
	localHwnd = CreateWindow(
		TEXT("button"),                      // The class name required is button
		GetWC(text),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		x, y,                                  // the left and top co-ordinates
		width, height,                              // width and height
		hwnd,                                 // parent window handle
		(HMENU)_id,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	OnClick([]()->void {});
}

void button::Delete() {
	DestroyWindow(localHwnd);
}

void button::OnClick(void(*function)()) {
	actions[_id] = function;
}

void button::Click() {
	actions[_id]();
}

void button::SetText(std::string text) {
	SetWindowText(localHwnd, (LPCWSTR)text.c_str());
}