#pragma once
#include "cScene.h"

class cMapObject;
class cMapData;

class mainScene : public cScene
{
private:
	cMapObject* mapObj;
	cMapData* mapData;
public:
	mainScene();
	~mainScene();

	virtual void Init()	override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;
};