#include "textbox.h"



void textbox::Init(char* text, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
	_id = lastId++;
	localHwnd = CreateWindow(
		TEXT("edit"),                      // The class name required is button
		GetWC(text),                  // the caption of the button
		WS_BORDER | ES_MULTILINE | WS_CHILD | WS_VISIBLE,  // the styles
		x, y,                                  // the left and top co-ordinates
		width, height,                              // width and height
		hwnd,                                 // parent window handle
		(HMENU)_id,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	OnChange([]()->void {});
}

void textbox::Delete() {
	DestroyWindow(localHwnd);
}

void textbox::OnChange(void(*function)()) {
	actions[_id] = function;
}

void textbox::Click() {
	actions[_id]();
}

void textbox::SetText(char* text) {
	SetWindowText(localHwnd, GetWC(text));
}

void textbox::GetText(char text[], unsigned int size) {
	GetWindowText(localHwnd, (LPWSTR)text, size);
}
