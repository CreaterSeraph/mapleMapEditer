#include "stdafx.h"
#include "mainWindow.h"

mainWindow::mainWindow()
{
}


mainWindow::~mainWindow()
{
}

bool mainWindow::Create(HINSTANCE hInstance, int nCmdShow)
{
	//typeid(mainWindow).hash_code();
#if defined(_DEBUG) || defined(DEBUGLOG_TORELEASE)
	std::wcout.imbue(std::locale("kor"));
#endif

	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmSaved);
	//deviceSize.x = 800;
	//deviceSize.y = 600;
	deviceSize.x = 1024;
	deviceSize.y = 768;

	const DWORD dwExStyle = 0;
	const DWORD dwStyle = WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;
	hWnd = CreateWindowExW(dwExStyle, WINDOW_NAME, TITLE_NAME, dwStyle,
		CW_USEDEFAULT, 0, deviceSize.x, deviceSize.y, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) return false;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	RECT rc;
	GetWindowRect(hWnd, &rc);
	
	AdjustWindowRect(&rc, dwStyle, false);
	winPos.x = rc.left;
	winPos.y = rc.top;
	winSize.x = deviceSize.x;
	winSize.y = deviceSize.y;

	MoveWindow(hWnd, winPos.x, winPos.y, rc.right - rc.left, rc.bottom - rc.top, false);

	MyDevice::CreateDevice(deviceSize.x, deviceSize.y);

	mg.Init();

	nowWindowMode = WINMODE_WINDOWED;
	windowMode = WINMODE_WINDOWED;

	GetWindowRect(hWnd, &winRc);

	return true;
}

WPARAM mainWindow::MessageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	
	TIMER->Init();
	TIMER->SetFixedFrame(60);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}

	return msg.wParam;
}

LRESULT mainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	mg.MsgProc(hWnd, message, wParam, lParam);
	switch (message)
	{
	case WM_SYSKEYDOWN://alt키는 모두 차단
		if (wParam == VK_RETURN)//화면변경
		{
			if (nowWindowMode == WINMODE_FULLSCREEN)
				ChangeSettingWindow(windowMode);
			else
				ChangeSettingWindow(WINMODE_FULLSCREEN);
		}
		else if (wParam == VK_F4);
		else
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		ptMouse.x = (double)LOWORD(lParam) * deviceSize.x / winSize.x;
		ptMouse.y = (double)HIWORD(lParam) * deviceSize.y / winSize.y;
		break;
	case WM_SIZE:
		winSize.x = LOWORD(lParam);
		winSize.y = HIWORD(lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void mainWindow::Update()
{
	if (TIMER->Update())
	{
		mg.Update();

		Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff0000ff, 1.0f, 0);
		Device->BeginScene();
		mg.Render();
		Device->EndScene();
		if (Device->Present(0, 0, 0, 0) == D3DERR_DEVICELOST)
		{
			mg.LostDevice();
		}
		if (Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			Device->Reset(&MyDevice::GetD3DPP(deviceSize.x, deviceSize.y));
			mg.ResetDevice();
		}
	}
}

void mainWindow::Destory()
{
	mg.Release();

	cTimer::ReleaseInstace();
	cSceneManager::ReleaseInstace();
	cKeyManager::ReleaseInstace();
	cLoadHelper::ReleaseInstace();
	cSoundManager::ReleaseInstace();
	cImageManager::ReleaseInstace();
	cTxtManager::ReleaseInstace();

	MyDevice::ReleaseDevice();

	if (nowWindowMode == WINMODE_FULLSCREEN)
		ResetDisplay();
}

void mainWindow::ChangeResolution(int x, int y)
{
	deviceSize.x = x;
	deviceSize.y = y;
	winSize.x = x;
	winSize.y = y;
	ChangeSettingWindow(nowWindowMode);
}

void mainWindow::ChangeSettingWindow(WINMODE mode)
{
	nowWindowMode = mode;

	ShowWindow(hWnd, SW_HIDE);
	DWORD style = (mode == WINMODE_WINDOWED) ?
		WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX :
		WS_POPUP;
	DWORD exStyle = (mode == WINMODE_FULLSCREEN) ?
		WS_EX_APPWINDOW :
		0;

	SetWindowLongPtr(hWnd, GWL_STYLE, style);
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle);

	mg.LostDevice();
	Device->Reset(&MyDevice::GetD3DPP(deviceSize.x, deviceSize.y));
	mg.ResetDevice();

	if (mode == WINMODE_FULLSCREEN)
	{
		DEVMODE dmSelected; // 원하는 해상도를 저장하는 변수

		memset(&dmSelected, 0, sizeof(dmSelected));
		dmSelected.dmSize = sizeof(dmSelected);
		dmSelected.dmPelsWidth = deviceSize.x;
		dmSelected.dmPelsHeight = deviceSize.y;
		dmSelected.dmBitsPerPel = 32;
		dmSelected.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		if (ChangeDisplaySettings(&dmSelected, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP);
			return;
		}
	}
	else
	{
		windowMode = mode;
		ResetDisplay();
	}

	ShowWindow(hWnd, SW_RESTORE);
	if (mode == WINMODE_FULLSCREEN)
	{
		MoveWindow(hWnd, 0, 0, deviceSize.x, deviceSize.y, false);
		ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	}
	else
	{
		RECT rc = { winPos.x, winPos.y, winPos.x + deviceSize.x, winPos.y + deviceSize.y };
		AdjustWindowRect(&rc, style, false);

		MoveWindow(hWnd, winPos.x, winPos.y, rc.right - rc.left, rc.bottom - rc.top, false);
	}
}

void mainWindow::ResetDisplay()
{
	ChangeDisplaySettings(&dmSaved, CDS_RESET);
}
