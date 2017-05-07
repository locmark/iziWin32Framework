#include "iwf.h"
#include "button.h"
#include "textbox.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

// test comment
//
//
//1



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);




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


class drawingArea {
	unsigned int x, y, size_x, size_y;
	Bitmap* static_bmp;
	Bitmap* dynamic_bmp;
	void(*dynamicDraw)();

	void FillStaticBmp() {
		Graphics* graph = Graphics::FromImage(static_bmp);
		SolidBrush* myBrush = new SolidBrush(Color::White);
		graph->FillRectangle(myBrush, 0, 0, size_x, size_y);
	}

public:
	void Init(unsigned int _x, unsigned int _y, unsigned int _size_x, unsigned int _size_y) {
		x = _x;
		y = _y;
		size_x = _size_x;
		size_y = _size_y;

		static_bmp = new Bitmap(size_x, size_y);
		dynamic_bmp = new Bitmap(size_x, size_y);

		FillStaticBmp();
		dynamic_bmp = static_bmp->Clone(0, 0, size_x, size_y, PixelFormatDontCare);
	}

	void StaticContent(void(*function)()) {
		function();
		dynamic_bmp = static_bmp->Clone(0, 0, size_x, size_y, PixelFormatDontCare);
	}

	void DynamicContent(void(*function)()) {
		dynamicDraw = function;
	}

	void Repaint() {
		dynamic_bmp = static_bmp->Clone(0, 0, size_x, size_y, PixelFormatDontCare);
		Rect* rect = new Rect(x, y, size_x, size_y);
		InvalidateRect(hwnd, (RECT*)rect, TRUE);
		dynamicDraw();
		hdc = BeginPaint(hwnd, &ps);
		Graphics graphics(hdc);
		graphics.DrawImage(dynamic_bmp, (int)x, y, size_x, size_y);
		EndPaint(hwnd, &ps);
	}

	Bitmap* GetDynamicBmp() {
		return dynamic_bmp;
	}

	Bitmap* GetStaticBmp() {
		return static_bmp;
	}
};


class window_t {
public:
	int Init(HINSTANCE hInstance_to_save, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
		hInstance = hInstance_to_save;

		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR           gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		// WYPE£NIANIE STRUKTURY
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
			MessageBox(NULL, L"Okno odmówi³o przyjœcia na œwiat!", L"Ale kicha...", MB_ICONEXCLAMATION);
			return 1;
		}

		ShowWindow(hwnd, nCmdShow); // Poka¿ okienko...
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
drawingArea Area;

int position = 0;

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
	Area.Init(0, 0, 400, 400);
	Interval.Init(500);
	GdiInterval.Init(100);

	Area.StaticContent([]()->void {
		Bitmap* static_bmp = Area.GetStaticBmp();
		Graphics* graph = Graphics::FromImage(static_bmp);
		Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 255));
		graph->DrawRectangle(&pen, 0, 0, 100, 100);
	});

	Area.DynamicContent([]()->void {
		Bitmap* dynamic_bmp = Area.GetDynamicBmp();
		Graphics* graph = Graphics::FromImage(dynamic_bmp);
		Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 255));
		graph->DrawRectangle(&pen, position, 10, 200, 200);
	});



	GdiInterval.SetAction([]()->void {
		Area.Repaint();
		position++;
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
