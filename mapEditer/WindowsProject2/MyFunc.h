#pragma once

namespace MyFunc
{
	POINT MakePT(int x, int y);
	POINT AddPt(const POINT& pt, int x, int y);
	POINT AddPt(const POINT& pt1, const POINT& pt2);
	bool CheckEnablePt(const POINT& pt, size_t maxX, size_t maxY);
}