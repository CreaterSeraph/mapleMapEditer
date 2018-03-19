#include "stdafx.h"
#include "cSceneManager.h"
#include "cScene.h"

cSceneManager::cSceneManager()
	:nowScene(nullptr), nextScene(nullptr)
{
}

cSceneManager::~cSceneManager()
{
	Release();
}

void cSceneManager::Release()
{
	if (nowScene)
		nowScene->Release();
	for (auto temp : mScenes)
		SAFE_DELETE(temp.second);
	mScenes.clear();
}

void cSceneManager::Update()
{
	if (nextScene)
	{
		if (nowScene)
			nowScene->Release();
		nextScene->Init();
		nowScene = nextScene;
		nextScene = nullptr;
		auto temp = TIMER->Skip();
	}
	if (nowScene)
		nowScene->Update();
}

void cSceneManager::Render()
{
	if (nowScene)
		nowScene->Render();
}

void cSceneManager::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (nowScene)
		nowScene->MsgProc(hWnd, iMessage, wParam, lParam);
}

cScene* cSceneManager::AddScene(const string& key, cScene* scene)
{
	if (!scene)
		return nullptr;
	if (mScenes.find(key) != mScenes.end())
		return nullptr;
	mScenes.insert(make_pair(key, scene));
	return scene;
}

cScene* cSceneManager::ChangeScene(const string& key)
{
	auto find = mScenes.find(key);
	if (find == mScenes.end())
		return nullptr;
	nextScene = find->second;
	return nextScene;
}

cScene* cSceneManager::FindScene(const string& key)
{
	auto find = mScenes.find(key);
	if (find == mScenes.end())
		return nullptr;
	return find->second;
}

cScene* cSceneManager::RestartScene()
{
	nextScene = nowScene;
	return nextScene;
}
