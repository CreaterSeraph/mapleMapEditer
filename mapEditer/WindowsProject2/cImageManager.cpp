#include "stdafx.h"
#include "cImageManager.h"

bool texture::Load()
{
	if (texturePtr) return false;//이미 로딩되어있는 리소스

	if (isLoaded)//한번 로딩이 되었던적이 있다면
	{
		return SUCCEEDED(D3DXCreateTextureFromFileEx(Device, filePath.c_str(), info.Width, info.Height, 0, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &texturePtr));
	}

	int width = info.Width;//새로 로딩해온다
	if (width == 0)
		width = D3DX_DEFAULT_NONPOW2;
	int height = info.Height;
	if (height == 0)
		height = D3DX_DEFAULT_NONPOW2;

	if (D3DXCreateTextureFromFileEx(Device, filePath.c_str(), width, height, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, nullptr, &texturePtr) == S_OK)
	{
		if (offset == OFFSET_CENTER)
		{
			offset.x = -(float)(info.Width / 2);
			offset.y = -(float)(info.Height / 2);
		}

		isLoaded = true;
		return true;
	}
	DEBUG_LOG_W(filePath.c_str() << L"의 로딩에 실패했습니다");
	return false;
}

void texture::Render(float x, float y, float rot)
{
	if (texturePtr || Load())
	{
		IMAGEMANAGER->Render(this, x, y, rot);
	}
}

void texture::Render(float x, float y, D3DCOLOR color)
{
	if (texturePtr || Load())
	{
		IMAGEMANAGER->Render(this, x, y, color);
	}
}

void texture::Render(const RenderData& data)
{
	if (texturePtr || Load())//texturePtr이 없는데 랜더링하려 하면 로딩을 시도해본다
	{
		IMAGEMANAGER->Render(this, data);
	}
}

void texture::Render(const RefRenderData& data)
{
	if (texturePtr || Load())//texturePtr이 없는데 랜더링하려 하면 로딩을 시도해본다
	{
		IMAGEMANAGER->Render(this, data);
	}
}

void texture::FixedRender(float x, float y, D3DCOLOR color)
{
	if (texturePtr || Load())
	{
		IMAGEMANAGER->FixedRender(this, x, y, color);
	}
}


cImageManager::cImageManager()
	:sprite(nullptr), zoomSize(1), camPos(0, 0)
{
	D3DXCreateSprite(Device, &sprite);
}


cImageManager::~cImageManager()
{
	for (auto iter : mImages)
		SAFE_DELETE(iter.second);
	mImages.clear();

	sprite->Release();
}

void cImageManager::Begin()
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void cImageManager::End()
{
	sprite->End();
}

bool cImageManager::AddImage(const string& key, texture* texturePtr)
{
	auto find = mImages.find(key);
	if (find == mImages.end())
	{
		mImages.insert(make_pair(key, texturePtr));
		return true;
	}
#ifdef _DEBUG
	else if (find->second->filePath != texturePtr->filePath)//이미 있는 키값인데 파일 경로가 다를경우
	{
		DEBUG_LOG("key :" << key);
		DEBUG_LINE_W(L"\tpath1 :" << find->second->filePath << L"\tpath2 :" << texturePtr->filePath);
	}
#endif
	return false;
}

texture* cImageManager::FindImage(const string& key)
{
	auto find = mImages.find(key);
	if (find == mImages.end()) return nullptr;
	return find->second;
}

bool cImageManager::DeleteTexture(const string& key)//있는 키값도 지우지는 않는다
{
	auto find = mImages.find(key);
	if (find == mImages.end()) return false;

	find->second->DeleteTexture();

	return true;
}

void cImageManager::FailedLoad(const string& key)
{
	auto find = mImages.find(key);
	if (find == mImages.end()) return;

	assert(find->second->texturePtr);

	mImages.erase(find);
}

void cImageManager::AddImageList(const map<string, texture*>& addList)
{
	for (auto& iter : addList)
	{
		auto find = mImages.find(iter.first);
		if (find == mImages.end())
			mImages.insert(std::move(iter));
#ifdef _DEBUG
		else if (find->second->filePath != iter.second->filePath)//이미 있는 키값인데 파일 경로가 다를경우
		{
			DEBUG_LOG("key :" << iter.first);
			DEBUG_LINE_W(L"\tpath1 :" << find->second->filePath << L"\tpath2 :" << iter.second->filePath);
		}
#endif
	}
}

void cImageManager::Render(texture* texturePtr, float x, float y, float rot)
{
	if (texturePtr)
	{
		D3DXMATRIXA16 matS, matR, matT, matFinal;
		D3DXVECTOR2 offset = texturePtr->offset;

		D3DXMatrixScaling(&matS, zoomSize, zoomSize, 1);

		D3DXMatrixRotationZ(&matR, rot);
		D3DXVec2TransformNormal(&offset, &offset, &matR);

		D3DXMatrixTranslation(&matT,
			(x + offset.x - camPos.x) * zoomSize, (y + offset.y - camPos.y) * zoomSize, 0);

		matFinal = matS * matR * matT;

		sprite->SetTransform(&matFinal);

		sprite->Draw(texturePtr->texturePtr, nullptr, nullptr, nullptr, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void cImageManager::Render(texture* texturePtr, float x, float y, D3DCOLOR color)
{
	if (texturePtr)
	{
		D3DXMATRIXA16 matS, matT, matFinal;
		D3DXVECTOR2 offset = texturePtr->offset;

		D3DXMatrixScaling(&matS, zoomSize, zoomSize, 1);

		D3DXMatrixTranslation(&matT,
			(x + offset.x - camPos.x) * zoomSize, (y + offset.y - camPos.y) * zoomSize, 0);

		matFinal = matS * matT;

		sprite->SetTransform(&matFinal);

		sprite->Draw(texturePtr->texturePtr, nullptr, nullptr, nullptr, color);
	}
}

void cImageManager::Render(texture* texturePtr, const RenderData& renderData)
{
	if (texturePtr)
	{
		auto& x = renderData.pos.x;
		auto& y = renderData.pos.y;
		auto& rot = renderData.rot;
		auto& sizeX = renderData.size.x;
		auto& sizeY = renderData.size.y;
		auto& color = renderData.color;
		D3DXMATRIXA16 matS, matR, matT, matFinal;
		D3DXVECTOR2 offset = texturePtr->offset;

		D3DXMatrixScaling(&matS, sizeX * zoomSize, sizeY * zoomSize, 1);
		offset.x *= sizeX;
		offset.y *= sizeY;

		D3DXMatrixRotationZ(&matR, rot);
		D3DXVec2TransformNormal(&offset, &offset, &matR);

		D3DXMatrixTranslation(&matT,
			(x + offset.x - camPos.x) * zoomSize, (y + offset.y - camPos.y) * zoomSize, 0);

		matFinal = matS * matR * matT;

		sprite->SetTransform(&matFinal);
		sprite->Draw(texturePtr->texturePtr, nullptr, nullptr, nullptr, color);
	}
}

void cImageManager::Render(texture* texturePtr, const RefRenderData& renderData)
{
	if (texturePtr)
	{
		auto& x = renderData.pos.x;
		auto& y = renderData.pos.y;
		auto& rot = renderData.rot;
		auto& sizeX = renderData.size.x;
		auto& sizeY = renderData.size.y;
		auto& color = renderData.color;
		D3DXMATRIXA16 matS, matR, matT, matFinal;
		D3DXVECTOR2 offset = texturePtr->offset;

		D3DXMatrixScaling(&matS, sizeX * zoomSize, sizeY * zoomSize, 1);
		offset.x *= sizeX;
		offset.y *= sizeY;

		D3DXMatrixRotationZ(&matR, rot);
		D3DXVec2TransformNormal(&offset, &offset, &matR);

		D3DXMatrixTranslation(&matT,
			(x + offset.x - camPos.x) * zoomSize, (y + offset.y - camPos.y) * zoomSize, 0);

		matFinal = matS * matR * matT;

		sprite->SetTransform(&matFinal);
		sprite->Draw(texturePtr->texturePtr, nullptr, nullptr, nullptr, color);
	}
}

void cImageManager::FixedRender(texture* texturePtr, float x, float y, D3DCOLOR color)
{
	D3DXMATRIXA16 matT;

	D3DXMatrixTranslation(&matT, x, y, 0);

	sprite->SetTransform(&matT);

	sprite->Draw(texturePtr->texturePtr, nullptr, nullptr, nullptr, color);
}

void cImageManager::LostDevice()
{
	sprite->OnLostDevice();
}

void cImageManager::ResetDevice()
{
	sprite->OnResetDevice();
}
