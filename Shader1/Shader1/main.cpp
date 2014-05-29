#include <Windows.h>

#include "Engine.h"


// Global Variables
const TCHAR* g_szWINDOW_TITLE = L"Shader#01 - plasma refraction";
const TCHAR* g_szCLASS_NAME = L"MyClass";

LRESULT CALLBACK WindowProc(HWND _hWnd, unsigned int _unMsg, WPARAM _wParam, LPARAM _lParam);


// Our program's main function
int WINAPI wWinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance,
	LPTSTR _lpCmdLine, int _nCmdShow)
{
	// Create a window structure
	WNDCLASSEX wndEx;

	wndEx.cbSize = sizeof(WNDCLASSEX);

	wndEx.style = CS_HREDRAW | CS_VREDRAW;
	wndEx.lpfnWndProc = WindowProc;
	wndEx.cbClsExtra = 0;
	wndEx.cbWndExtra = 0;
	wndEx.hInstance = _hInstance;
	wndEx.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wndEx.lpszMenuName = NULL;
	wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndEx.lpszClassName = g_szCLASS_NAME;
	wndEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Register the structure
	RegisterClassEx(&wndEx);

	// Create the window 
	HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, g_szCLASS_NAME, g_szWINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		(GetSystemMetrics(SM_CXSCREEN) / 2) - 400, (GetSystemMetrics(SM_CYSCREEN) / 2) - 300,
		800, 600, nullptr, nullptr, _hInstance, nullptr);

	if (hWnd == nullptr)
		return -1;

	// Show the new window
	ShowWindow(hWnd, _nCmdShow);
	UpdateWindow(hWnd);

	MSG Msg;


	Engine newApp;
	newApp.Init(hWnd);
	newApp.AddSprite(L"Rayman.png");

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec); 
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 TimeStamp = 0; 
	

	while (true)
	{
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_QUIT)
				break;
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		QueryPerformanceCounter((LARGE_INTEGER*)&TimeStamp);
		float dt = secsPerCnt * TimeStamp;
		newApp.Render(dt);
	}

	newApp.Release();

	UnregisterClass(g_szWINDOW_TITLE, _hInstance);

	return 0;
};

// Function for our Window Proc
LRESULT CALLBACK WindowProc(HWND _hWnd, unsigned int _unMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_unMsg)
	{
		case WM_CLOSE:
		{
			DestroyWindow(_hWnd);
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return(0);
		}

		default:
		{
			break;
		}
	};

	return DefWindowProc(_hWnd, _unMsg, _wParam, _lParam);
};