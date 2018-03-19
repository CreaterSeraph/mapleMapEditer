#pragma once

class cLoadHelper;
class mainGame
{
public:
	mainGame();
	~mainGame();

	void Init();
	void Release();
	void Update();
	void Render();

	void LostDevice();
	void ResetDevice();
	void MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

