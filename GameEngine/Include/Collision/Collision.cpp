#include "Collision.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/ColliderCircle.h"
#include "../Component/ColliderPixel.h"

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

		// �浹 ������ ���� ������ �������� ���� ���̴�
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
	// Box���� �浹 ó���� �������� �ʾҴٸ� skip �Ѵ�
	if (!CollisionBox2DToBox2D(SrcResult, DestResult, Src, Dest.Box))
		return false;

	// �������� ���Ѵ�
	float Left      = Src.Min.x > Dest.Box.Min.x ? Src.Min.x : Dest.Box.Min.x;
	float Right    = Src.Max.x < Dest.Box.Max.x ? Src.Max.x : Dest.Box.Max.x;
	float Bottom = Src.Min.y > Dest.Box.Min.y ? Src.Min.y : Dest.Box.Min.y;
	float Top       = Src.Max.y < Dest.Box.Max.y ? Src.Max.y : Dest.Box.Max.y;

	// ���� ���������� ��,�ϴ� ��ǥ�� ���Ѵ�
	Vector2 LB = Dest.Box.Center - Dest.Box.Length;

	Left -= LB.x;
	Right -= LB.x;
	Bottom -= LB.y;
	Top -= LB.y;

	Left = Left < 0.f ? 0.f : Left;
	Right = Right >= (float)Dest.Width ? (float)Dest.Width - 1.f : Right;
	Bottom = Bottom < 0.f ? 0.f : Bottom;
	Top = Top >= (float)Dest.Height ? (float)Dest.Height - 1.f : Top;

	// �̹����� ��ǥ�� �����ֱ� ���� Top, Bottom�� �ѹ� ������ �ش�
	Top = (float)Dest.Height - Top;
	Bottom = (float)Dest.Height - Bottom;

	bool Collision = false;

	// ������ ������ �ݺ��Ѵ� ( ���� ��� ����, ������ �ϴ� �������� )
	for (int row = (int)Top; row < (int)Bottom; row++)
	{
		for (int col = (int)Left; col < (int)Right; col++)
		{
			int Index = row * ((int)Dest.Width * 4) + (col * 4);

			// ���� �ε����� �ȼ���, ���� �ڽ� �ȿ� �����ϴ����� �Ǵ��Ѵ�
			// ���� �ȼ��� ���� ���������� ��ġ�� �����ش�
			// Vector ������ x�� column�� �ش��ϰ�, y�� row�� �ش��Ѵ�.
			// �̹����� ���ϴ� ���� ��ġ�� ���ؾ� �ϹǷ�, �ٽ� �ѹ� y�� �������ش� 
			Vector2 PixelWorldPos = LB + Vector2((float)col, (float)Dest.Height - (float)row);

			// �ڽ��� �ڽ����� �浹 ó�� 
			if (!CollisionBox2DToPoint(SrcResult, DestResult, Src, PixelWorldPos))
				continue;

			// ���� �Ǵ� 
			switch (Dest.Type)
			{
				// �ش� �����̸� ����
			case PixelCollision_Type::Color_Ignore :
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] && 
					Dest.Pixel[Index + 2] == Dest.Color[2])
				{
					continue;
				}
				Collision = true;
				break;
			case PixelCollision_Type::Color_Confirm :
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
				{
					Collision = true;
					break;
				}
			case PixelCollision_Type::Alpha_Ignore:
				if (Dest.Pixel[Index + 2] == Dest.Color[3])
				{
					continue;
				}
				Collision = true;
				break;
			case PixelCollision_Type::Alpha_Confirm:
				if (Dest.Pixel[Index + 2] == Dest.Color[3])
				{
					Collision = true;
					break;
				}
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
	// Box���� �浹 ó���� �������� �ʾҴٸ� skip �Ѵ�
	if (!CollisionBox2DToCircle(SrcResult, DestResult, Dest.Box , Src))
		return false;

	// �������� ���Ѵ�
	float Left = Src.Min.x > Dest.Box.Min.x ? Src.Min.x : Dest.Box.Min.x;
	float Right = Src.Max.x < Dest.Box.Max.x ? Src.Max.x : Dest.Box.Max.x;
	float Bottom = Src.Min.y > Dest.Box.Min.y ? Src.Min.y : Dest.Box.Min.y;
	float Top = Src.Max.y < Dest.Box.Max.y ? Src.Max.y : Dest.Box.Max.y;

	// ���� ���������� ��,�ϴ� ��ǥ�� ���Ѵ�
	Vector2 LB = Dest.Box.Center - Dest.Box.Length;

	Left -= LB.x;
	Right -= LB.x;
	Bottom -= LB.y;
	Top -= LB.y;

	Left = Left < 0.f ? 0.f : Left;
	Right = Right >= (float)Dest.Width ? (float)Dest.Width - 1.f : Right;
	Bottom = Bottom < 0.f ? 0.f : Bottom;
	Top = Top >= (float)Dest.Height ? (float)Dest.Height - 1.f : Top;

	// �̹����� ��ǥ�� �����ֱ� ���� Top, Bottom�� �ѹ� ������ �ش�
	Top = (float)Dest.Height - Top;
	Bottom = (float)Dest.Height - Bottom;

	bool Collision = false;

	// ������ ������ �ݺ��Ѵ� ( ���� ��� ����, ������ �ϴ� �������� )
	for (int row = (int)Top; row < (int)Bottom; row++)
	{
		for (int col = (int)Left; col < (int)Right; col++)
		{
			int Index = row * ((int)Dest.Width * 4) + (col * 4);

			// ���� �ε����� �ȼ���, ���� �ڽ� �ȿ� �����ϴ����� �Ǵ��Ѵ�
			// ���� �ȼ��� ���� ���������� ��ġ�� �����ش�
			// Vector ������ x�� column�� �ش��ϰ�, y�� row�� �ش��Ѵ�.
			// �̹����� ���ϴ� ���� ��ġ�� ���ؾ� �ϹǷ�, �ٽ� �ѹ� y�� �������ش� 
			Vector2 PixelWorldPos = LB + Vector2((float)col, (float)Dest.Height - (float)row);

			// �ڽ��� �ڽ����� �浹 ó�� 
			if (!CollisionCircleToPoint(SrcResult, DestResult, Src, PixelWorldPos))
				continue;

			// ���� �Ǵ� 
			switch (Dest.Type)
			{
				// �ش� �����̸� ����
			case PixelCollision_Type::Color_Ignore:
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
				{
					continue;
				}
				Collision = true;
				break;
			case PixelCollision_Type::Color_Confirm:
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
				{
					Collision = true;
					break;
				}
			case PixelCollision_Type::Alpha_Ignore:
				if (Dest.Pixel[Index + 2] == Dest.Color[3])
				{
					continue;
				}
				Collision = true;
				break;
			case PixelCollision_Type::Alpha_Confirm:
				if (Dest.Pixel[Index + 2] == Dest.Color[3])
				{
					Collision = true;
					break;
				}
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
	if (!CollisionBox2DToPoint(SrcResult, DestResult, Info.Box, Point))
		return false;

	Vector2	LB = Info.Box.Center - Info.Box.Length;

	Vector2	ConvertPoint = Point - LB;

	ConvertPoint.y = Info.Height - ConvertPoint.y;

	int	Index = (int)ConvertPoint.y * (int)Info.Width * 4 + (int)ConvertPoint.x * 4;

	bool	Result = false;

	switch (Info.Type)
	{
	case PixelCollision_Type::Color_Ignore:
		if (Info.Pixel[Index] == Info.Color[0] &&
			Info.Pixel[Index + 1] == Info.Color[1] &&
			Info.Pixel[Index + 2] == Info.Color[2])
			Result = false;

		else
			Result = true;
		break;
	case PixelCollision_Type::Color_Confirm:
		if (Info.Pixel[Index] == Info.Color[0] &&
			Info.Pixel[Index + 1] == Info.Color[1] &&
			Info.Pixel[Index + 2] == Info.Color[2])
			Result = true;

		else
			Result = false;
		break;
	case PixelCollision_Type::Alpha_Ignore:
		if (Info.Pixel[Index + 3] == Info.Color[3])
			Result = false;

		else
			Result = true;
		break;
	case PixelCollision_Type::Alpha_Confirm:
		if (Info.Pixel[Index + 3] == Info.Color[3])
			Result = true;

		else
			Result = false;
		break;
	}

	return Result;
}
