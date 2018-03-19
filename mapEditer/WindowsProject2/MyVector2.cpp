#include "stdafx.h"
#include "MyVector2.h"

namespace MyVector2
{
	//double ccw(const D3DXVECTOR2& a, const D3DXVECTOR2& b)
	//{
	//	return a.x * b.y - a.y * b.x;
	//}
	////점 p를 기준으로 벡터 b가 벡터 a의 반시계 방향이면 양수, 시계방향이면 음수, 평행이면 0을 반환 한다.
	//double ccw(const D3DXVECTOR2& p, const D3DXVECTOR2& a, const D3DXVECTOR2& b)
	//{
	//	return ccw(a - p, b - p);
	//}
	//
	////점 a, b와 점 c, d가 평행한 두 선분 일 때 이들이 한 점에서 겹치는지 확인한다.
	//bool paralleSegments(D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c, D3DXVECTOR2 d, D3DXVECTOR2& p)
	//{
	//	if (b < a) swap(a, b);
	//	if (d < c) swap(c, d);
	//
	//	//한 직선위에 없거나 두 선분이 겹치지 않는 경우를 우선 걸러낸다. 본문의 1번 관계인 경우이다.
	//	if (ccw(a, b, c) != 0 || b < c || d < a) return false;
	//
	//	//두 선분이 확실히 겹친다면 교차점 하나를 찾는다.
	//	p = a < c ? c : a;
	//	return true;
	//}
	//
	//bool inBoundingRectangle(const D3DXVECTOR2& p, D3DXVECTOR2 a, D3DXVECTOR2 b)
	//{
	//	if (b < a) swap(a, b);
	//	return p == a || p == b || (a <p && p < b);
	//}
	//
	//bool lineIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, const D3DXVECTOR2& c, const D3DXVECTOR2& d, D3DXVECTOR2& x)
	//{
	//	double det = (b - a).x*(d - c).y - (b - a).y * (d - c).x;
	//	//두선이 평행인 경우
	//	if (fabs(det) < 0.00001) return false;
	//	x = a + (b - a)* (((c - a).x * (d - c).y - (c - a).y * (d - c).x) / det);
	//	return true;
	//}
	//
	//// - 두 점 a, b를 지나는 선분과 두 점 c, b를 지나는 선분을 p에 반환한다.
	//// - 교짐이 여러개일 경우 아무점이나 반환한다.
	//bool segmentIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, const D3DXVECTOR2& c, const D3DXVECTOR2& d, D3DXVECTOR2& p)
	//{
	//	//두 직선이 평행인 경우를 우선 예외로 처리한다.
	//	if (!lineIntersection(a, b, c, d, p))
	//		return paralleSegments(a, b, c, d, p);
	//	//p가 두 선분에 포함되어 있는 경우에만 참을 반환한다.
	//	return inBoundingRectangle(p, a, b) && inBoundingRectangle(p, c, d);
	//}
	//
	//bool segmentIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, D3DXVECTOR2* now, const D3DXVECTOR2& will)
	//{
	//	return segmentIntersection(a, b, *now, will, *now);
	//}
	//출처: http://bowbowbow.tistory.com/17 [멍멍멍]

	bool GetIntersectPoint(const D3DXVECTOR2& AP1, const D3DXVECTOR2& AP2, const D3DXVECTOR2& BP1, const D3DXVECTOR2& BP2, D3DXVECTOR2* IP)
	{
		double t;
		double s;
		double under = (BP2.y - BP1.y)*(AP2.x - AP1.x) - (BP2.x - BP1.x)*(AP2.y - AP1.y);
		if (under == 0) return false;

		double _t = (BP2.x - BP1.x)*(AP1.y - BP1.y) - (BP2.y - BP1.y)*(AP1.x - BP1.x);
		double _s = (AP2.x - AP1.x)*(AP1.y - BP1.y) - (AP2.y - AP1.y)*(AP1.x - BP1.x);

		t = _t / under;
		s = _s / under;

		if (t<0.0 || t>1.0 || s<0.0 || s>1.0) return false;
		if (_t == 0 && _s == 0) return false;

		IP->x = AP1.x + t * (double)(AP2.x - AP1.x);
		IP->y = AP1.y + t * (double)(AP2.y - AP1.y);

		return true;
	}
	//출처: http://www.gisdeveloper.co.kr/?p=89
}