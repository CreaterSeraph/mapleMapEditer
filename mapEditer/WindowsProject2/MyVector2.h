#pragma once

namespace MyVector2
{
	//double ccw(const D3DXVECTOR2& a, const D3DXVECTOR2& b);
	//
	////�� p�� �������� ���� b�� ���� a�� �ݽð� �����̸� ���, �ð�����̸� ����, �����̸� 0�� ��ȯ �Ѵ�.
	//double ccw(const D3DXVECTOR2& p, const D3DXVECTOR2& a, const D3DXVECTOR2& b);
	//
	////�� a, b�� �� c, d�� ������ �� ���� �� �� �̵��� �� ������ ��ġ���� Ȯ���Ѵ�.
	//bool paralleSegments(D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c, D3DXVECTOR2 d, D3DXVECTOR2& p);
	//
	//bool inBoundingRectangle(const D3DXVECTOR2& p, D3DXVECTOR2 a, D3DXVECTOR2 b);
	//
	//bool lineIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, const D3DXVECTOR2& c, const D3DXVECTOR2& d, D3DXVECTOR2& x);
	//
	//// - �� �� a, b�� ������ ���а� �� �� c, b�� ������ ������ p�� ��ȯ�Ѵ�.
	//// - ������ �������� ��� �ƹ����̳� ��ȯ�Ѵ�.
	//bool segmentIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, const D3DXVECTOR2& c, const D3DXVECTOR2& d, D3DXVECTOR2& p);
	//
	//bool segmentIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, D3DXVECTOR2* now, const D3DXVECTOR2& will);
	//
	//��ó: http://bowbowbow.tistory.com/17 [�۸۸�]
	
	bool GetIntersectPoint(const D3DXVECTOR2& AP1, const D3DXVECTOR2& AP2, const D3DXVECTOR2& BP1, const D3DXVECTOR2& BP2, D3DXVECTOR2* IP);
}

using namespace MyVector2;