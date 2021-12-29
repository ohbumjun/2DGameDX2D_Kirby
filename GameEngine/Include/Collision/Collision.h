#pragma once

#include "../GameInfo.h"

class CCollision
{
public :
	static bool CollisionBox2DToPoint(CollisionResult& SrcResult,
		CollisionResult& DestResult, const Box2DInfo& BoxInfo, const Vector2& Point);
};

