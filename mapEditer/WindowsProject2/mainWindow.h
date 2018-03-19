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
	DEVMODE dmSaved;//���ӽ��۽� �ػ󵵸� �����Ѵ�
	POINT winPos;//â���� �������� ��ġ�� ����صд�
	POINT winSize;//â���� �������� ����� ����صд�

	bool winActive;//���� �����찡 Ȱ��ȭ������

	WINMODE nowWindowMode;//���� ������ ���
	WINMODE windowMode;//�⺻ ������ ���

	mainGame mg;
	POINT deviceSize;//���� ����̽��� ������ �ִ� ����� ����صд�

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