#pragma once

class cMapLine
{
private:
	vector<D3DXVECTOR2> mapLine;
	D3DXVECTOR2 offset;
	int lineNum;
public:
	const vector<D3DXVECTOR2>& GetMapLine() { return mapLine; }
	D3DXVECTOR2& GetPosRef(size_t idx) { return mapLine[idx]; }
	D3DXVECTOR2& GetOffsetRef() { return offset; }

	size_t AddPoint(const D3DXVECTOR2& pos);
	bool IsCollide(D3DXVECTOR2* pos, const D3DXVECTOR2& speed) const;
	bool MoveAlongALine(D3DXVECTOR2* pos, const D3DXVECTOR2& speed) const;
	void Resort();
	void Optimize();

	cMapLine();
	~cMapLine();
};

class cPortal
{
private:
	D3DXVECTOR2 pos;
	string name;
	int destNum;//목적지 번호
	D3DXVECTOR2 destPos;//목적지 포지션

	texture* image;

	int portalStyle;
public:
	bool IsCollide(const D3DXVECTOR2& pos) const;
	const string& GetName() { return name; }

	void Render();

	cPortal(const D3DXVECTOR2& pos, const string& name, int destNum, const D3DXVECTOR2& destPos, int portalStyle = 0);
};

class cWall
{
private:
	D3DXVECTOR2 startPos;
	double height;

	enum TYPE : char
	{
		WT_NONGRV = 1, WT_GRV, WT_ALL
	} colType;
	int type;
public:
	bool IsCollide(D3DXVECTOR2* pos, const D3DXVECTOR2& speed, bool gravity) const;

	cWall(const D3DXVECTOR2& startPos, double height);
};

class cMapObject
{
private:
	vector<cMapLine*> ground;	//바닥오브젝트
	vector<cMapLine*> ceiling;	//천장오브젝트(ceiling-한계, 천장)
	//벽오브젝트
	vector<cWall*> walls;
	//포탈오브젝트
	vector<cPortal*> portal;

	D3DXVECTOR2 pos1, pos2;
	bool state;
public:
	void Init();
	void Release();
	void Update();
	void Render();
	//지점에 인접한곳찾기
	//유닛의 스피드를 받아서 이동하기

	cMapObject();
	~cMapObject();
};

