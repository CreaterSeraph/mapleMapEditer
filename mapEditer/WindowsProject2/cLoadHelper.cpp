#include "stdafx.h"
#include "cLoadHelper.h"

#include "sLoadImage.h"
#include "sLoadSound.h"

cFileLoader::cFileLoader()
	:willLoadFileCount(0), willLoadFileSize(0), fileSize(0), loadedFileSize(0), fileCount(0), loadedFileCount(0), nowLoading(false)
{
}

cFileLoader::~cFileLoader()
{
	if (loadThread.joinable())
		loadThread.join();
	for (auto& iter : vLoadList)
		SAFE_DELETE(iter);
}

void cFileLoader::AddLoadData(sLoadData* add)
{
	vLoadList.push_back(add);
	willLoadFileSize += add->fileSize;
	willLoadFileCount++;
}

void cFileLoader::StartLoad()
{
	if (vLoadList.empty()) return;
	if (loadThread.joinable())
		loadThread.join();
	fileCount = willLoadFileCount;
	fileSize = willLoadFileSize;

	willLoadFileCount = 0;
	willLoadFileSize = 0;

	loadedFileSize = 0;
	loadedFileCount = 0;

	nowLoading = true;

	loadThread = thread([&]() {LoadFunc(); });
}

void cFileLoader::LoadFunc()
{
	for (auto iter : vLoadList)
	{
		if (!iter->Load())
		{
			vFailed[iter->kind].push_back(iter->key);
		}
		loadedFileSize += iter->fileSize;
		loadedFileCount++;
	}
}

void cFileLoader::EndLoad()
{
	if (loadThread.joinable())
		loadThread.join();
	for (auto iter : vFailed[LK_IMAGE])
	{
		IMAGEMANAGER->FailedLoad(iter);
	}
	for (auto iter : vFailed[LK_SOUND])
	{
		SOUNDMANAGER->FailedLoad(iter);
	}
	for (auto& iter : vLoadList)
	{
		SAFE_DELETE(iter);
	}
	vLoadList.clear();
	vFailed[LK_IMAGE].clear();
	vFailed[LK_SOUND].clear();

	nowLoading = false;
}

void cFileLoader::ResetData()
{
	loadedFileSize = 0;
	loadedFileCount = 0;

	fileSize = 0;
	fileCount = 0;
}

cLoadHelper::cLoadHelper()
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	for (size_t i = 0; i < systemInfo.dwNumberOfProcessors; i++)
	{
		vFileLoader.push_back(new cFileLoader);
	}
}

cLoadHelper::~cLoadHelper()
{
	for (auto iter : vFileLoader)
		SAFE_DELETE(iter);
	for (auto iter : vLoadList)
		SAFE_DELETE(iter);
}

bool cLoadHelper::AddImage(const string& key, const tstring& path, int width, int height, const D3DXVECTOR2& offset)
{
	auto temp = new sLoadNormalImage(key, path, width, height, offset);
	if (temp->fileSize != 0)
	{
		if (IMAGEMANAGER->AddImage(temp->key, temp->texturePtr))
		{
			vLoadList.push_back(temp);
			return true;
		}
	}
	delete temp;
	return false;
}

bool cLoadHelper::AddCenterImage(const string& key, const tstring& path, int width, int height)
{
	auto temp = new sLoadCenterImage(key, path, width, height);
	if (temp->fileSize != 0)
	{
		if (IMAGEMANAGER->AddImage(temp->key, temp->texturePtr))
		{
			vLoadList.push_back(temp);
			return true;
		}
	}
	delete temp;
	return false;
}

bool cLoadHelper::AddImage(const string& key, const tstring& path, const D3DXVECTOR2& offset)
{
	auto temp = new sLoadNormalImage(key, path, offset);
	if (temp->fileSize != 0)
	{
		if (IMAGEMANAGER->AddImage(temp->key, temp->texturePtr))
		{
			vLoadList.push_back(temp);
			return true;
		}
	}
	delete temp;
	return false;
}

bool cLoadHelper::AddCenterImage(const string& key, const tstring& path)
{
	auto temp = new sLoadCenterImage(key, path);
	if (temp->fileSize != 0)
	{
		if (IMAGEMANAGER->AddImage(temp->key, temp->texturePtr))
		{
			vLoadList.push_back(temp);
			return true;
		}
	}
	delete temp;
	return false;
}

bool cLoadHelper::AddSound(const string& key, const tstring& path, const SOUND_KIND& kind, float volume)
{
	auto temp = new sLoadSound(key, path, kind, volume);
	if (temp->fileSize != 0)
	{
		if (SOUNDMANAGER->AddSound(temp->key, temp->soundPtr))
		{
			vLoadList.push_back(temp);
			return true;
		}
	}
	delete temp;
	return false;
}

//struct LoadSizeBig
//{
//	bool operator()(sLoadData* data1, sLoadData* data2) const
//	{
//		return data1->fileSize > data2->fileSize;
//	}
//};

void cLoadHelper::Start()
{
	if (vFileLoader.empty()) return;
	size_t loaderSize = vFileLoader.size();
	sort(vLoadList.begin(), vLoadList.end(), [](sLoadData* data1, sLoadData* data2)
	{
		return data1->fileSize > data2->fileSize;
	});
	//sort(vLoadList.begin(), vLoadList.end(), LoadSizeBig());

	size_t imageSize = vLoadList.size();
	for (size_t i = 0; i < imageSize; i++)
	{
		__int64 minFileSize = INT_MAX;
		int minIdx = -1;
		for (int j = 0; j < loaderSize; j++)
		{
			if (vFileLoader[j]->IsLoading()) continue;//현재 로딩중인 스레드는 제외시킨다
			__int64 nowSize = vFileLoader[j]->GetWillLoadFileSize();
			if (nowSize < minFileSize)
			{
				minFileSize = nowSize;
				minIdx = j;
			}
		}
		if (minIdx == -1) continue;//모든 스레드가 로딩중인데 어떻게 추가하냐

		vFileLoader[minIdx]->AddLoadData(std::move(vLoadList[i]));
		vLoadList.erase(vLoadList.begin() + i--);
		imageSize--;
	}

	for (size_t i = 0; i < loaderSize; i++)
	{
		if (vFileLoader[i]->GetWillLoadFileCount() != 0)
			vFileLoader[i]->StartLoad();
	}
	isLoading = true;
}

void cLoadHelper::Update()
{
	size_t end = 0;
	for (auto& iter : vFileLoader)
	{
		if (iter->IsLoadEnd())
			iter->EndLoad();
		if (!iter->IsLoading())
			end++;
	}
	if (end == vFileLoader.size() && isLoading)
	{
		for (auto& iter : vFileLoader)
			iter->ResetData();
		isLoading = false;
	}
	if(isLoading)
		DEBUG_LOG(GetFileCount() << "\t" << GetLoadedFileCount() << "\t" << GetFileSize() << "\t" << GetLoadedFileSize());
}

__int64 cLoadHelper::GetFileSize()
{
	__int64 result = 0;
	if (isLoading)
	{
		for (auto& iter : vFileLoader)
		{
			result += iter->GetFileSize();
		}
	}
	return result;
}

__int64 cLoadHelper::GetLoadedFileSize()
{
	__int64 result = 0;
	if (isLoading)
	{
		for (auto& iter : vFileLoader)
		{
			result += iter->GetLoadedFileSize();
		}
	}
	return result;
}

size_t cLoadHelper::GetFileCount()
{
	size_t result = 0;
	if (isLoading)
	{
		for (auto& iter : vFileLoader)
		{
			result += iter->GetFileCount();
		}
	}
	return result;
}

size_t cLoadHelper::GetLoadedFileCount()
{
	size_t result = 0;
	if (isLoading)
	{
		for (auto& iter : vFileLoader)
		{
			result += iter->GetLoadedFileCount();
		}
	}
	return result;
}