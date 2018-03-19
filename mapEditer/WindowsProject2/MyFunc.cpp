#include "stdafx.h"
#include "MyFunc.h"

namespace MyFunc
{
	POINT MakePT(int x, int y)
	{
		POINT result;
		result.x = x;
		result.y = y;
		return result;
	}
	POINT AddPt(const POINT& pt, int x, int y)
	{
		POINT result;
		result.x = pt.x + x;
		result.y = pt.y + y;
		return result;
	}
	POINT AddPt(const POINT& pt1, const POINT& pt2)
	{
		POINT result;
		result.x = pt1.x + pt2.x;
		result.y = pt1.y + pt2.y;
		return result;
	}
	bool CheckEnablePt(const POINT& pt, size_t maxX, size_t maxY)
	{
		if (pt.x < 0) return false;
		if (pt.y < 0) return false;
		if (pt.x >= maxX) return false;
		if (pt.y >= maxY) return false;
		return true;
	}
}