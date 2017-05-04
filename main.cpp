#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
// #include <Unknwn.h>
// #include <gdiplus.h>
// #include <objidl.h>
//
//
// using namespace Gdiplus;
//
// #pragma comment (lib,"Gdiplus.lib")

char* NazwaKlasy = (char*)"Klasa Okienka";
MSG Komunikat;
HWND hwnd;
HINSTANCE hInstance;
HDC hdc;

unsigned long int lastId = 10;
unsigned long int lastIntervalId = 1;

void(*actions[100])();
void(*intervalActions[100])();

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );


class button {
  HWND localHwnd;
  unsigned int _id;
public:
  button (std::string text, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    _id = lastId++;
    localHwnd = CreateWindow(
    TEXT("button"),                      // The class name required is button
		TEXT(text.c_str()),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		x, y,                                  // the left and top co-ordinates
		width, height,                              // width and height
		hwnd,                                 // parent window handle
		(HMENU)_id,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

    OnClick ([]()->void{});
  }

  void Delete () {
    DestroyWindow(localHwnd);
  }

  void OnClick (void(*function)()) {
    actions[_id] = function;
  }

  void Click () {
    actions[_id]();
  }

  void SetText (std::string text) {
    SetWindowText(localHwnd, text.c_str());
  }
};


class textbox {
  HWND localHwnd;
  unsigned int _id;
public:
  textbox (std::string text, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    _id = lastId++;
    localHwnd = CreateWindow(
      TEXT("edit"),                      // The class name required is button
  		TEXT(text.c_str()),                  // the caption of the button
  		WS_BORDER|ES_MULTILINE|WS_CHILD|WS_VISIBLE,  // the styles
  		x, y,                                  // the left and top co-ordinates
  		width, height,                              // width and height
  		hwnd,                                 // parent window handle
  		(HMENU)_id,                   // the ID of your button
  		hInstance,                            // the instance of your application
  		NULL);                               // extra bits you dont really need

    OnClick ([]()->void{});
  }

  void Delete () {
    DestroyWindow(localHwnd);
  }

  void OnClick (void(*function)()) {
    actions[_id] = function;
  }

  void Click () {
    actions[_id]();
  }

  void SetText (std::string text) {
    SetWindowText(localHwnd, text.c_str());
  }
};


class radiobutton {
  HWND localHwnd;
  unsigned int _id;
public:
  radiobutton (std::string text, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    _id = lastId++;
    localHwnd = CreateWindow(
    TEXT("button"),                      // The class name required is button
		TEXT(text.c_str()),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,  // the styles
		x, y,                                  // the left and top co-ordinates
		width, height,                              // width and height
		hwnd,                                 // parent window handle
		(HMENU)_id,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

    OnClick ([]()->void{});
  }

  void Delete () {
    DestroyWindow(localHwnd);
  }

  void OnClick (void(*function)()) {
    actions[_id] = function;
  }

  void Click () {
    actions[_id]();
  }

  void SetText (std::string text) {
    SetWindowText(localHwnd, text.c_str());
  }
};


class interval {
  HWND localHwnd;
  unsigned int _id;
  void(*action)();
public:
  interval (unsigned int interval) {
    _id = lastIntervalId++;
    SetTimer(hwnd, _id, interval, 0);
    SetAction ([]()->void{});
  }

  void Delete () {
    DestroyWindow(localHwnd);
  }

  void Stop () {
    KillTimer(hwnd, _id);
  }

  void SetAction (void(*function)()) {
    intervalActions[_id] = function;
  }

  void Action () {
    intervalActions[_id]();
  }
};


class window_t {
public:
  int Init (HINSTANCE hInstance_to_save, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInstance = hInstance_to_save;
    // WYPEŁNIANIE STRUKTURY
    WNDCLASSEX wc;

    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NazwaKlasy;
    wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );

    // REJESTROWANIE KLASY OKNA
    if( !RegisterClassEx( & wc ) )
    {
        MessageBox( NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
        MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }

    // TWORZENIE OKNA


    hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, NazwaKlasy, "Oto okienko", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 800, 480, NULL, NULL, hInstance, NULL );

    if( hwnd == NULL )
    {
        MessageBox( NULL, "Okno odmówiło przyjścia na świat!", "Ale kicha...", MB_ICONEXCLAMATION );
        std::cout << ":(\n";
        return 1;
    }

    ShowWindow( hwnd, nCmdShow ); // Pokaż okienko...
    UpdateWindow( hwnd );
  }

  void Loop () {
    while( GetMessage( & Komunikat, NULL, 0, 0 ) )
    {
        TranslateMessage( & Komunikat );
        DispatchMessage( & Komunikat );
    }
  }

  int Events ( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    int wmId, wmEvent;
    switch( msg ) {
      case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        actions[wmId]();
        break;

      case WM_TIMER:
        intervalActions[wParam]();
        break;

      case WM_PAINT:
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hwnd, &ps);
        break;

      case WM_CLOSE:
        DestroyWindow( hwnd );
        break;

      case WM_DESTROY:
        PostQuitMessage( 0 );
        break;

      default:
        return DefWindowProc( hwnd, msg, wParam, lParam );
    }
  }
};


window_t window;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
  window.Init (hInstance, hPrevInstance, lpCmdLine, nCmdShow);

  textbox TextBox ("test_text", 300, 200, 100, 50);
  button resetButton ("reset", 200, 200, 100, 50);
  button drawButton ("draw", 100, 100, 100, 50);
  radiobutton radio1 ("radio1", 100, 200, 100, 50);
  radiobutton radio2 ("radio2", 100, 250, 100, 50);

  drawButton.OnClick ([]()->void{
    // Graphics graphics(hdc);
  	// Pen pen(Color(255, 0, 0, 255));

    static int i = 0;
    if (i >= 64) {
      MessageBox( NULL, "dupa blada", "Niestety...",
      MB_ICONEXCLAMATION | MB_OK );
    } else {
      std::cout << i << "\n";
      button dupaButton ("dupa", (((i/8)*100)+300)%(8*100), (i%8)*50, 100, 50);
      i++;
    }
  });

  resetButton.OnClick ([]()->void{
    std::cout << "test132\n";
    // TextBox.SetText("reset :)");
    // resetButton.Delete();
  });

  interval Interval (500);
  Interval.SetAction([]()->void{
    static int times = 0;
    times++;
    std::cout << "timer works :P " << times << "\n";

    if (times >= 10) {
      // Interval.Stop();
    }
  });

  window.Loop ();
}


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    window.Events(hwnd, msg, wParam, lParam);
}
