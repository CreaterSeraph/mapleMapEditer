#pragma once
#include "singleton.h"

struct sLoadData;
struct sLoadImage;
struct sLoadSound;
class cFileLoader
{
	vector<sLoadData*> vLoadList;
	vector<string> vFailed[LK_END];

	__int64 fileSize;
	__int64 loadedFileSize;
	__int64 willLoadFileSize;

	size_t fileCount;
	size_t loadedFileCount;
	size_t willLoadFileCount;

	thread loadThread;

	bool nowLoading;
public:
	cFileLoader();
	~cFileLoader();

	void AddLoadData(sLoadData* add);

	bool IsLoading() { return nowLoading; }
	bool IsLoadEnd() { return vLoadList.empty() || loadedFileCount == fileCount; }
	__int64 GetFileSize() { return nowLoading ? fileSize : loadedFileSize; }
	__int64 GetLoadedFileSize() { return loadedFileSize; }
	size_t GetFileCount() { return nowLoading ? fileCount : loadedFileCount; }
	size_t GetLoadedFileCount() { return loadedFileCount; }

	__int64 GetWillLoadFileSize() { return willLoadFileSize; }
	size_t GetWillLoadFileCount() { return willLoadFileCount; }

	void StartLoad();
	void LoadFunc(); 
	void EndLoad();
	void ResetData();
};

class cLoadHelper : public singleton<cLoadHelper>
{
private:
	vector<sLoadData*> vLoadList;
	vector<cFileLoader*> vFileLoader;

	bool isLoading;
public:
	cLoadHelper();
	~cLoadHelper();

	bool AddImage(const string& key, const tstring& path, int width, int height, const D3DXVECTOR2& offset = D3DXVECTOR2(0, 0));
	bool AddCenterImage(const string& key, const tstring& path, int width, int height);
	bool AddImage(const string& key, const tstring& path, const D3DXVECTOR2& offset = D3DXVECTOR2(0, 0));
	bool AddCenterImage(const string& key, const tstring& path);
	bool AddSound(const string& key, const tstring& path, const SOUND_KIND& kind, float volume = 1.0f);

	void Start();
	void Update();

	__int64 GetFileSize();
	__int64 GetLoadedFileSize();
	size_t GetFileCount();
	size_t GetLoadedFileCount();
};

#define LOADMANAGER cLoadHelper::GetInstance()