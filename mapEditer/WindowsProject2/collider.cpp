#include "stdafx.h"
#include "collider.h"

sCircleCollider::sCircleCollider(const D3DXVECTOR2 & pos, const D3DXVECTOR2 & offset, collidersize r, collidersize rot)
	:r(r), pos(pos), offset(offset), rot(rot), scale(1)
{
}

sRectCollider::sRectCollider(const D3DXVECTOR2& pos, const D3DXVECTOR2& offset, const D3DXVECTOR2& size)
	:pos(pos), offset(offset), size(size), scale(1, 1)
{
}

sLineCollider::sLineCollider(const D3DXVECTOR2& pos1, const D3DXVECTOR2& pos2, collidersize weight)
	:pos1(pos1), pos2(pos2), weight(weight)
{
}

bool sCircleCollider::IsCollide(const sCircleCollider& collider) const
{
	D3DXVECTOR2 offset1, offset2;
	D3DXMATRIXA16 matRot1, matRot2;
	D3DXMatrixRotationZ(&matRot1, rot);
	D3DXMatrixRotationZ(&matRot2, collider.rot);
	D3DXVec2TransformCoord(&offset1, &offset, &matRot1);
	D3DXVec2TransformCoord(&offset2, &collider.offset, &matRot2);

	double lenX = (offset1.x * scale + pos.x) - (offset2.x * scale + collider.pos.x);
	double lenY = (offset1.y * scale + pos.y) - (offset2.y * scale + collider.pos.y);

	return pow(lenX, 2) + pow(lenY, 2) < pow((r + collider.r) * scale, 2);
}

bool sCircleCollider::IsCollide(const sRectCollider& collider) const
{
	D3DXVECTOR2 offset1;
	D3DXMATRIXA16 matRot1;
	D3DXMatrixRotationZ(&matRot1, rot);
	D3DXVec2TransformCoord(&offset1, &offset, &matRot1);

	collidersize resultX = abs((collider.pos.x + collider.offset.x * collider.scale.x) -
		(offset1.x * scale + pos.x)) - collider.size.x * collider.scale.x / 2;
	collidersize resultY = abs((collider.pos.y + collider.offset.y * collider.scale.y) -
		(offset1.y * scale + pos.y)) - collider.size.y * collider.scale.y / 2;

	collidersize circleSize = r * scale;

	if (resultX > circleSize || resultY > circleSize)
		return false;

	if (resultX > 0)
	{
		if (resultY < 0)
			return true;
		else
			return pow(resultX, 2) * pow(resultY, 2) < pow(circleSize, 2);
	}
	return true;
}

bool sCircleCollider::IsCollide(const sLineCollider& collider) const
{
	float resultLen;

	double lineLen = D3DXVec2Length(&(collider.pos1 - collider.pos2));
	if (lineLen == 0)
		resultLen = D3DXVec2Length(&(collider.pos1 - pos));
	else
	{
		double prj = ((pos.x - collider.pos1.x) * (collider.pos2.x - collider.pos1.x)
			+ (pos.y - collider.pos1.y) * (collider.pos2.y - collider.pos1.y)) / lineLen;
		if (prj < 0)
			resultLen = D3DXVec2Length(&(collider.pos1 - pos));
		else if (prj > lineLen)
			resultLen = D3DXVec2Length(&(collider.pos2 - pos));
		else
			resultLen = fabs((-1)*(pos.x - collider.pos1.x)*(collider.pos2.y - collider.pos1.y)
				+ (pos.y - collider.pos1.y)*(collider.pos2.x - collider.pos1.x)) / lineLen;
	}

	//http://kipl.tistory.com/56 [Geometry & Recognition]

	return resultLen < collider.weight + this->r * this->scale;
}

bool sRectCollider::IsCollide(const sCircleCollider& collider) const
{
	return collider.IsCollide(*this);
}

bool sRectCollider::IsCollide(const sRectCollider& collider) const
{
	double rect1[4];
	double rect2[4];

	rect1[0] = pos.x - (offset.x + size.x / 2) * scale.x;
	rect1[2] = pos.x - (offset.x - size.x / 2) * scale.x;
	rect1[1] = pos.y - (offset.y + size.y / 2) * scale.y;
	rect1[3] = pos.y - (offset.y - size.y / 2) * scale.y;

	rect2[0] = collider.pos.x - (collider.offset.x + collider.size.x / 2) * collider.scale.x;
	rect2[2] = collider.pos.x - (collider.offset.x - collider.size.x / 2) * collider.scale.x;
	rect2[1] = collider.pos.y - (collider.offset.y + collider.size.y / 2) * collider.scale.y;
	rect2[3] = collider.pos.y - (collider.offset.y - collider.size.y / 2) * collider.scale.y;

	if (rect1[2] < rect2[0] || rect1[0] > rect2[2]) return false;
	if (rect1[3] < rect2[1] || rect1[1] > rect2[3]) return false;

	return true;
}

bool sRectCollider::IsCollide(const sLineCollider& collider) const
{
	DEBUG_LOG("구현되지 않은 부분입니다");
	return false;
}

bool sLineCollider::IsCollide(const sCircleCollider& collider) const
{
	return collider.IsCollide(*this);
}

bool sLineCollider::IsCollide(const sRectCollider& collider) const
{
	return collider.IsCollide(*this);
}

bool sLineCollider::IsCollide(const sLineCollider& collider) const
{
	DEBUG_LOG("구현되지 않은 부분입니다");
	return false;
}

#pragma region 연산자오버로딩
bool sCircleCollider::operator==(const sCircleCollider& collider) const
{
	return IsCollide(collider);
}

bool sCircleCollider::operator==(const sRectCollider& collider) const
{
	return IsCollide(collider);
}

bool sCircleCollider::operator==(const sLineCollider & collider) const
{
	return IsCollide(collider);
}

bool sCircleCollider::operator!=(const sCircleCollider& collider) const
{
	return !IsCollide(collider);
}

bool sCircleCollider::operator!=(const sRectCollider& collider) const
{
	return !IsCollide(collider);
}

bool sCircleCollider::operator!=(const sLineCollider & collider) const
{
	return !IsCollide(collider);
}

bool sRectCollider::operator==(const sCircleCollider& collider) const
{
	return IsCollide(collider);
}

bool sRectCollider::operator==(const sRectCollider& collider) const
{
	return IsCollide(collider);
}

bool sRectCollider::operator==(const sLineCollider & collider) const
{
	return IsCollide(collider);
}

bool sRectCollider::operator!=(const sCircleCollider& collider) const
{
	return !IsCollide(collider);
}

bool sRectCollider::operator!=(const sRectCollider& collider) const
{
	return !IsCollide(collider);
}
bool sRectCollider::operator!=(const sLineCollider & collider) const
{
	return !IsCollide(collider);
}

bool sLineCollider::operator==(const sCircleCollider& collider) const
{
	return IsCollide(collider);
}

bool sLineCollider::operator==(const sRectCollider& collider) const
{
	return IsCollide(collider);
}

bool sLineCollider::operator==(const sLineCollider& collider) const
{
	return IsCollide(collider);
}

bool sLineCollider::operator!=(const sCircleCollider& collider) const
{
	return !IsCollide(collider);
}

bool sLineCollider::operator!=(const sRectCollider& collider) const
{
	return !IsCollide(collider);
}

bool sLineCollider::operator!=(const sLineCollider& collider) const
{
	return !IsCollide(collider);
}
#pragma endregion