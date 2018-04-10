#pragma once
#include "singleton.h"

class cScene;
class cSceneManager : public singleton<cSceneManager>
{
private:
	map<string, cScene*> mScenes;
	cScene* nowScene;
	cScene* nextScene;
private:
	void Release();
public:
	cSceneManager();
	~cSceneManager();

	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	cScene* AddScene(const string& key, cScene* scene);
	cScene* ChangeScene(const string& key);
	cScene* FindScene(const string& key);
	cScene* RestartScene();
};

#define SCENEMANAGER cSceneManager::GetInstance()