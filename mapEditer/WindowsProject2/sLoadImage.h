#pragma once
#include "sLoadData.h"

struct sLoadImage : public sLoadData
{
	texture* texturePtr;

	sLoadImage(const string& key, const tstring& path, const D3DXVECTOR2& offset, int width, int height)
		:sLoadData(key, path, LK_IMAGE), texturePtr(new texture(path, width, height, offset, false))
	{
	}

	sLoadImage(const string& key, const tstring& path, const D3DXVECTOR2& offset)
		:sLoadData(key, path, LK_IMAGE), texturePtr(new texture(path, offset, false))
	{
	}

	virtual ~sLoadImage() {}

	virtual bool Load() override;
};

struct sLoadNormalImage : public sLoadImage
{
	sLoadNormalImage(const string& key, const tstring& path, int width, int height, const D3DXVECTOR2& offset = D3DXVECTOR2(0, 0))
		:sLoadImage(key, path, offset ,width, height)
	{
	}

	sLoadNormalImage(const string& key, const tstring& path, const D3DXVECTOR2& offset = D3DXVECTOR2(0, 0))
		:sLoadImage(key, path, offset)
	{
	}
};

struct sLoadCenterImage : public sLoadImage
{
	sLoadCenterImage(const string& key, const tstring& path, int width, int height)
		:sLoadImage(key, path, OFFSET_CENTER, width, height)
	{
	}

	sLoadCenterImage(const string& key, const tstring& path)
		:sLoadImage(key, path, OFFSET_CENTER)
	{
	}
};