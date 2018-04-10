#pragma once

struct sLoadData
{
	const LOAD_KIND kind;
	size_t fileSize;
	string key;
	tstring path;

	sLoadData(const string& key, const tstring& path, const LOAD_KIND kind)
		:path(path), key(key), kind(kind)
	{
		fileSize = TXTMANAGER->GetFileInfo(path).st_size;
		if (fileSize == 0)
			DEBUG_LOG_W("���� ������ �ε��ؿ����߽��ϴ�\t" << path.c_str());
	}

	virtual ~sLoadData() {}

	virtual bool Load() PURE;
};

