#pragma once
#include "singleton.h"

class cKeyManager : public singleton<cKeyManager>
{
private:
	bool nowKeyState[256];
	bool prevKeyState[256];

	int wheelCount;
	int stackWheelCount;
private:
	void Init();
public:
	cKeyManager();
	~cKeyManager();

	void Update();
	void MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	bool IsOnceKeyDown(int key);
	bool IsStayKeyDown(int key);
	bool IsOnceKeyUp(int key);
	int GetWheelCount();
};

#define KEYMANAGER cKeyManager::GetInstance()