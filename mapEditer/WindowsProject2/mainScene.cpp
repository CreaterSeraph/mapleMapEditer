#include "stdafx.h"
#include "mainScene.h"

#include "cMapObject.h"
#include "cMapData.h"

mainScene::mainScene()
{
}

mainScene::~mainScene()
{
}

void mainScene::Init()
{
	mapData = new cMapData(T("1"));
	//mapData->TextureLoad(T("ancientJungle.img"));
	//
	//mapData->DeleteAdjMapData();
	//mapData->MakeAdjMapData();
	//mapData->Save(1);
}

void mainScene::Release()
{
	SAFE_DELETE(mapData);
}

void mainScene::Update()
{
	mapData->Update();
}

void mainScene::Render()
{
	mapData->Render();
}

void mainScene::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

}
