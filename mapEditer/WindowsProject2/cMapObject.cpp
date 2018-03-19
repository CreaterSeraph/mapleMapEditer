#include "stdafx.h"
#include "cMapObject.h"

struct MapLineLessFunc
{
	bool operator()(const D3DXVECTOR2& pos1, const D3DXVECTOR2& pos2) const
	{
		return pos1.x < pos2.x;
	}
};

size_t cMapLine::AddPoint(const D3DXVECTOR2& pos)
{
	auto iter = mapLine.insert(upper_bound(mapLine.begin(), mapLine.end(), pos, MapLineLessFunc()), pos);

	return distance(mapLine.begin(), iter);
}

bool cMapLine::IsCollide(D3DXVECTOR2* pos, const D3DXVECTOR2& speed) const
{
	D3DXVECTOR2 willPos = *pos + speed;
	auto iter1 = lower_bound(mapLine.begin(), mapLine.end(), *pos - offset, MapLineLessFunc());
	auto iter2 = upper_bound(mapLine.begin(), mapLine.end(), willPos - offset, MapLineLessFunc());

	int value;
	if (speed.x < 0)
	{
		iter2--;
		value = -1;
	}
	else
	{
		iter1--;
		value = 1;
	}
	
	while (iter1 != iter2)
	{
		if (MyVector2::GetIntersectPoint(*iter1 + offset, *(iter1 + value) + offset, *pos, willPos, pos))
			return true;
		iter1 += value;
	}
	return false;
}

bool cMapLine::MoveAlongALine(D3DXVECTOR2* pos, const D3DXVECTOR2& speed) const
{
	D3DXVECTOR2 willPos = *pos + speed;

	if (willPos.x < mapLine.front().x || willPos.x > mapLine.back().x)
		return false;
	auto iter = upper_bound(mapLine.begin(), mapLine.end(), willPos - offset, MapLineLessFunc());
	auto front = *(iter - 1);
	auto back = *iter;

	double resultY = front.y + (willPos.x - front.x) / (back.x - front.x) * (back.y - front.y);
	pos->x = willPos.x;
	pos->y = resultY;

	return true;
}

void cMapLine::Resort()
{
	sort(mapLine.begin(), mapLine.end(), MapLineLessFunc());
}

void cMapLine::Optimize()
{
	if (mapLine.size() < 3) return;
	double inc;//기울기
	size_t idx = 0;

	while (mapLine[idx + 1].x - mapLine[idx].x < EPSION)
	{
		mapLine.erase(mapLine.begin());
	}
	inc = (mapLine[idx + 1].y - mapLine[idx].y) / (mapLine[idx + 1].x - mapLine[idx].x);//같은 x값변화에 따른 y값 비율

	idx++;
	size_t size = mapLine.size();
	while (idx < size - 1)
	{
		if (mapLine[idx + 1].x - mapLine[idx].x < EPSION)
		{
			mapLine.erase(mapLine.begin() + idx);
			size--;
			continue;
		}
		double tempInc = (mapLine[idx + 1].y - mapLine[idx].y) / (mapLine[idx + 1].x - mapLine[idx].x);
		if (abs(inc - tempInc) < EPSION)//비율이 같으면 중간에 있는 점을 슥삭한다
		{
			mapLine.erase(mapLine.begin() + idx);
			size--;
			continue;
		}
		inc = tempInc;
		idx++;
	}
}

cMapLine::cMapLine()
{
}

cMapLine::~cMapLine()
{
}

bool cPortal::IsCollide(const D3DXVECTOR2& pos) const
{
	return false;
}

void cPortal::Render()
{
	image->Render(pos.x, pos.y);
}

cPortal::cPortal(const D3DXVECTOR2& pos, const string& name, int destNum, const D3DXVECTOR2& destPos, int portalStyle)
	:pos(pos), name(name), destNum(destNum), destPos(destPos), portalStyle(portalStyle)
{
	char imageKey[32];
	sprintf(imageKey, "map_portal_%d", portalStyle);
	image = IMAGEMANAGER->FindImage(imageKey);
}

bool cWall::IsCollide(D3DXVECTOR2* pos, const D3DXVECTOR2& speed, bool gravity) const
{
	if (colType == WT_NONGRV)
	{
		if (gravity == true)
			return false;
	}
	if (colType == WT_GRV)
	{
		if (gravity == false)
			return false;
	}
	switch (type)
	{
	}
	
	D3DXVECTOR2 willPos = *pos + speed;
	return (MyVector2::GetIntersectPoint(startPos, D3DXVECTOR2(startPos.x, startPos.y + height), *pos, willPos, pos));
}

cWall::cWall(const D3DXVECTOR2& startPos, double height)
	:startPos(startPos), height(height)
{
}


cMapObject::cMapObject()
{
}


cMapObject::~cMapObject()
{
	Release();
}

void cMapObject::Init()
{
	ground.push_back(new cMapLine());

	auto temp = ground.back();
	//temp->AddPoint(D3DXVECTOR2(000, 300));
	//temp->AddPoint(D3DXVECTOR2(100, 100));
	//temp->AddPoint(D3DXVECTOR2(200, 300));
	//temp->AddPoint(D3DXVECTOR2(300, 200));
	//temp->AddPoint(D3DXVECTOR2(400, 400));
	//temp->AddPoint(D3DXVECTOR2(500, 300));
	//temp->AddPoint(D3DXVECTOR2(600, 100));
	//temp->AddPoint(D3DXVECTOR2(700, 350));
	//temp->AddPoint(D3DXVECTOR2(800, 222));

	temp->AddPoint(D3DXVECTOR2(000, 100));
	temp->AddPoint(D3DXVECTOR2(100, 100));
	temp->AddPoint(D3DXVECTOR2(200, 100));
	temp->AddPoint(D3DXVECTOR2(300, 200));
	temp->AddPoint(D3DXVECTOR2(400, 300));
	temp->AddPoint(D3DXVECTOR2(500, 200));
	temp->AddPoint(D3DXVECTOR2(600, 100));
	temp->AddPoint(D3DXVECTOR2(700, 100));

	temp->Optimize();

	state = false;
}

void cMapObject::Release()
{
	for (auto iter : ground)
	{
		SAFE_DELETE(iter);
	}
}

void cMapObject::Update()
{
	if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
	{
		if (state)
		{
			pos2.x = ptMouse.x;
			pos2.y = ptMouse.y;

			DEBUG_LOG(pos1.x << "\t" << pos1.y);
			DEBUG_LOG(pos2.x << "\t" << pos2.y);

			ground.back()->IsCollide(&pos1, pos2 - pos1);
			DEBUG_LOG(pos1.x << "\t" << pos1.y);

			state = false;
		}
		else
		{
			pos1.x = ptMouse.x;
			pos1.y = ptMouse.y;
			state = true;
		}
	}
}

void cMapObject::Render()
{
	texture temp(T("./image/dest.png"));
	texture rect(T("./image/rect.png"));
	temp.Load();
	rect.Load();
	rect.offset.x = rect.info.Width / 2;
	rect.offset.y = rect.info.Height;

	temp.Render(0, 0);
	rect.Render(pos1.x, pos1.y);
}