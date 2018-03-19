#include "stdafx.h"
#include "mainGame.h"

#include "mainScene.h"

mainGame::mainGame()
{
}

mainGame::~mainGame()
{
}

void mainGame::Init()
{
	srand(time(NULL));

	int result = rand() % 3 + 1;

	double 총합 = 0;
	double 증가량 = 20;
	int 퍼센트 = 0;

	while (퍼센트 < 100)
	{
		총합 += 증가량;
		퍼센트 += 5;
		DEBUG_LOG("퍼센트 : " << 퍼센트 << "\t 총합 : " << 총합 << "\t 증가량 : " << 증가량);
		증가량 *= 0.80245134346231945;
	}

	SCENEMANAGER->AddScene("main", new mainScene);
	SCENEMANAGER->ChangeScene("main");
}

void mainGame::Release()
{
}

void mainGame::Update()
{
	KEYMANAGER->Update();
	SOUNDMANAGER->Update();
	SCENEMANAGER->Update();

	LOADMANAGER->Update();
}

void mainGame::Render()
{
	IMAGEMANAGER->Begin();

	SCENEMANAGER->Render();

	IMAGEMANAGER->End();
}

void mainGame::LostDevice()
{
	IMAGEMANAGER->LostDevice();
}

void mainGame::ResetDevice()
{
	IMAGEMANAGER->ResetDevice();
}

void mainGame::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	KEYMANAGER->MsgProc(hWnd, iMessage, wParam, lParam);
	SCENEMANAGER->MsgProc(hWnd, iMessage, wParam, lParam);
}
