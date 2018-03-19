#pragma once
class cScene abstract
{
public:
	cScene();
	virtual ~cScene();

	virtual void Init()	PURE;
	virtual void Release() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};