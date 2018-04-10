#include "stdafx.h"
#include "introScene.h"

#include "mainScene.h"

introScene::introScene()
{
}


introScene::~introScene()
{
}

void introScene::Init()
{
	//로딩화면에 출력시킬 텍스쳐와 사운드 먼저 로딩

	//로딩화면에서 로드해올 메인화면 및 플레이어 infomation로드
	LOADMANAGER->Start();
}

void introScene::Release()
{
}

void introScene::Update()
{
	if (true/*인트로 애니메이션이 출력종료되고*/ && true/*로딩도 종료 되면*/)
	{
		SCENEMANAGER->ChangeScene("main");//메인 화면으로의 씬전환
	}
	LOADMANAGER->Update();
}

void introScene::Render()
{
	//로드 매니저 사용해서 진행상황 출력
	//그리고 인트로 애니메이션 출력
}

void introScene::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
}
