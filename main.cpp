#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

const wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	//mbstowcs_s(wc, c, cSize);
	size_t outSize;
	mbstowcs_s(&outSize, wc, cSize, c, cSize + 1);

	return wc;
}


class button {
	HWND localHwnd;
	unsigned int _id;
public:
	void Init(char* text, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
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
		//SetText(text);
	}

	void Delete() {
		DestroyWindow(localHwnd);
	}

	void OnClick(void(*function)()) {
		actions[_id] = function;
	}

	void Click() {
		actions[_id]();
	}

	void SetText(std::string text) {
		SetWindowText(localHwnd, (LPCWSTR)text.c_str());
	}
};


class textbox {
	HWND localHwnd;
	unsigned int _id;
public:
	void Init(char* text, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
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

	void Delete() {
		DestroyWindow(localHwnd);
	}

	void OnChange(void(*function)()) {
		actions[_id] = function;
	}

	void Click() {
		actions[_id]();
	}

	void SetText(char* text) {
		SetWindowText(localHwnd, GetWC(text));
	}

	void GetText(char text[], unsigned int size) {
		GetWindowText(localHwnd, (LPWSTR)text, size);
	}
};


class radiobutton {
	HWND localHwnd;
	unsigned int _id;
public:
	void Init(char* text, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
		_id = lastId++;
		localHwnd = CreateWindow(
			TEXT("button"),                      // The class name required is button
			GetWC(text),                  // the caption of the button
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,  // the styles
			x, y,                                  // the left and top co-ordinates
			width, height,                              // width and height
			hwnd,                                 // parent window handle
			(HMENU)_id,                   // the ID of your button
			hInstance,                            // the instance of your application
			NULL);                               // extra bits you dont really need

		OnClick([]()->void {});
	}

	void Delete() {
		DestroyWindow(localHwnd);
	}

	void OnClick(void(*function)()) {
		actions[_id] = function;
	}

	void Click() {
		actions[_id]();
	}

	void SetText(char* text) {
		SetWindowText(localHwnd, GetWC(text));
	}
};


class interval {
	HWND localHwnd;
	unsigned int _id;
	void(*action)();
public:
	void Init(unsigned int interval) {
		_id = lastIntervalId++;
		SetTimer(hwnd, _id, interval, 0);
		SetAction([]()->void {});
	}

	void Delete() {
		DestroyWindow(localHwnd);
	}

	void Stop() {
		KillTimer(hwnd, _id);
	}

	void SetAction(void(*function)()) {
		intervalActions[_id] = function;
	}

	void Action() {
		intervalActions[_id]();
	}

	void SetTime(unsigned int interval) {
		KillTimer(hwnd, _id);
		SetTimer(hwnd, _id, interval, 0);
	}
};


class draw {
public:
	void repaint(RECT *area) {
		InvalidateRect(hwnd, area, TRUE);
	}
};


class window_t {
public:
	int Init(HINSTANCE hInstance_to_save, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
		hInstance = hInstance_to_save;

		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR           gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		// WYPEŁNIANIE STRUKTURY
		WNDCLASSEX wc;

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = 0;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = NazwaKlasy;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		// REJESTROWANIE KLASY OKNA
		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL, L"Wysoka Komisja odmawia rejestracji tego okna!", L"Niestety...",
				MB_ICONEXCLAMATION | MB_OK);
			return 1;
		}

		// TWORZENIE OKNA


		hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, NazwaKlasy, L"Oto okienko", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 800, 480, NULL, NULL, hInstance, NULL);

		if (hwnd == NULL)
		{
			MessageBox(NULL, L"Okno odmówiło przyjścia na świat!", L"Ale kicha...", MB_ICONEXCLAMATION);
			return 1;
		}

		ShowWindow(hwnd, nCmdShow); // Pokaż okienko...
		UpdateWindow(hwnd);
	}

	void Loop() {
		while (GetMessage(&Komunikat, NULL, 0, 0))
		{
			TranslateMessage(&Komunikat);
			DispatchMessage(&Komunikat);
		}
	}

	int Events(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		int wmId, wmEvent;
		switch (msg) {
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			actions[wmId]();
			break;

		case WM_TIMER:
			intervalActions[wParam]();
			break;

		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			EndPaint(hwnd, &ps);
			break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_ERASEBKGND:
			return TRUE;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
};


window_t window;


textbox TextBox;
button resetButton;
button drawButton;
button GdiButton;
radiobutton radio1;
radiobutton radio2;
interval Interval;
interval GdiInterval;



RECT drawArea1 = { 0, 0, 150, 200 };
void test() {

	InvalidateRect(hwnd, NULL, TRUE);
	hdc = BeginPaint(hwnd, &ps);


	Graphics graphics(hdc);
	Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 255));
	graphics.DrawLine(&pen, Point(10, 10), Point(300, 300));
	graphics.DrawRectangle(&pen, 50, 50, 100, 200);

	EndPaint(hwnd, &ps);

}

Bitmap whiteBmp(400, 400);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// *****show console***** //
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	FILE *stream;
	freopen_s(&stream, "CON", "w", stdout);
	// ********************** //
	window.Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	TextBox.Init("test_text", 300, 200, 100, 50);
	resetButton.Init("reset", 200, 200, 100, 50);
	drawButton.Init("draw", 100, 100, 100, 50);
	GdiButton.Init("GDI", 100, 150, 100, 50);
	radio1.Init("radio1", 100, 200, 100, 50);
	radio2.Init("radio2", 100, 250, 100, 50);
	Interval.Init(500);
	GdiInterval.Init(10);

	/*Graphics* graph = Graphics::FromImage(&whiteBmp);
	SolidBrush* myBrush = new SolidBrush(Color::White);
	graph->FillRectangle(myBrush, 0, 0, 300, 300);*/


	GdiInterval.SetAction([]()->void {
		RECT rect = { 0, 0, 100, 100 };
		static int position = 0;
		InvalidateRect(hwnd, NULL, TRUE);
		Bitmap *bmp = whiteBmp.Clone(0, 0, 400, 400, PixelFormatDontCare);
		Graphics* graph = Graphics::FromImage(bmp);
		Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 255));
		//graphics.DrawLine(&pen, Point(10, 10), Point(300, 300));


		graph->DrawRectangle(&pen, (50 + position), 50, 100, 200);
		hdc = BeginPaint(hwnd, &ps);
		Graphics graphics(hdc);
		graphics.DrawImage(bmp, 0, 0, 400, 400);

		EndPaint(hwnd, &ps);
		position++;
	});

	GdiButton.OnClick([]()->void {
		test();
		std::cout << "GdiTest\n";
	});

	drawButton.OnClick([]()->void {


		static int i = 0;
		if (i >= 64) {
			MessageBox(NULL, L"dupa blada", L"Niestety...",
				MB_ICONEXCLAMATION | MB_OK);
		}
		else {
			std::cout << i << "\n";
			button dupaButton;
			dupaButton.Init("dupa", (((i / 8) * 100) + 300) % (8 * 100), (i % 8) * 50, 100, 50);
			i++;
		}
	});

	resetButton.OnClick([]()->void {
		std::cout << "test132\n";
		TextBox.SetText("reset :)");
		resetButton.Delete();
	});

	radio1.OnClick([]()->void {
		Interval.SetAction([]()->void {
			drawButton.Click();
		});
	});

	radio2.OnClick([]()->void {
		Interval.Stop();
	});

	TextBox.OnChange([]()->void {
		char text[1024];
		TextBox.GetText(text, 1024);
		std::cout << text << "\n";
	});

	window.Loop();
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return window.Events(hwnd, msg, wParam, lParam);
}
