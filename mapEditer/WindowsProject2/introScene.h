#pragma once
#include "cScene.h"
class introScene : public cScene
{
private:
	
public:
	introScene();
	~introScene();

	virtual void Init()	override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;
};