#include "stdafx.h"
#include "MyVector2.h"

namespace MyVector2
{
	//double ccw(const D3DXVECTOR2& a, const D3DXVECTOR2& b)
	//{
	//	return a.x * b.y - a.y * b.x;
	//}
	////�� p�� �������� ���� b�� ���� a�� �ݽð� �����̸� ���, �ð�����̸� ����, �����̸� 0�� ��ȯ �Ѵ�.
	//double ccw(const D3DXVECTOR2& p, const D3DXVECTOR2& a, const D3DXVECTOR2& b)
	//{
	//	return ccw(a - p, b - p);
	//}
	//
	////�� a, b�� �� c, d�� ������ �� ���� �� �� �̵��� �� ������ ��ġ���� Ȯ���Ѵ�.
	//bool paralleSegments(D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c, D3DXVECTOR2 d, D3DXVECTOR2& p)
	//{
	//	if (b < a) swap(a, b);
	//	if (d < c) swap(c, d);
	//
	//	//�� �������� ���ų� �� ������ ��ġ�� �ʴ� ��츦 �켱 �ɷ�����. ������ 1�� ������ ����̴�.
	//	if (ccw(a, b, c) != 0 || b < c || d < a) return false;
	//
	//	//�� ������ Ȯ���� ��ģ�ٸ� ������ �ϳ��� ã�´�.
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
	//	//�μ��� ������ ���
	//	if (fabs(det) < 0.00001) return false;
	//	x = a + (b - a)* (((c - a).x * (d - c).y - (c - a).y * (d - c).x) / det);
	//	return true;
	//}
	//
	//// - �� �� a, b�� ������ ���а� �� �� c, b�� ������ ������ p�� ��ȯ�Ѵ�.
	//// - ������ �������� ��� �ƹ����̳� ��ȯ�Ѵ�.
	//bool segmentIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, const D3DXVECTOR2& c, const D3DXVECTOR2& d, D3DXVECTOR2& p)
	//{
	//	//�� ������ ������ ��츦 �켱 ���ܷ� ó���Ѵ�.
	//	if (!lineIntersection(a, b, c, d, p))
	//		return paralleSegments(a, b, c, d, p);
	//	//p�� �� ���п� ���ԵǾ� �ִ� ��쿡�� ���� ��ȯ�Ѵ�.
	//	return inBoundingRectangle(p, a, b) && inBoundingRectangle(p, c, d);
	//}
	//
	//bool segmentIntersection(const D3DXVECTOR2& a, const D3DXVECTOR2& b, D3DXVECTOR2* now, const D3DXVECTOR2& will)
	//{
	//	return segmentIntersection(a, b, *now, will, *now);
	//}
	//��ó: http://bowbowbow.tistory.com/17 [�۸۸�]

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
	//��ó: http://www.gisdeveloper.co.kr/?p=89
}