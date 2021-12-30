#pragma once

#include "../GameInfo.h"

class CCollision
{
public :
	static bool CollisionBox2DToPoint(CollisionResult& SrcResult,
		CollisionResult& DestResult, const Box2DInfo& BoxInfo, const Vector2& Point);
	static bool CollisionBox2DToBox2D(CollisionResult& SrcResult,
		CollisionResult& DestResult, const Box2DInfo& Src, const Box2DInfo& Dest);
	static bool CollisionBox2DToBox2D(class CColliderBox2D* Src, class CColliderBox2D* Dest);
};

