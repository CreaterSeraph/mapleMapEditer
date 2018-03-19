#pragma once

#define ACCURATE_COLLIDER

#ifdef ACCURATE_COLLIDER
typedef double collidersize;
#else
typedef float collidersize;
#endif

struct sRectCollider;
struct sCircleCollider;
struct sLineCollider;

struct sCircleCollider
{
	const collidersize r;
	const D3DXVECTOR2& pos;
	const D3DXVECTOR2 offset;
	const collidersize rot;
	collidersize scale;

	sCircleCollider(const D3DXVECTOR2& pos, const D3DXVECTOR2& offset, collidersize r, collidersize rot = 0.f);

	bool IsCollide(const sCircleCollider& collider) const;
	bool IsCollide(const sRectCollider& collider) const;
	bool IsCollide(const sLineCollider& collider) const;
	bool operator==(const sCircleCollider& collider) const;
	bool operator==(const sRectCollider& collider) const;
	bool operator==(const sLineCollider& collider) const;
	bool operator!=(const sCircleCollider& collider) const;
	bool operator!=(const sRectCollider& collider) const;
	bool operator!=(const sLineCollider& collider) const;
};

struct sRectCollider
{
	const D3DXVECTOR2& pos;
	const D3DXVECTOR2 offset;
	const D3DXVECTOR2 size;
	D3DXVECTOR2 scale;

	sRectCollider(const D3DXVECTOR2& pos, const D3DXVECTOR2& offset, const D3DXVECTOR2& size);

	bool IsCollide(const sCircleCollider& collider) const;
	bool IsCollide(const sRectCollider& collider) const;
	bool IsCollide(const sLineCollider& collider) const;
	bool operator==(const sCircleCollider& collider) const;
	bool operator==(const sRectCollider& collider) const;
	bool operator==(const sLineCollider& collider) const;
	bool operator!=(const sCircleCollider& collider) const;
	bool operator!=(const sRectCollider& collider) const;
	bool operator!=(const sLineCollider& collider) const;
};

struct sLineCollider
{
	const D3DXVECTOR2& pos1;
	const D3DXVECTOR2& pos2;
	collidersize weight;

	sLineCollider(const D3DXVECTOR2& pos1, const D3DXVECTOR2& pos2, collidersize weight = 0);

	bool IsCollide(const sCircleCollider& collider) const;
	bool IsCollide(const sRectCollider& collider) const;
	bool IsCollide(const sLineCollider& collider) const;
	bool operator==(const sCircleCollider& collider) const;
	bool operator==(const sRectCollider& collider) const;
	bool operator==(const sLineCollider& collider) const;
	bool operator!=(const sCircleCollider& collider) const;
	bool operator!=(const sRectCollider& collider) const;
	bool operator!=(const sLineCollider& collider) const;
};