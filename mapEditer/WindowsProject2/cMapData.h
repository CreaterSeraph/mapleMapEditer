#pragma once

enum TileResourceType
{
	TRT_BSC,
	TRT_EDDL, TRT_EDDR, TRT_EDUL, TRT_EDUR,
	TRT_ENH0, TRT_ENH1,
	TRT_ENV0, TRT_ENV1,
	TRT_SLLD, TRT_SLLU, TRT_SLRD, TRT_SLRU,
	TRT_END
};

enum TileType
{
	TT_EMPTY, TT_BSC,
	TT_GROUND, TT_LU, TT_RU, TT_EDUL, TT_EDUR,
	TT_CEILING, TT_LD, TT_RD, TT_EDDL, TT_EDDR,
	TT_CEILING_C, TT_LD_C, TT_RD_C, TT_EDDL_C, TT_EDDR_C,
	TT_LEFT, TT_RIGHT, TT_LEFT_C, TT_RIGHT_C,
	TT_END
};

struct cTile
{
	TileType type;

	size_t textureNum;

	cTile(TileType type = TT_EMPTY, size_t textureNum = 0)
		:type(type), textureNum(textureNum)
	{
	}

	TileResourceType GetResourceType()
	{
		return (TileResourceType)(*("nafkmdegjlbcgjlbdhihi" + type) - 'a');
	}
};

#define TILEWIDTH 90
#define TILEHEIGHT 60

#define MAPSIZEX 2400
#define MAPSIZEY 1350

#define MAXWIDTHLEN 100
#define MAXHEIGHTLEN 100

class cButton;

class cMapLine;
class cWall;
class cMapData
{
private:
	bool active;
	//텍스쳐 로드를 위해 사용될 번호(타일 이미지의 번호)
	tstring textureName;
	vector<texture*> image[TRT_END];

	vector<cTile> tiles;//2차원 타일 벡터
	size_t widthLen;//타일 사이즈
	size_t heightLen;
	
	D3DXVECTOR2 offsetPos;//시작포지션

	size_t lineNum;//충돌덩어리 번호

	texture* editTexture;

	size_t selectIdx;

	cButton* save;
	cButton* load;
	cButton* vButton[2];
	cButton* hButton[2];

	//POINT camPos;
private:
	size_t MakeIdx(size_t x, size_t y);
	size_t MakeIdx(const POINT& pt);
	POINT MakePt(size_t idx);

	void MakeMapGround(cMapLine* mapLine, vector<size_t>& ptList, const POINT& nowPt);
	void MakeMapCeiling(cMapLine* mapLine, vector<size_t>& ptList, const POINT& nowPt);
	void MakeMapWall(vector<size_t>& ptList, const POINT& nowPt);
public:
	cMapData(size_t width, size_t height, const D3DXVECTOR2& offset);
	cMapData(vector<cTile>&& tiles, size_t width, const D3DXVECTOR2& offset);
	cMapData(const tstring& name);
	cMapData();
	~cMapData();

	void Init();
	void TextureLoad(const tstring& textureName);

	void Release();

	void Update();
	void Render();

	void SaveMapImage(const tstring& filePath);
	vector<cMapLine*> MakeMapGround();
	vector<cMapLine*> MakeMapCeiling();
	vector<cWall*> MakeMapWall();

	void Save(const tstring& name);
	void SaveMapGround(const tstring& groundName);
	void SaveMapCeiling(const tstring& ceilingName);
	void SaveMapWall(const tstring& wallName);
	void SaveTileMap(const tstring& tileName);

	void Load(const tstring& name);

	static void MakeAdjMapData();
	static void DeleteAdjMapData();
};

