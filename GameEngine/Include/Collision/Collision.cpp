#include "Collision.h"

bool CCollision::CollisionBox2DToPoint(CollisionResult& SrcResult, CollisionResult& DestResult,
	const Box2DInfo& BoxInfo, const Vector2& Point)
{
	// ������ x,y �࿡ ���� �����Ͽ�, ������ ��ġ���� �Ǵ�
	// �Ѵ� ���ľ߸� �浹�� �ν��Ѵ�
	Vector2 CenterDir = BoxInfo.Center - Point;

	// x �� 
	Vector2 Axis = BoxInfo.Axis[0];

	// x�࿡ ������ ���� --> ���� �̱� ������ ������ �Ǹ� �ȵȴ�.
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
