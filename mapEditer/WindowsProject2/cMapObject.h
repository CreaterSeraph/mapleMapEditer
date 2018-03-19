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
	int destNum;//������ ��ȣ
	D3DXVECTOR2 destPos;//������ ������

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
	vector<cMapLine*> ground;	//�ٴڿ�����Ʈ
	vector<cMapLine*> ceiling;	//õ�������Ʈ(ceiling-�Ѱ�, õ��)
	//��������Ʈ
	vector<cWall*> walls;
	//��Ż������Ʈ
	vector<cPortal*> portal;

	D3DXVECTOR2 pos1, pos2;
	bool state;
public:
	void Init();
	void Release();
	void Update();
	void Render();
	//������ �����Ѱ�ã��
	//������ ���ǵ带 �޾Ƽ� �̵��ϱ�

	cMapObject();
	~cMapObject();
};

