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
	//�ε�ȭ�鿡 ��½�ų �ؽ��Ŀ� ���� ���� �ε�

	//�ε�ȭ�鿡�� �ε��ؿ� ����ȭ�� �� �÷��̾� infomation�ε�
	LOADMANAGER->Start();
}

void introScene::Release()
{
}

void introScene::Update()
{
	if (true/*��Ʈ�� �ִϸ��̼��� �������ǰ�*/ && true/*�ε��� ���� �Ǹ�*/)
	{
		SCENEMANAGER->ChangeScene("main");//���� ȭ�������� ����ȯ
	}
	LOADMANAGER->Update();
}

void introScene::Render()
{
	//�ε� �Ŵ��� ����ؼ� �����Ȳ ���
	//�׸��� ��Ʈ�� �ִϸ��̼� ���
}

void introScene::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
}
