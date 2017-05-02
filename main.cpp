#include <windows.h>
#include <iostream>
#include <string>

char* NazwaKlasy = (char*)"Klasa Okienka";
MSG Komunikat;
HWND hwnd;
HINSTANCE hInstance;

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );


class button_t {
  HWND buttonHwnd;
  void(*action)();
public:
  Create (std::string text, unsigned int id, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    buttonHwnd = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT(text.c_str()),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		x, y,                                  // the left and top co-ordinates
		width, height,                              // width and height
		hwnd,                                 // parent window handle
		(HMENU)id,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

    OnClick ([]()->void{});
  }

  OnClick (void(*function)()) {
    action = function;
  }

  Click () {
    action();
  }

  Text (std::string text) {
    SetWindowText(buttonHwnd, text.c_str());
  }
};


class window_t {
  button_t buttonList[1000];
  void(*buttonActions[])();
public:
  // document_t* document;
  init (HINSTANCE hInstance_to_save, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
        return 1;
    }

    ShowWindow( hwnd, nCmdShow ); // Pokaż okienko...
    UpdateWindow( hwnd );
  }

  loop () {
    while( GetMessage( & Komunikat, NULL, 0, 0 ) )
    {
        TranslateMessage( & Komunikat );
        DispatchMessage( & Komunikat );
    }
  }

  events ( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    int wmId, wmEvent;
    switch( msg ) {
      case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        buttonList[wmId].Click();
        break;

      // case WM_PAINT:
      //   PAINTSTRUCT ps;
      //   HDC hdc = BeginPaint(hwnd, &ps);
      //   // TODO: Add any drawing code that uses hdc here...
      //   EndPaint(hwnd, &ps);
      //   break;
      //
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

  button_t& button (unsigned int id) {
    return buttonList[id];
  }
};


window_t window;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
  window.init (hInstance, hPrevInstance, lpCmdLine, nCmdShow);

  window.button(2100).Create("test_button", 2100, 200, 300, 100, 50);
  window.button(2100).OnClick ([]()->void{
    std::cout << "test132\n";
    window.button(2100).Text("new text");
  });

  window.loop ();
}


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    window.events(hwnd, msg, wParam, lParam);
}
