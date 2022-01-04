#include "Collision.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/ColliderCircle.h"

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

	CenterProjDist = abs(CenterDir.Dot(Axis));

	if (CenterProjDist > BoxInfo.Length.y)
		return false;

	return true;
}

bool CCollision::CollisionBox2DToBox2D(CollisionResult& SrcResult, CollisionResult& DestResult, const Box2DInfo& Src,
	const Box2DInfo& Dest)
{
	Vector2 CenterDir = Src.Center - Dest.Center;

	Vector2 Axis = Src.Axis[0];

	float CenterProjDist = abs(CenterDir.Dot(Axis));

	float r1, r2;

	r1 = Src.Length.x;
	r2 = abs(Dest.Axis[0].Dot(Axis) * Dest.Length.x) +
		abs(Dest.Axis[1].Dot(Axis) * Dest.Length.y);

	if (CenterProjDist > r1 + r2)
		return false;

	Axis = Src.Axis[1];
	CenterProjDist = abs(CenterDir.Dot(Axis));

	r1 = Src.Length.y;
	r2 = abs(Dest.Axis[0].Dot(Axis) * Dest.Length.x) +
		abs(Dest.Axis[1].Dot(Axis) * Dest.Length.y);

	if (CenterProjDist > r1 + r2)
		return false;

	Axis = Dest.Axis[0];
	CenterProjDist = abs(CenterDir.Dot(Axis));

	r1 = Dest.Length.x;
	r2 = abs(Src.Axis[0].Dot(Axis) * Src.Length.x) +
		abs(Src.Axis[1].Dot(Axis) * Src.Length.y);

	if (CenterProjDist > r1 + r2)
		return false;

	Axis = Dest.Axis[1];
	CenterProjDist = abs(CenterDir.Dot(Axis));
	r1 = Dest.Length.y;
	r2 = abs(Src.Axis[0].Dot(Axis) * Src.Length.x) +
		abs(Src.Axis[1].Dot(Axis) * Src.Length.y);
	if (CenterProjDist > r1 + r2)
		return false;

	return true;
}

bool CCollision::CollisionBox2DToBox2D(CColliderBox2D* Src, CColliderBox2D* Dest)
{
	CollisionResult srcResult, destResult;

	if (CollisionBox2DToBox2D(srcResult, destResult, Src->GetInfo(), Dest->GetInfo()))
	{
		srcResult.Src = Src;
		srcResult.Dest = Dest;

		destResult.Src = Dest;
		destResult.Dest = Src;

		Src->m_Result   = srcResult;
		Dest->m_Result = destResult;

		// 충돌 지점에 대한 정보는 저장하지 않을 것이다
		return true;
	}
	return false;
}

bool CCollision::CollisionBox2DToCircle(CColliderBox2D* Src, CColliderCircle* Circle)
{
	CollisionResult srcResult, destResult;

	if (CollisionBox2DToCircle(srcResult,destResult, Src->GetInfo(), Circle->GetInfo()))
	{
		srcResult.Src = Src;
		srcResult.Dest = Circle;

		destResult.Src = Circle;
		destResult.Dest = Src;

		Src->m_Result = srcResult;
		Circle->m_Result = destResult;
		return true;
	}
	return false;
}

bool CCollision::CollisionCircleToCircle(CColliderCircle* Src, CColliderCircle* Dest)
{
	CollisionResult srcResult, destResult;
	if (CollisionCircleToCircle(srcResult, destResult, Src->GetInfo(), Dest->GetInfo()))
	{
		srcResult.Src = Src;
		srcResult.Dest = Dest;

		destResult.Src = Dest;
		destResult.Dest = Src;

		Src->m_Result = srcResult;
		Dest->m_Result = destResult;

		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToPoint(CollisionResult& SrcResult, CollisionResult& DestResult,
	const CircleInfo& CircleInfo, const Vector2& Point)
{
	float Dist = CircleInfo.Center.Distance(Point);
	return Dist <= CircleInfo.Radius;
}

bool CCollision::CollisionBox2DToCircle(CollisionResult& SrcResult, CollisionResult& DestResult, const Box2DInfo& Src,
	const CircleInfo& Dest)
{
	Vector2 CenterDir = Src.Center - Dest.Center;
	Vector2 Axis = Src.Axis[0];

	float CenterProjDist = abs(CenterDir.Dot(Axis));

	float r1, r2;
	r1 = Src.Length.x;
	r2 = Dest.Radius;

	if (CenterProjDist > r1 + r2)
		return false;

	Axis = Src.Axis[1];

	CenterProjDist = abs(CenterDir.Dot(Axis));

	r1 = Src.Length.y;

	if (CenterProjDist > r1 + r2)
		return false;

	Axis = CenterDir;
	Axis.Normalize();

	r1 = abs(Src.Axis[0].Dot(Axis) * Src.Length.x) + abs(Src.Axis[1].Dot(Axis) * Src.Length.y);
	if (CenterProjDist > r1 + r2)
		return false;

	return true;
}

bool CCollision::CollisionCircleToCircle(CollisionResult& SrcResult, CollisionResult& DestResult,
	const CircleInfo& SrcInfo, const CircleInfo& Dest)
{
	float Dist = SrcInfo.Center.Distance(Dest.Center);
	return Dist <= (SrcInfo.Radius + Dest.Radius);
}
