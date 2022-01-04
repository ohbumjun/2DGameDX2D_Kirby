#pragma once

#include "../GameInfo.h"

class CCollision
{
public : // Simple Versions
	static bool CollisionBox2DToBox2D(class CColliderBox2D* Src, class CColliderBox2D* Dest);
	static bool CollisionBox2DToCircle(class CColliderBox2D* Dest, class CColliderCircle* Circle);
	static bool CollisionCircleToCircle(class CColliderCircle* Src, class CColliderCircle* Dest);
	static bool CollisionCircleToPoint(const CircleInfo& CircleInfo, const Vector2& Point);
public : // To Point
	static bool CollisionBox2DToPoint(CollisionResult& SrcResult,
		CollisionResult& DestResult, const Box2DInfo& BoxInfo, const Vector2& Point);
	static bool CollisionCircleToPoint(CollisionResult& SrcResult, CollisionResult& DestResult,
		const CircleInfo& CircleInfo, const Vector2& Point);
	static bool CollisionBox2DToBox2D(CollisionResult& SrcResult,
		CollisionResult& DestResult, const Box2DInfo& Src, const Box2DInfo& Dest);
	static bool CollisionBox2DToCircle(CollisionResult& SrcResult, CollisionResult& DestResult,
		const Box2DInfo& Src, const CircleInfo& Dest);
	static bool CollisionCircleToCircle(CollisionResult& SrcResult, CollisionResult& DestResult,
		const CircleInfo& SrcInfo, const CircleInfo& Dest);
};

