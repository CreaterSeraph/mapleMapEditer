#pragma once
#include "singleton.h"
#include "mainGame.h"
#include "singleton.h"

enum WINMODE
{
	WINMODE_WINDOWED, WINMODE_POPUP, WINMODE_FULLSCREEN
};

class mainWindow : public singleton<mainWindow>
{
	static int test;
private:
	DEVMODE dmSaved;//게임시작시 해상도를 저장한다
	POINT winPos;//창모드시 윈도우의 위치를 기억해둔다
	POINT winSize;//창모드시 윈도우의 사이즈를 기억해둔다

	bool winActive;//현재 윈도우가 활성화중인지

	WINMODE nowWindowMode;//현재 윈도우 모드
	WINMODE windowMode;//기본 윈도우 모드

	mainGame mg;
	POINT deviceSize;//현재 디바이스가 가지고 있는 사이즈를 기억해둔다

	RECT winRc;
public:
	POINT GetDeviceSize() { return deviceSize; }
	POINT GetWinSize() { return winSize; }
	RECT GetWinodwRect() { return winRc; }
public:
	mainWindow();
	~mainWindow();

	bool Create(HINSTANCE hInstance, int nCmdShow);
	WPARAM MessageLoop();
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Update();
	void Destory();

	void ChangeResolution(int x, int y);
	void ChangeSettingWindow(WINMODE mode);
	void ResetDisplay();
};

#define MYWINDOW mainWindow::GetInstance()