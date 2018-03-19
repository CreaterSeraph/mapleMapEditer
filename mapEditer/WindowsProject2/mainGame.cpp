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

	double ���� = 0;
	double ������ = 20;
	int �ۼ�Ʈ = 0;

	while (�ۼ�Ʈ < 100)
	{
		���� += ������;
		�ۼ�Ʈ += 5;
		DEBUG_LOG("�ۼ�Ʈ : " << �ۼ�Ʈ << "\t ���� : " << ���� << "\t ������ : " << ������);
		������ *= 0.80245134346231945;
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
