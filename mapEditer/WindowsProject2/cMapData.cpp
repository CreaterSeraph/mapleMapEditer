#include "stdafx.h"
#include "cMapData.h"

#include "cMapObject.h"
#include "cButton.h"

size_t cMapData::MakeIdx(size_t x, size_t y)
{
	if (x < 0 || y < 0 || x >= widthLen || y >= heightLen)
		return -1; 
	return y * widthLen + x;
}

size_t cMapData::MakeIdx(const POINT& pt)
{
	return MakeIdx(pt.x, pt.y);
}

POINT cMapData::MakePt(size_t idx)
{
	POINT result;
	result.x = idx % widthLen;
	result.y = idx / widthLen;

	return result;
}

cMapData::cMapData(size_t width, size_t height, const D3DXVECTOR2& offset)
	:widthLen(width), heightLen(height), offsetPos(offset), active(true)
{
	tiles.resize(width * height);
	Init();
}

cMapData::cMapData(vector<cTile>&& tiles, size_t width, const D3DXVECTOR2& offset)
	:tiles(tiles), widthLen(width), heightLen(tiles.size() / width), offsetPos(offset), active(true)
{
	Init();
}

cMapData::cMapData(const tstring& name)
	:widthLen(0), heightLen(0), active(false), offsetPos(0, 0), selectIdx(-1)
{
	Init();
	Load(name);
}

cMapData::cMapData()
	:active(false), widthLen(0), heightLen(0), offsetPos(0, 0), selectIdx(-1)
{
	Init();
}

void cMapData::Init()
{
	editTexture = new texture(T("./image/edit/tileOutLine.png"));

#pragma region SaveButton
	auto funcSave = [&]()
	{
		OPENFILENAME ofn;
		TCHAR lpstrFile[MAX_PATH] = T("");

		memset(&ofn, 0, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFilter = T("MapDataFile(*.map)\0*.map\0");
		ofn.lpstrFile = lpstrFile;
		ofn.lpstrInitialDir = T(".\\map");
		ofn.lpstrTitle = T("");
		ofn.Flags = OFN_DONTADDTORECENT | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_NOCHANGEDIR;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = T(".map");
		//http://soen.kr/lecture/win32api/reference/Structure/OPENFILENAME.htm
		if (GetSaveFileName(&ofn) != 0)
		{
			tstring filePath = ofn.lpstrFile;
			tstring fileName = filePath.substr(ofn.nFileOffset, filePath.size() - ofn.nFileOffset - 4);
			Save(fileName);
		}
	};

	save = new cButton(funcSave, T("./image/BtBase"), D3DXVECTOR2(MYWINDOW->GetDeviceSize().x - 110, 20));
#pragma endregion

#pragma region LoadButton
	auto funcLoad = [&]()
	{
		OPENFILENAME ofn;
		TCHAR lpstrFile[MAX_PATH] = T("");

		memset(&ofn, 0, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFilter = T("TileDataFile(*.tile)\0*.tile\0");
		ofn.lpstrFile = lpstrFile;
		ofn.lpstrInitialDir = T(".\\tileMap");
		ofn.lpstrTitle = T("");
		ofn.Flags = OFN_DONTADDTORECENT | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_NOCHANGEDIR;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = T(".tile");
		//http://soen.kr/lecture/win32api/reference/Structure/OPENFILENAME.htm
		if (GetOpenFileName(&ofn) != 0)
		{
			tstring filePath = ofn.lpstrFile;
			tstring fileName = filePath.substr(ofn.nFileOffset, filePath.size() - ofn.nFileOffset - 5);
			Load(fileName);
		}
	};

	load = new cButton(funcLoad, T("./image/BtBase"), D3DXVECTOR2(MYWINDOW->GetDeviceSize().x - 60, 20));
#pragma endregion

#pragma region ExtendButton
	auto v0Func = [&]()
	{
		if (heightLen > 1)
		{
			tiles.erase(tiles.end() - widthLen, tiles.end());
			heightLen--;
		}
	};

	vButton[0] = new cButton(v0Func, T("./image/BtVec0"), D3DXVECTOR2(MYWINDOW->GetDeviceSize().x - 70, 70));

	auto v1Func = [&]()
	{
		if (heightLen < MAXHEIGHTLEN)//최대 사이즈 100
		{
			tiles.resize(tiles.size() + widthLen);
			heightLen++;
		}
	};

	vButton[1] = new cButton(v1Func, T("./image/BtVec1"), D3DXVECTOR2(MYWINDOW->GetDeviceSize().x - 70, 110));

	auto h0Func = [&]()
	{
		if (widthLen > 1)//최대 사이즈 100
		{
			for (int i = heightLen; i != 0; i--)
			{
				tiles.erase(tiles.begin() + i * widthLen - 1);
			}
			widthLen--;
		}
	};

	hButton[0] = new cButton(h0Func, T("./image/BtHori0"), D3DXVECTOR2(MYWINDOW->GetDeviceSize().x - 90, 90));

	auto h1Func = [&]()
	{
		if (widthLen < MAXWIDTHLEN)//최대 사이즈 100
		{
			for (int i = 0; i < heightLen; i++)
			{
				tiles.insert(tiles.begin() + (i + 1) * widthLen + i, cTile());
			}
			widthLen++;
		}
	};

	hButton[1] = new cButton(h1Func, T("./image/BtHori1"), D3DXVECTOR2(MYWINDOW->GetDeviceSize().x - 50, 90));
#pragma endregion
}

cMapData::~cMapData()
{
	Release();
	SAFE_DELETE(editTexture);

	SAFE_DELETE(save);
	SAFE_DELETE(load);

	SAFE_DELETE(vButton[0]);
	SAFE_DELETE(vButton[1]);
	SAFE_DELETE(hButton[0]);
	SAFE_DELETE(hButton[1]);
}

void cMapData::Release()
{
	for (int i = 0; i < TRT_END; i++)
	{
		for (auto iter : image[i])
			SAFE_DELETE(iter);
		image[i].clear();
	}
}

void cMapData::Update()
{
	auto lButtonDown = KEYMANAGER->IsOnceKeyDown(VK_LBUTTON);
	auto rButtonDown = KEYMANAGER->IsOnceKeyDown(VK_RBUTTON);
	if (ptMouse.x < 900 && ptMouse.y < 600)
	{
		auto camPos = IMAGEMANAGER->GetCamPos();
		size_t xIdx = camPos.x + ptMouse.x / 90;
		size_t yIdx = camPos.y + ptMouse.y / 60;

		auto idxResult = MakeIdx(xIdx, yIdx);

		if (lButtonDown)
		{
			if (selectIdx == idxResult && idxResult != -1)
			{
				tiles[idxResult].type = (TileType)((tiles[idxResult].type + 1) % TT_END);
			}
			selectIdx = idxResult;
		}
		if (rButtonDown)
		{
			if (selectIdx == idxResult && idxResult != -1)
			{
				tiles[idxResult].type = (TileType)((tiles[idxResult].type - 1) % TT_END);
				if (tiles[idxResult].type == -1)
					tiles[idxResult].type = (TileType)(TT_END - 1);
			}
			selectIdx = idxResult;
		}
	}
	else if (ptMouse.x < 990 && ptMouse.y > MYWINDOW->GetDeviceSize().y - TILEHEIGHT * 2)
	{
		if (lButtonDown)
		{
			if (selectIdx != -1)
			{
				auto camPos = IMAGEMANAGER->GetCamPos();
				size_t xIdx = camPos.x + ptMouse.x / 90;
				size_t yIdx = camPos.y + (ptMouse.y - (MYWINDOW->GetDeviceSize().y - TILEHEIGHT * 2)) / 60;

				size_t idxResult = yIdx * 11 + xIdx;
				
				if (idxResult != 21)
				{
					tiles[selectIdx].type = (TileType)idxResult;
					tiles[selectIdx].textureNum = 0;
				}
			}
		}
	}
	else
	{
		if (lButtonDown || rButtonDown)
			selectIdx = -1;
	}
	save->Update();
	load->Update();

	vButton[0]->Update();
	vButton[1]->Update();
	hButton[0]->Update();
	hButton[1]->Update();
}

void cMapData::Render()
{
	for (int i = 0; i < heightLen; i++)
	{
		for (int j = 0; j < widthLen; j++)
		{
			auto& tile = tiles[MakeIdx(j, i)];
			auto textureEnum = tile.GetResourceType();
			if(textureEnum >= TRT_EDDL && textureEnum <= TRT_EDUR)
				image[tile.GetResourceType()][tile.textureNum]->Render(offsetPos.x + j * TILEWIDTH, offsetPos.y + i * TILEHEIGHT);
		}
	}

	for (int i = 0; i < heightLen; i++)
	{
		for (int j = 0; j < widthLen; j++)
		{
			auto& tile = tiles[MakeIdx(j, i)];
			auto textureEnum = tile.GetResourceType();
			if (!(textureEnum >= TRT_EDDL && textureEnum <= TRT_EDUR || textureEnum == TRT_END))
				image[tile.GetResourceType()][tile.textureNum]->Render(offsetPos.x + j * TILEWIDTH, offsetPos.y + i * TILEHEIGHT);
		}
	}

	for (int i = 0; i < heightLen; i++)
	{
		for (int j = 0; j < widthLen; j++)
		{
			if (selectIdx == MakeIdx(j, i))
				editTexture->Render(j * TILEWIDTH, i * TILEHEIGHT, (D3DCOLOR)0xffff00ff);
			else
				editTexture->Render(j * TILEWIDTH, i * TILEHEIGHT, (D3DCOLOR)0x80ffffff);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if (i == 1 && j == 10) break;
			if(selectIdx != -1 && tiles[selectIdx].type == i * 11 + j)
				editTexture->FixedRender(j * TILEWIDTH, MYWINDOW->GetDeviceSize().y - (2 - i) * TILEHEIGHT, (D3DCOLOR)0xffff00ff);
			else
				editTexture->FixedRender(j * TILEWIDTH, MYWINDOW->GetDeviceSize().y - (2 - i) * TILEHEIGHT);
		}
	}

	save->Render();
	load->Render();
	vButton[0]->Render();
	vButton[1]->Render();
	hButton[0]->Render();
	hButton[1]->Render();
}

void cMapData::TextureLoad(const tstring& textureName)
{
	Release();

	CFileFind file;
	TCHAR path[128];
	_stprintf(path, T("./image/mapTile/%s/*.png"), textureName.c_str());
	bool b = file.FindFile(path);

	vector<tstring> fileList;
	while (b)
	{
		b = file.FindNextFile();
		if (!file.IsDots())
		{
			auto fileName = file.GetFileName();
			fileList.push_back(fileName.GetBuffer());
		}
	}

	_stprintf(path, T("./image/mapTile/%s/"), textureName.c_str());
	for (size_t i = 0; i < fileList.size(); i++)
	{
		auto find = fileList[i].find(T('.'));
		switch (fileList[i][find - 1])
		{
		case T('c')://bsc
			image[TRT_BSC].push_back(new texture(path + fileList[i]));
			break;

		case T('D')://edD, slLD, slRD
			switch (fileList[i][find - 2])
			{
			case T('d')://오프셋이 필요함
			{
				TXTMANAGER->ReadOpen(path + fileList[i] + T(".txt"));
				int offsetX = -(TXTMANAGER->ReadLine<int>());
				TXTMANAGER->Close();
				image[TRT_EDDR].push_back(new texture(path + fileList[i], D3DXVECTOR2(offsetX, 0)));
				image[TRT_EDDL].push_back(new texture(path + fileList[i], D3DXVECTOR2(offsetX + TILEWIDTH, 0)));
			}
			break;
			case T('L'):
				image[TRT_SLLD].push_back(new texture(path + fileList[i]));
				break;
			case T('R'):
				image[TRT_SLRD].push_back(new texture(path + fileList[i]));
				break;
			}
			break;

		case T('U')://edU, slLU, slRU
			switch (fileList[i][find - 2])
			{
			case T('d')://오프셋이 필요함
			{
				TXTMANAGER->ReadOpen(path + fileList[i] + T(".txt"));
				int offsetX = -(TXTMANAGER->ReadLine<int>());
				TXTMANAGER->Close();
				auto addTexture1 = new texture(path + fileList[i], D3DXVECTOR2(offsetX, 0));
				auto addTexture2 = new texture(path + fileList[i], D3DXVECTOR2(offsetX + TILEWIDTH, 0));
				addTexture1->offset.y = -1 * (int)(addTexture1->info.Height) + TILEHEIGHT;
				addTexture2->offset.y = -1 * (int)(addTexture2->info.Height) + TILEHEIGHT;
				image[TRT_EDUR].push_back(addTexture1);
				image[TRT_EDUL].push_back(addTexture2);
			}
			break;
			case T('L'):
			{
				auto addTexture = new texture(path + fileList[i]);
				addTexture->offset.y = (int)addTexture->info.Height * -1 + TILEHEIGHT;
				image[TRT_SLLU].push_back(addTexture);
			}
			break;
			case T('R'):
			{
				auto addTexture = new texture(path + fileList[i]);
				addTexture->offset.y = (int)addTexture->info.Height * -1 + TILEHEIGHT;
				image[TRT_SLRU].push_back(addTexture);
			}
			break;
			}
			break;

		case T('0')://enH0, enV0
		{
			auto addTexture = new texture(path + fileList[i]);
			if (fileList[i][find - 2] == T('H'))
			{
				addTexture->offset.y = (int)addTexture->info.Height * -1 + TILEHEIGHT;
				image[TRT_ENH0].push_back(addTexture);
			}
			else
			{
				addTexture->offset.x = (int)addTexture->info.Width * -1 + TILEWIDTH;
				image[TRT_ENV0].push_back(addTexture);
			}
		}
		break;
		case T('1')://enH1, enV1
			if (fileList[i][find - 2] == T('H'))
				image[TRT_ENH1].push_back(new texture(path + fileList[i]));
			else
				image[TRT_ENV1].push_back(new texture(path + fileList[i]));
			break;
		}
	}

	file.Close();

	this->textureName = textureName;
}

void cMapData::Save(const tstring& name)
{
	TCHAR filePath[128];
	_stprintf(filePath, T("./map/%s"), name.c_str());
	_tmkdir(filePath);
	TXTMANAGER->WriteOpen((tstring)filePath + T(".map"));

	TXTMANAGER->WriteLine(offsetPos);
	TXTMANAGER->WriteLine(widthLen);
	TXTMANAGER->WriteLine(heightLen);

	TXTMANAGER->Close();
	_stprintf(filePath, T("./map/%s/tiles.png"), name.c_str());
	SaveMapImage(filePath);
	SaveMapGround(name);
	SaveMapCeiling(name);
	//SaveMapWall(num);

	SaveTileMap(name);
}

void cMapData::SaveMapImage(const tstring& filePath)
{
	LPDIRECT3DSURFACE9 backSurface;

	POINT temp = MYWINDOW->GetDeviceSize();
	MYWINDOW->ChangeResolution(widthLen * TILEWIDTH, heightLen * TILEHEIGHT);
	Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	Device->CreateOffscreenPlainSurface(widthLen * TILEWIDTH, heightLen * TILEHEIGHT, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &backSurface, NULL);

	if (SUCCEEDED(Device->BeginScene()))
	{
		IMAGEMANAGER->Begin();

		for (int i = 0; i < heightLen; i++)
		{
			for (int j = 0; j < widthLen; j++)
			{
				auto& tile = tiles[MakeIdx(j, i)];
				auto textureEnum = tile.GetResourceType();
				if (textureEnum >= TRT_EDDL && textureEnum <= TRT_EDUR)
					image[tile.GetResourceType()][tile.textureNum]->Render(j * TILEWIDTH, i * TILEHEIGHT);
			}
		}

		for (int i = 0; i < heightLen; i++)
		{
			for (int j = 0; j < widthLen; j++)
			{
				auto& tile = tiles[MakeIdx(j, i)];
				auto textureEnum = tile.GetResourceType();
				if (!(textureEnum >= TRT_EDDL && textureEnum <= TRT_EDUR || textureEnum == TRT_END))
					image[tile.GetResourceType()][tile.textureNum]->Render(j * TILEWIDTH, i * TILEHEIGHT);
			}
		}

		IMAGEMANAGER->End();

		Device->EndScene();
	}
	Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backSurface);

	D3DXSaveSurfaceToFile(filePath.c_str(), D3DXIFF_PNG, backSurface, nullptr, nullptr);

	SAFE_RELEASE(backSurface);
	MYWINDOW->ChangeResolution(temp.x, temp.y);
}

void cMapData::SaveMapGround(const tstring& groundName)
{
	auto mapLine = MakeMapGround();

	for (size_t i = 0; i < mapLine.size(); i++)
	{
		auto iter = mapLine[i];
		TCHAR filePath[128];
		_stprintf(filePath, T("./map/%s/%zd.ground"), groundName.c_str(), i);
		TXTMANAGER->WriteOpen(filePath);

		TXTMANAGER->WriteLine(iter->GetOffsetRef());
		for (auto mapIter : iter->GetMapLine())
		{
			TXTMANAGER->WriteLine(mapIter);
		}

		TXTMANAGER->Close();
	}
}

void cMapData::SaveMapCeiling(const tstring& ceilingName)
{
	auto mapLine = MakeMapCeiling();
	for (size_t i = 0; i < mapLine.size(); i++)
	{
		auto iter = mapLine[i];
		TCHAR filePath[128];
		_stprintf(filePath, T("./map/%s/%zd.ceiling"), ceilingName.c_str(), i);
		TXTMANAGER->WriteOpen(filePath);

		TXTMANAGER->WriteLine(iter->GetOffsetRef());
		for (auto mapIter : iter->GetMapLine())
		{
			TXTMANAGER->WriteLine(mapIter);
		}

		TXTMANAGER->Close();
	}
}

void cMapData::SaveTileMap(const tstring& tileName)
{
	TCHAR filePath[128];
	_stprintf(filePath, T("./tileMap/%s.tile"), tileName.c_str());
	TXTMANAGER->WriteOpen(filePath);

	TXTMANAGER->WriteLine(textureName);
	TXTMANAGER->WriteLine(offsetPos);
	tstring line;
	for (int y = 0; y < heightLen; y++)
	{
		line.clear();
		for (int x = 0; x < widthLen; x++)
		{
			line += tiles[MakeIdx(x, y)].type + 'a';
			line += tiles[MakeIdx(x, y)].textureNum + 'a';
		}
		TXTMANAGER->WriteLine(line);
	}

	TXTMANAGER->Close();
}

vector<cMapLine*> cMapData::MakeMapGround()
{
	//TT_GROUND, TT_LU, TT_RU, TT_EDUL, TT_EDUR(뒤에 두개는 가져다 붙이는 느낌
	vector<cMapLine*> result;

	vector<size_t> ptList;
	ptList.reserve(widthLen);

	for (int i = 0; i < tiles.size(); i++)
	{
		if (find(ptList.begin(), ptList.end(), i) != ptList.end()) continue;
		auto type = tiles[i].type;
		if (type >= TT_GROUND && type <= TT_EDUR)
		{
			cMapLine* newMap = new cMapLine();
			newMap->GetOffsetRef() = offsetPos;
			MakeMapGround(newMap, ptList, MakePt(i));//재귀함수를 통해 바닥 범위를 만듬
			newMap->Resort();//혹시 모르니 재정렬
			newMap->Optimize();//겹치는 점이나 일직선상의 점을 제거
			if (newMap->GetMapLine().empty())//어 쉬바 이거 머임 왜 바닥이 없어
				delete newMap;
			else
				result.push_back(newMap);//ㄱㅇㄷ
		}
	}

	return result;
}

vector<cMapLine*> cMapData::MakeMapCeiling()
{
	vector<cMapLine*> result;

	vector<size_t> ptList;
	ptList.reserve(widthLen);

	for (int i = 0; i < tiles.size(); i++)
	{
		if (find(ptList.begin(), ptList.end(), i) != ptList.end()) continue;
		auto type = tiles[i].type;
		if (type >= TT_CEILING_C && type <= TT_EDDR_C)
		{
			cMapLine* newMap = new cMapLine();
			newMap->GetOffsetRef() = offsetPos;
			MakeMapCeiling(newMap, ptList, MakePt(i));//재귀함수를 통해 바닥 범위를 만듬
			newMap->Resort();//혹시 모르니 재정렬
			newMap->Optimize();//겹치는 점이나 일직선상의 점을 제거
			if (newMap->GetMapLine().empty())//어 쉬바 이거 머임 왜 바닥이 없어
				delete newMap;
			else
				result.push_back(newMap);//ㄱㅇㄷ
		}
	}

	return result;
}

void cMapData::MakeMapGround(cMapLine* mapLine, vector<size_t>& ptList, const POINT& nowPt)
{
	size_t idx = MakeIdx(nowPt);
	if (!MyFunc::CheckEnablePt(nowPt, widthLen, heightLen))//맵사이즈를 벗어나는것도 예외처리
		return;
	if (find(ptList.begin(), ptList.end(), idx) != ptList.end())//리스트에 이미 있는넘은 예외처리
		return;

	ptList.push_back(idx);
	switch (tiles[idx].type)
	{
	case TT_EDUR:
	{
		cTile nowTile = tiles[MakeIdx(nowPt)];
		auto nowImage = image[nowTile.GetResourceType()][nowTile.textureNum];

		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH, nowPt.y * TILEHEIGHT + 30));
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + nowImage->offset.x + nowImage->info.Width, nowPt.y * TILEHEIGHT + 30));
		//잘린 판정범위 먼저 추가
		POINT pt = MyFunc::AddPt(nowPt, -1, 0);
		if (MyFunc::CheckEnablePt(pt, widthLen, heightLen))//왼쪽타일이 있을수 없는경우
		{
			cTile tile = tiles[MakeIdx(pt)];
			if (tile.type == TT_GROUND || tile.type == TT_RU)//왼쪽타일이 있음
			{
				MakeMapGround(mapLine, ptList, pt);
				break;
			}
		}
		
		pt = MyFunc::AddPt(nowPt, -1, 1);
		if (MyFunc::CheckEnablePt(pt, widthLen, heightLen))//왼쪽아래 타일이 있을수 없는경우
		{
			if (tiles[MakeIdx(pt)].type == TT_LU)
			{
				MakeMapGround(mapLine, ptList, pt);
				break;
			}
		}		
		//남은 부분 판정범위 추가
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + nowImage->offset.x, nowPt.y * TILEHEIGHT + 30));
		break;
	}
	case TT_EDUL:
	{
		cTile nowTile = tiles[MakeIdx(nowPt)];
		auto nowImage = image[nowTile.GetResourceType()][nowTile.textureNum];

		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + nowImage->offset.x, nowPt.y * TILEHEIGHT + 30));
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + TILEWIDTH, nowPt.y * TILEHEIGHT + 30));
		//잘린 판정범위 먼저 추가
		POINT pt = MyFunc::AddPt(nowPt, 1, 0);
		if (MyFunc::CheckEnablePt(pt, widthLen, heightLen))//오른쪽타일이 있을수 없는경우
		{
			auto tileType = tiles[MakeIdx(pt)].type;
			if (tileType == TT_GROUND || tileType == TT_LU)//오른쪽타일이 있음
			{
				MakeMapGround(mapLine, ptList, pt);
				break;
			}
		}

		pt = MyFunc::AddPt(nowPt, 1, 1);
		if (MyFunc::CheckEnablePt(pt, widthLen, heightLen))//오른쪽아래 타일이 있을수 없는경우
		{
			if (tiles[MakeIdx(pt)].type == TT_RU)
			{
				MakeMapGround(mapLine, ptList, pt);
				break;
			}
		}
		//남은 부분 판정범위 추가
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + nowImage->offset.x + nowImage->info.Width, nowPt.y * TILEHEIGHT + 30));
		break;
	}
	case TT_GROUND:
	{
		//판정범위 먼저 추가
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH, nowPt.y * TILEHEIGHT + 30));
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + TILEWIDTH, nowPt.y * TILEHEIGHT + 30));

		POINT pt1, pt2, pt3, pt4;//좌상단, 좌측, 우상단, 우측
		pt1 = MyFunc::AddPt(nowPt, -1, 0);
		pt2 = MyFunc::AddPt(nowPt, -1, 1);
		pt3 = MyFunc::AddPt(nowPt, 1, 0);
		pt4 = MyFunc::AddPt(nowPt, 1, 1);
		
		if (MyFunc::CheckEnablePt(pt1, widthLen, heightLen))//왼쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt1)].type;
			if (tileType == TT_GROUND || tileType == TT_RU || tileType == TT_EDUL)//왼쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt1);
			else if (MyFunc::CheckEnablePt(pt2, widthLen, heightLen))//왼쪽 아래타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt2)].type == TT_LU)
					MakeMapGround(mapLine, ptList, pt2);
			}
		}
		

		if (MyFunc::CheckEnablePt(pt3, widthLen, heightLen))//오른쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt3)].type;
			if (tileType == TT_GROUND || tileType == TT_LU || tileType == TT_EDUR)//왼쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt3);
			else if (MyFunc::CheckEnablePt(pt4, widthLen, heightLen))//오른쪽 아래타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt4)].type == TT_RU)
					MakeMapGround(mapLine, ptList, pt4);
			}
		}

		break;
	}
	case TT_LU:
	{
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH, nowPt.y * TILEHEIGHT + 30));
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + TILEWIDTH, nowPt.y * TILEHEIGHT - 30));

		POINT pt1, pt2, pt3, pt4;//좌상단, 좌측, 우상단, 우측
		pt1 = MyFunc::AddPt(nowPt, -1, 0);
		pt2 = MyFunc::AddPt(nowPt, -1, 1);
		pt3 = MyFunc::AddPt(nowPt, 1, -1);
		pt4 = MyFunc::AddPt(nowPt, 1, 0);

		if (MyFunc::CheckEnablePt(pt1, widthLen, heightLen))//왼쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt1)].type;
			if (tileType == TT_GROUND || tileType == TT_RU || tileType == TT_EDUL)//왼쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt1);
			else if (MyFunc::CheckEnablePt(pt2, widthLen, heightLen))//왼쪽 아래타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt2)].type == TT_LU)
					MakeMapGround(mapLine, ptList, pt2);
			}
		}

		if (MyFunc::CheckEnablePt(pt3, widthLen, heightLen))//오른쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt3)].type;
			if (tileType == TT_GROUND || tileType == TT_LU || tileType == TT_EDUR)//왼쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt3);
			else if (MyFunc::CheckEnablePt(pt4, widthLen, heightLen))//오른쪽 아래타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt4)].type == TT_RU)
					MakeMapGround(mapLine, ptList, pt4);
			}
		}
		
		break;
	}
	case TT_RU:
	{
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH, nowPt.y * TILEHEIGHT - 30));
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + TILEWIDTH, nowPt.y * TILEHEIGHT + 30));

		POINT pt1, pt2, pt3, pt4;//좌상단, 좌측, 우상단, 우측
		pt1 = MyFunc::AddPt(nowPt, -1, -1);
		pt2 = MyFunc::AddPt(nowPt, -1, 0);
		pt3 = MyFunc::AddPt(nowPt, 1, 0);
		pt4 = MyFunc::AddPt(nowPt, 1, 1);

		if (MyFunc::CheckEnablePt(pt1, widthLen, heightLen))//왼쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt1)].type;
			if (tileType == TT_GROUND || tileType == TT_RU || tileType == TT_EDUL)//왼쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt1);
			else if (MyFunc::CheckEnablePt(pt2, widthLen, heightLen))//왼쪽 아래타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt2)].type == TT_LU)
					MakeMapGround(mapLine, ptList, pt2);
			}
		}

		if (MyFunc::CheckEnablePt(pt3, widthLen, heightLen))//오른쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt3)].type;
			if (tileType == TT_GROUND || tileType == TT_LU || tileType == TT_EDUR)//왼쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt3);
			else if (MyFunc::CheckEnablePt(pt4, widthLen, heightLen))//오른쪽 아래타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt4)].type == TT_RU)
					MakeMapGround(mapLine, ptList, pt4);
			}
		}
		break;
	}
	}
}

void cMapData::MakeMapCeiling(cMapLine* mapLine, vector<size_t>& ptList, const POINT& nowPt)
{
	//TT_CEILING_C, TT_LD_C, TT_RD_C, TT_EDDL_C, TT_EDDR_C,
	size_t idx = MakeIdx(nowPt);
	if (!MyFunc::CheckEnablePt(nowPt, widthLen, heightLen))//맵사이즈를 벗어나는것도 예외처리
		return;
	if (find(ptList.begin(), ptList.end(), idx) != ptList.end())//리스트에 이미 있는넘은 예외처리
		return;

	ptList.push_back(idx);
	switch (tiles[idx].type)
	{
	case TT_EDDR_C:
	{
		cTile nowTile = tiles[MakeIdx(nowPt)];
		auto nowImage = image[nowTile.GetResourceType()][nowTile.textureNum];

		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH, nowPt.y * TILEHEIGHT + 10));
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + nowImage->offset.x + nowImage->info.Width, nowPt.y * TILEHEIGHT + 10));
		//잘린 판정범위 먼저 추가
		POINT pt = MyFunc::AddPt(nowPt, -1, 0);
		if (MyFunc::CheckEnablePt(pt, widthLen, heightLen))//왼쪽타일이 있을수 있는경우
		{
			cTile tile = tiles[MakeIdx(pt)];
			if (tile.type == TT_CEILING_C || tile.type == TT_RD_C)//왼쪽타일이 있음
			{
				MakeMapGround(mapLine, ptList, pt);
				break;
			}
		}

		pt = MyFunc::AddPt(nowPt, -1, -1);
		if (MyFunc::CheckEnablePt(pt, widthLen, heightLen))//왼쪽위에 타일이 있을수 있는경우
		{
			if (tiles[MakeIdx(pt)].type == TT_LD_C)
			{
				MakeMapGround(mapLine, ptList, pt);
				break;
			}
		}
		//남은 부분 판정범위 추가
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + nowImage->offset.x, nowPt.y * TILEHEIGHT + 10));
		break;
	}
	case TT_EDDL_C:
	{
		cTile nowTile = tiles[MakeIdx(nowPt)];
		auto nowImage = image[nowTile.GetResourceType()][nowTile.textureNum];

		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + nowImage->offset.x, nowPt.y * TILEHEIGHT + 10));
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + TILEWIDTH, nowPt.y * TILEHEIGHT + 10));
		//잘린 판정범위 먼저 추가
		POINT pt = MyFunc::AddPt(nowPt, 1, 0);
		if (MyFunc::CheckEnablePt(pt, widthLen, heightLen))//오른쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt)].type;
			if (tileType == TT_CEILING_C || tileType == TT_LD_C)//오른쪽타일이 있음
			{
				MakeMapGround(mapLine, ptList, pt);
				break;
			}
		}

		pt = MyFunc::AddPt(nowPt, 1, -1);
		if (MyFunc::CheckEnablePt(pt, widthLen, heightLen))//오른쪽위에 타일이 있을수 있는경우
		{
			if (tiles[MakeIdx(pt)].type == TT_RD_C)
			{
				MakeMapGround(mapLine, ptList, pt);
				break;
			}
		}
		//남은 부분 판정범위 추가
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + nowImage->offset.x + nowImage->info.Width, nowPt.y * TILEHEIGHT + 10));
		break;
	}
	case TT_CEILING_C:
	{
		//판정범위 먼저 추가
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH, nowPt.y * TILEHEIGHT + 10));
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + TILEWIDTH, nowPt.y * TILEHEIGHT + 10));

		POINT pt1, pt2, pt3, pt4;//좌측, 좌상단, 우측, 우상단
		pt1 = MyFunc::AddPt(nowPt, -1, 0);
		pt2 = MyFunc::AddPt(nowPt, -1, -1);
		pt3 = MyFunc::AddPt(nowPt, 1, 0);
		pt4 = MyFunc::AddPt(nowPt, 1, -1);

		if (MyFunc::CheckEnablePt(pt1, widthLen, heightLen))//왼쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt1)].type;
			if (tileType == TT_CEILING_C || tileType == TT_RD_C || tileType == TT_EDDL_C)//왼쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt1);
			else if (MyFunc::CheckEnablePt(pt2, widthLen, heightLen))//왼쪽 위타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt2)].type == TT_LD_C)
					MakeMapGround(mapLine, ptList, pt2);
			}
		}

		if (MyFunc::CheckEnablePt(pt3, widthLen, heightLen))//오른쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt3)].type;
			if (tileType == TT_CEILING_C || tileType == TT_LD_C || tileType == TT_EDDR_C)//오른쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt3);
			else if (MyFunc::CheckEnablePt(pt4, widthLen, heightLen))//오른쪽 위타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt4)].type == TT_RD_C)
					MakeMapGround(mapLine, ptList, pt4);
			}
		}

		break;
	}
	case TT_LD_C:
	{
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH, nowPt.y * TILEHEIGHT + 10));
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + TILEWIDTH, nowPt.y * TILEHEIGHT + 90));

		POINT pt1, pt2, pt3, pt4;//좌측, 좌상단, 우측, 우상단
		pt1 = MyFunc::AddPt(nowPt, -1, 0);
		pt2 = MyFunc::AddPt(nowPt, -1, -1);
		pt3 = MyFunc::AddPt(nowPt, 1, 0);
		pt4 = MyFunc::AddPt(nowPt, 1, -1);

		if (MyFunc::CheckEnablePt(pt1, widthLen, heightLen))//왼쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt1)].type;
			if (tileType == TT_CEILING_C || tileType == TT_RD_C || tileType == TT_EDDL_C)//왼쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt1);
			else if (MyFunc::CheckEnablePt(pt2, widthLen, heightLen))//왼쪽 위타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt2)].type == TT_LD_C)
					MakeMapGround(mapLine, ptList, pt2);
			}
		}

		if (MyFunc::CheckEnablePt(pt3, widthLen, heightLen))//오른쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt3)].type;
			if (tileType == TT_CEILING_C || tileType == TT_LD_C || tileType == TT_EDDR_C)//오른쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt3);
			else if (MyFunc::CheckEnablePt(pt4, widthLen, heightLen))//오른쪽 위타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt4)].type == TT_RD_C)
					MakeMapGround(mapLine, ptList, pt4);
			}
		}

		break;
	}
	case TT_RD_C:
	{
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH, nowPt.y * TILEHEIGHT + 90));
		mapLine->AddPoint(D3DXVECTOR2(nowPt.x * TILEWIDTH + TILEWIDTH, nowPt.y * TILEHEIGHT + 10));

		POINT pt1, pt2, pt3, pt4;//좌측, 좌상단, 우측, 우상단
		pt1 = MyFunc::AddPt(nowPt, -1, 0);
		pt2 = MyFunc::AddPt(nowPt, -1, -1);
		pt3 = MyFunc::AddPt(nowPt, 1, 0);
		pt4 = MyFunc::AddPt(nowPt, 1, -1);

		if (MyFunc::CheckEnablePt(pt1, widthLen, heightLen))//왼쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt1)].type;
			if (tileType == TT_CEILING_C || tileType == TT_RD_C || tileType == TT_EDDL_C)//왼쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt1);
			else if (MyFunc::CheckEnablePt(pt2, widthLen, heightLen))//왼쪽 위타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt2)].type == TT_LD_C)
					MakeMapGround(mapLine, ptList, pt2);
			}
		}

		if (MyFunc::CheckEnablePt(pt3, widthLen, heightLen))//오른쪽타일이 있을수 있는경우
		{
			auto tileType = tiles[MakeIdx(pt3)].type;
			if (tileType == TT_CEILING_C || tileType == TT_LD_C || tileType == TT_EDDR_C)//오른쪽타일이 있음
				MakeMapGround(mapLine, ptList, pt3);
			else if (MyFunc::CheckEnablePt(pt4, widthLen, heightLen))//오른쪽 위타일이 있을수 있는경우
			{
				if (tiles[MakeIdx(pt4)].type == TT_RD_C)
					MakeMapGround(mapLine, ptList, pt4);
			}
		}
		break;
	}
	}
}

void cMapData::Load(const tstring& name)
{
	TCHAR filePath[128];
	_stprintf(filePath, T("./tileMap/%s.tile"), name.c_str());
	if (TXTMANAGER->ReadOpen(filePath) == TXT_SUCCESS)
	{
		tiles.clear();
		active = true;
		widthLen = 0;
		heightLen = 0;
		textureName = TXTMANAGER->ReadLine();
		offsetPos = TXTMANAGER->ReadLine<D3DXVECTOR2>();

		tstring line;

		while (true)
		{
			line = TXTMANAGER->ReadLine();

			if (line.empty()) break;
			heightLen++;
			for (int i = 0; i < line.size(); i += 2)
			{
				tiles.push_back(cTile((TileType)(line[i] - 'a'), line[i + 1] - 'a'));
			}
			widthLen = line.size() / 2;
		}

		TXTMANAGER->Close();

		TextureLoad(textureName);
	}
	else
	{
		DEBUG_LOG("없는 타일맵 번호입니다");
	}
}

void cMapData::MakeAdjMapData()
{
	CFileFind file;
	TCHAR path[128];
	_stprintf(path, T("./map/*.*"));
	bool b = file.FindFile(path);

	while (b)
	{
		b = file.FindNextFile();
		if (file.IsDots() == TRUE || file.IsDirectory() == TRUE) continue;

		tstring filePath = file.GetFilePath().GetBuffer();
		TXTMANAGER->ReadOpen(filePath);

		D3DXVECTOR2 pos = TXTMANAGER->ReadLine<D3DXVECTOR2>();
		size_t width = TXTMANAGER->ReadLine<size_t>();
		size_t height = TXTMANAGER->ReadLine<size_t>();

		size_t startX = pos.x / MAPSIZEX;//시작포지션
		size_t startY = pos.y / MAPSIZEY;//시작포지션

		size_t endX = (pos.x + width) / MAPSIZEX;
		size_t endY = (pos.y + height) / MAPSIZEY;
		TXTMANAGER->Close();

		for (size_t y = startY; y <= endY; y++)
		{
			for (size_t x = startX; x <= endX; x++)
			{
				_stprintf(path, T("./adjMap/%zd.%zd.txt"), x, y);
				TXTMANAGER->SubseqWriteOpen(path);
				TXTMANAGER->WriteLine(filePath);
				TXTMANAGER->Close();
			}
		}
	}
}

void cMapData::DeleteAdjMapData()
{
	CFileFind file;
	TCHAR path[128];
	_stprintf(path, T("./adjMap/*.*"));
	bool b = file.FindFile(path);

	while (b)
	{
		b = file.FindNextFile();
		if (file.IsDots() == TRUE || file.IsDirectory() == TRUE) continue;
		DeleteFile(file.GetFilePath());
	}

	file.Close();
}
