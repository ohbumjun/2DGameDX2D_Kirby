#include "Collision.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/ColliderCircle.h"
#include "../Component/ColliderPixel.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/CameraManager.h"
#include "../Scene/SceneCollision.h"

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

bool CCollision::CollisionCircleToPoint(const CircleInfo& CircleInfo, const Vector2& Point)
{
	CollisionResult srcResult, destResult;
	if (CollisionCircleToPoint(srcResult, destResult, CircleInfo, Point))
	{
		return true;
	}
	return false;
}

bool CCollision::CollisionBox2DToPixel(CColliderBox2D* Src, CColliderPixel* Dest)
{
	CollisionResult srcResult, destResult;

	if (CollisionBox2DToPixel(srcResult, destResult, Src->GetInfo(), Dest->GetInfo()))
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

bool CCollision::CollisionCircleToPixel(CColliderCircle* Src, CColliderPixel* Dest)
{
	CollisionResult srcResult, destResult;

	if (CollisionCircleToPixel(srcResult, destResult, Src->GetInfo(), Dest->GetInfo()))
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

	r1 = abs(Src.Axis[0].Dot(Axis) * Src.Length.x) + abs(Src.Axis[1].Dot(Axis) * Src.Length.y); //
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

bool CCollision::CollisionBox2DToPixel(CollisionResult& SrcResult, CollisionResult& DestResult, const Box2DInfo& Src,
	const PixelInfo& Dest)
{
	// Box와 Box 충돌을 먼저 판별한다.
	if (!CollisionBox2DToBox2D(SrcResult, DestResult, Src, Dest.Box))
		return false;

	// 겹치는 구간을 구한다
	Vector2 LB = Dest.Box.Center - Dest.Box.Length;

	// 큰것 먼저 구하기 
	float Left        = Src.Min.x > Dest.Box.Min.x ? Src.Min.x : Dest.Box.Min.x;
	float Right      = Src.Max.x < Dest.Box.Max.x ? Src.Max.x : Dest.Box.Max.x;
	float Bottom   = Src.Min.y > Dest.Box.Min.y ? Src.Min.y : Dest.Box.Min.y;
	float Top        = Src.Max.y < Dest.Box.Max.y ? Src.Max.y : Dest.Box.Max.y;

	Left -= LB.x;
	Right -= LB.x;
	Bottom -= LB.y;
	Top -= LB.y;

	Left = Left < 0.f ? 0.f : Left;
	Bottom = Bottom < 0.f ? 0.f : Bottom;

	Right = Right >= (float)Dest.Width ? (float)Dest.Width - 1.f : Right;
	Top = Top >= (float)Dest.Height ? (float)Dest.Height - 1.f : Top;

	// 이미지 상의 좌표로 변환하여 검사하기 위해 y를 뒤집는다
	Bottom = Dest.Height - Bottom;
	Top = Dest.Height - Top;

	int Index = -1;
	bool Collision = false;

	for (int row = (int)Top; row <= (int)Bottom; row++)
	{
		for (int col = (int)Left; col <= (int)Right; col++)
		{
			// 현재 위치와 Box가 충돌하는지 살핀다
			// 이번에는 World 좌표 기준으로 판단해야 하므로 y를 한번 더 뒤집고
			// Camera의 World 좌표를 더해준다.
			Vector2 PixelWorldPos = LB + Vector2((float)col, (float)Dest.Height - row);
			if (!CollisionBox2DToPoint(SrcResult, DestResult, Dest.Box, PixelWorldPos))
				continue;

			// 범위 안에 들어왔다면  Pixel 내의 색상을 비교할 것이다
			Index = (row * (Dest.Width * 4)) + (col * 4);

			switch (Dest.Type)
			{
			case PixelCollision_Type::Color_Confirm :
				{
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
				{
					Collision = true;
				}
				}
				break;
			case PixelCollision_Type::Color_Ignore:
			{
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
				{
					continue;
				}
				Collision = true;
			}
			break;
			case PixelCollision_Type::Alpha_Confirm:
			{
				if (Dest.Pixel[Index + 3] == Dest.Color[3])
				{
					Collision = true;
				}
			}
			break;
			case PixelCollision_Type::Alpha_Ignore:
			{
				if (Dest.Pixel[Index + 3] == Dest.Color[3])
				{
					continue;
				}
				Collision = true;
			}
			break;
			}
			if (Collision)
				break;
		}
		if (Collision)
			break;
	}

	return Collision;
}

bool CCollision::CollisionCircleToPixel(CollisionResult& SrcResult, CollisionResult& DestResult, const CircleInfo& Src,
	const PixelInfo& Dest)
{
	if (!CollisionBox2DToCircle(SrcResult, DestResult, Dest.Box, Src))
		return false;

	// 교집합을 구한다.
	float	Left = Src.Min.x < Dest.Min.x ? Dest.Min.x : Src.Min.x;
	float	Right = Src.Max.x > Dest.Max.x ? Dest.Max.x : Src.Max.x;

	float	Bottom = Src.Min.y < Dest.Min.y ? Dest.Min.y : Src.Min.y;
	float	Top = Src.Max.y > Dest.Max.y ? Dest.Max.y : Src.Max.y;

	// 월드 공간에서의 좌 하단 좌표를 구한다.
	Vector2	LB = Dest.Box.Center - Dest.Box.Length;

	Left -= LB.x;
	Right -= LB.x;

	Bottom -= LB.y;
	Top -= LB.y;

	Left = Left < 0.f ? 0.f : Left;
	Bottom = Bottom < 0.f ? 0.f : Bottom;

	Right = Right >= (float)Dest.Width ? (float)Dest.Width - 1.f : Right;
	Top = Top >= (float)Dest.Height ? (float)Dest.Height - 1.f : Top;

	Top = Dest.Height - Top;
	Bottom = Dest.Height - Bottom;

	bool	Collision = false;

	// 교집합 구간을 반복한다.
	for (int y = (int)Top; y < (int)Bottom; ++y)
	{
		for (int x = (int)Left; x < (int)Right; ++x)
		{
			int	Index = y * (int)Dest.Width * 4 + x * 4;

			// 현재 인덱스의 픽셀이 상대방 박스 안에 존재하는지를 판단한다.
			// 현재 픽셀의 월드공간에서의 위치를 구해준다.
			Vector2	PixelWorldPos = LB + Vector2((float)x, (float)Dest.Height - (float)y);
			if (!CollisionCircleToPoint(SrcResult, DestResult, Src, PixelWorldPos))
				continue;

			switch (Dest.Type)
			{
			case PixelCollision_Type::Color_Ignore:
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
					continue;

				Collision = true;
				break;
			case PixelCollision_Type::Color_Confirm:
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
					Collision = true;
				break;
			case PixelCollision_Type::Alpha_Ignore:
				if (Dest.Pixel[Index + 3] == Dest.Color[3])
					continue;

				Collision = true;
				break;
			case PixelCollision_Type::Alpha_Confirm:
				if (Dest.Pixel[Index + 3] == Dest.Color[3])
					Collision = true;
				break;
			}

			if (Collision)
				break;
		}

		if (Collision)
			break;
	}

	return Collision;
}

bool CCollision::CollisionPixelToPoint(CollisionResult& SrcResult, CollisionResult& DestResult, const PixelInfo& Info,
	const Vector2& Point)
{
	// Box와 Point 충돌 체크
	if (!CollisionBox2DToPoint(SrcResult, DestResult, Info.Box, Point))
		return false;

	Vector2 NewPointPos = Point - Info.Min;
	NewPointPos.y = Info.Height - NewPointPos.y;

	int Index = (int)(NewPointPos.x * Info.Width * 4) + (NewPointPos.y * 4);
	bool Collision = false;

	switch (Info.Type)
	{
	case PixelCollision_Type::Color_Ignore:
		{
			if (Info.Pixel[Index] == Info.Color[0] &&
				Info.Pixel[Index + 1] == Info.Color[1] &&
				Info.Pixel[Index + 2] == Info.Color[2])
				Collision = false;
			else
				Collision = true;
		}
		break;
	case PixelCollision_Type::Color_Confirm:
		{
			if (Info.Pixel[Index] == Info.Color[0] &&
				Info.Pixel[Index + 1] == Info.Color[1] &&
				Info.Pixel[Index + 2] == Info.Color[2])
				Collision = true;
			else
				Collision = false;
		}
		break;
	case PixelCollision_Type::Alpha_Ignore:
		{
			if (Info.Pixel[Index + 3] == Info.Color[3])
				Collision = false;
			else
				Collision = true;
		}
		break;
	case PixelCollision_Type::Alpha_Confirm:
		{
		if (Info.Pixel[Index + 3] == Info.Color[3])
			Collision = true;
		else
			Collision = false;
		}
	}
	return Collision;
}
