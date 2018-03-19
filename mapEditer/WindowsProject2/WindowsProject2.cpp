// Win32Project1.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "WindowsProject2.h"

//#include "mainWindow.h"

#define MAX_LOADSTRING 100

//�������� ���
HWND				hWnd;
LPDIRECT3DDEVICE9	Device;
POINT				ptMouse;

//mainWindow			mainWnd;

//WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
//WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	//_CrtSetBreakAlloc(823);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#if defined(_DEBUG) || defined(DEBUGLOG_TORELEASE)
	AllocConsole();
	freopen("CONOUT$", "a", stdout);
	freopen("CONOUT$", "a", stderr);
	freopen("CONIN$", "r", stdin);
#endif

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	MyRegisterClass(hInstance);

	if (!MYWINDOW->Create(hInstance, nCmdShow))
	{
		return false;
	}

	WPARAM result = MYWINDOW->MessageLoop();

	MYWINDOW->Destory();
	mainWindow::ReleaseInstace();
#if defined(_DEBUG) || defined(DEBUGLOG_TORELEASE)
	FreeConsole();
#endif

	return (int)result;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WINDOW_NAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return MYWINDOW->WndProc(hWnd, message, wParam, lParam);
}