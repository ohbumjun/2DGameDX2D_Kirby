#include "Collision.h"

bool CCollision::CollisionBox2DToPoint(CollisionResult& SrcResult, CollisionResult& DestResult,
	const Box2DInfo& BoxInfo, const Vector2& Point)
{
	// 상자의 x,y 축에 점을 투영하여, 구간이 겹치는지 판단
	// 둘다 겹쳐야만 충돌로 인식한다
	Vector2 CenterDir = BoxInfo.Center - Point;

	// x 축 
	Vector2 Axis = BoxInfo.Axis[0];

	// x축에 투영한 길이 --> 길이 이기 때문에 음수가 되면 안된다.
	float CenterProjDist = abs(CenterDir.Dot(Axis));

	if (CenterProjDist > BoxInfo.Length.x)
		return false;

	Axis = BoxInfo.Axis[1];
	CenterDir = BoxInfo.Center.y - Point.y;
	CenterProjDist = abs(CenterDir.Dot(Axis));
	if (CenterProjDist > BoxInfo.Length.y)
		return false;

	return true;
}
