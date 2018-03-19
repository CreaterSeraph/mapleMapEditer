#pragma once
#include "singleton.h"

struct RenderData
{
	D3DXVECTOR2 pos;
	float rot;
	D3DXVECTOR2 size;

	D3DCOLOR color;
};

struct RefRenderData
{
	const D3DXVECTOR2& pos;
	const float& rot;
	const D3DXVECTOR2& size;

	const D3DCOLOR& color;
	RefRenderData(const D3DXVECTOR2& pos, const float& rot, const D3DXVECTOR2& size, const D3DCOLOR& color)
		:pos(pos), rot(rot), size(size), color(color)
	{
	}
};

struct MYIMAGEINFO : public D3DXIMAGE_INFO
{
	MYIMAGEINFO()
	{
		ZeroMemory(this, sizeof(MYIMAGEINFO));
	}
};

#define OFFSET_CENTER	D3DXVECTOR2(1365, 2080) //Feat.진하

struct texture
{
	LPDIRECT3DTEXTURE9 texturePtr;		//4바이트
	MYIMAGEINFO info;					//28바이트(최대한 피하자)
	D3DXVECTOR2 offset;					//8바이트
	tstring filePath;
	bool isLoaded;

	texture(const tstring& filePath, const D3DXVECTOR2 offset = D3DXVECTOR2(0, 0), bool load = true)
		:texturePtr(nullptr), offset(offset), filePath(filePath), isLoaded(false), info()
	{
		info.Width = D3DX_DEFAULT_NONPOW2;
		info.Height = D3DX_DEFAULT_NONPOW2;
		if (load)
			Load();
	}

	texture(const tstring& filePath, UINT width, UINT height,
		const D3DXVECTOR2 offset = D3DXVECTOR2(0, 0), bool load = true)
		:texturePtr(nullptr), offset(offset), filePath(filePath), isLoaded(false), info()
	{
		info.Width = width;
		info.Height = height;
		if (load)
			Load();
	}

	~texture()
	{
		DeleteTexture();
	}

	void Render(float x, float y, float rot);
	void Render(float x, float y, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	void Render(const RenderData& data);
	void Render(const RefRenderData& data);
	void FixedRender(float x, float y, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	bool Load();

	void DeleteTexture() { SAFE_RELEASE(texturePtr); }
};

class cImageManager : public singleton<cImageManager>
{
private:
	LPD3DXSPRITE sprite;
	map<string, texture*> mImages;

	D3DXVECTOR2 camPos;
	double zoomSize;
public:
	cImageManager();
	~cImageManager();

	void Begin();
	void End();

	const D3DXVECTOR2& GetCamPos() { return camPos; }
	void SetCamPos(const D3DXVECTOR2& camPos) { this->camPos = camPos; }
	double GetZoomSize() { return zoomSize; }
	void SetZoomSize(double zoomSize) { this->zoomSize = zoomSize; }

	bool AddImage(const string& key, texture* texturePtr);
	texture* FindImage(const string& key);

	bool DeleteTexture(const string& key);
	void FailedLoad(const string& key);

	void AddImageList(const map<string, texture*>& addList);

	void Render(texture* texturePtr, float x, float y, float rot);
	void Render(texture* texturePtr, float x, float y, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	void Render(texture* texturePtr, const RenderData& renderData);
	void Render(texture* texturePtr, const RefRenderData& renderData);

	void FixedRender(texture* texturePtr, float x, float y, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

	void LostDevice();
	void ResetDevice();
};

#define IMAGEMANAGER cImageManager::GetInstance()
