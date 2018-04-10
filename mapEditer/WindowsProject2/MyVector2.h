#pragma once

namespace MyVector2
{
	//double ccw(const D3DXVECTOR2& a, const D3DXVECTOR2& b);
	//
	////점 p를 기준으로 벡터 b가 벡터 a의 반시계 방향이면 양수, 시계방향이면 음수, 평행이면 0을 반환 한다.
	//double ccw(const D3DXVECTOR2& p, const D3DXVECTOR2& a, const D3DXVECTOR2& b);
	//
	////점 a, b와 점 c, d가 평행한 두 선분 일 때 이들이 한 점에서 겹치는지 확인한다.
	//bool paralleSegments(D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c, D3DXVECTOR2 d, D3DXVECTOR2& p);
	//
	//bool inBoundingRectangle(const D3DXVECTOR2& p, D3DXVECTOR2 a, D3DXVECTOR2 b);
	//
	//bool lineIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, const D3DXVECTOR2& c, const D3DXVECTOR2& d, D3DXVECTOR2& x);
	//
	//// - 두 점 a, b를 지나는 선분과 두 점 c, b를 지나는 선분을 p에 반환한다.
	//// - 교짐이 여러개일 경우 아무점이나 반환한다.
	//bool segmentIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, const D3DXVECTOR2& c, const D3DXVECTOR2& d, D3DXVECTOR2& p);
	//
	//bool segmentIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, D3DXVECTOR2* now, const D3DXVECTOR2& will);
	//
	//출처: http://bowbowbow.tistory.com/17 [멍멍멍]
	
	bool GetIntersectPoint(const D3DXVECTOR2& AP1, const D3DXVECTOR2& AP2, const D3DXVECTOR2& BP1, const D3DXVECTOR2& BP2, D3DXVECTOR2* IP);
}

using namespace MyVector2;