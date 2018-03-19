#include "stdafx.h"
#include "cKeyManager.h"


cKeyManager::cKeyManager()
{
	Init();
}


cKeyManager::~cKeyManager()
{
}

void cKeyManager::Init()
{
	ZeroMemory(nowKeyState, sizeof(nowKeyState));
	ZeroMemory(prevKeyState, sizeof(nowKeyState));
}

void cKeyManager::Update()
{
	memcpy(prevKeyState, nowKeyState, sizeof(nowKeyState));
	for (int i = 0; i < 256; i++)
	{
		nowKeyState[i] = (GetAsyncKeyState(i) & 0x8000);
	}

	wheelCount = stackWheelCount;
	stackWheelCount = 0;
}

void cKeyManager::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (iMessage == WM_MOUSEWHEEL)
	{
		if (HIWORD(wParam) > 0)			stackWheelCount++;
		else if (HIWORD(wParam) < 0)	stackWheelCount--;
	}
}

bool cKeyManager::IsOnceKeyDown(int key)
{
	return nowKeyState[key] && !prevKeyState[key];
}

bool cKeyManager::IsStayKeyDown(int key)
{
	return nowKeyState[key];
}

bool cKeyManager::IsOnceKeyUp(int key)
{
	return !nowKeyState[key] && prevKeyState[key];
}

int cKeyManager::GetWheelCount()
{
	return wheelCount;
}
