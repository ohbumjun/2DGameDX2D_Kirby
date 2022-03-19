#include "AttackEffect.h"
#include "Component/TileEmptyComponent.h"
#include "Scene/Scene.h"

CAttackEffect::CAttackEffect()  :
	m_SideCollisionApplied(true),
	m_BottomCollisionApplied(false)
{}

CAttackEffect::CAttackEffect(const CAttackEffect& obj)
{}

CAttackEffect::~CAttackEffect()
{}

void CAttackEffect::BottomCollisionSpecificAction()
{
	Destroy();
}

bool CAttackEffect::CheckSideCollision()
{
	if (!m_SideCollisionApplied)
		return false;

	CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

	if (!TileMap)
		return false;

	Vector3 m_Pos = GetWorldPos();

	// �� �ش� Object�� ������� ���� 
	if (m_PrevPos.x == 0.f && m_PrevPos.y == 0.f)
	{
		m_PrevPos = m_Pos;
	}

	float DirX = m_Pos.x - m_PrevPos.x;

	if (DirX != 0)
	{
		// ���� ��ġ, ���� ��ġ ������ �̵� ���� ���� ��� Tile ���� ������ ���̴�

		Vector3 WorldScale = GetWorldScale();
		Vector3 Pivot = GetPivot();
		Vector3 TileSize = TileMap->GetTileEmptySize();

		Vector3 CurLB = m_Pos - Pivot * WorldScale;
		Vector3 CurRT = CurLB + WorldScale;

		Vector3 PrevLB = m_PrevPos - Pivot * WorldScale;
		Vector3 PrevRT = PrevLB + WorldScale;

		int LBIndexX = -1, LBIndexY = -1, RTIndexX = -1, RTIndexY = -1;

		Vector3 ResultLB, ResultRT;

		// ���������� �̵��� �� --> ����Ÿ�Ͽ��� ������ Ÿ�� ������� �����ϱ�
		if (DirX > 0.f)
		{
			int Index = -1;

			ResultLB.x = PrevRT.x;
			ResultLB.y = CurLB.y < PrevLB.y ? CurLB.y : PrevLB.y;

			ResultRT.x = CurRT.x;
			ResultRT.y = CurRT.y > PrevRT.y ? CurRT.y : PrevRT.y;

			LBIndexX = TileMap->GetTileEmptyIndexX(Vector3(ResultLB.x, ResultLB.y, m_Pos.z));
			LBIndexY = TileMap->GetTileEmptyIndexY(Vector3(ResultLB.x, ResultLB.y, m_Pos.z));

			RTIndexX = TileMap->GetTileEmptyIndexX(Vector3(ResultRT.x, ResultRT.y, m_Pos.z));
			RTIndexY = TileMap->GetTileEmptyIndexY(Vector3(ResultRT.x, ResultRT.y, m_Pos.z));

			// ���� �����ϱ�
			LBIndexX = LBIndexX < 0 ? 0 : LBIndexX;
			LBIndexY = LBIndexY < 0 ? 0 : LBIndexY;

			RTIndexX = RTIndexX >= TileMap->GetTileCountX() ? TileMap->GetTileCountX() - 1 : RTIndexX;
			RTIndexY = RTIndexY >= TileMap->GetTileCountY() ? TileMap->GetTileCountY() - 1 : RTIndexY;

			for (int row = LBIndexY; row <= RTIndexY; row++)
			{
				for (int col = LBIndexX; col <= RTIndexX; col++)
				{
					Index = row * TileMap->GetTileCountX() + col;

					// ���� ó��
					if (TileMap->GetTileEmpty(Index)->GetTileType() != Tile_Type::Wall)
						continue;

					Vector3 TilePos = TileMap->GetTileEmpty(Index)->GetWorldPos();

					// ��Ʈ �浹
					if (CurLB.x <= TilePos.x + TileSize.x &&
						CurRT.x >= TilePos.x &&
						CurLB.y <= TilePos.y + TileSize.y &&
						CurRT.y >= TilePos.y
						)
					{
						if (TilePos.y + TileSize.y - 150.f <= ResultLB.y &&
							ResultLB.y <= TilePos.y + TileSize.y + 150.f)
							continue;
						
						return true;
					}
				}
			}
		}
		// ���� �������� �̵� ���̶�� 
		else
		{
			int Index = -1;
			Vector3 TilePos = {};

			ResultLB.x = CurLB.x;
			ResultLB.y = CurLB.y < PrevLB.y ? CurLB.y : PrevLB.y;

			ResultRT.x = PrevLB.x;
			ResultRT.y = CurRT.y > PrevRT.y ? CurRT.y : PrevRT.y;

			LBIndexX = TileMap->GetTileEmptyIndexX(Vector3(ResultLB.x, ResultLB.y, m_Pos.z));
			LBIndexY = TileMap->GetTileEmptyIndexY(Vector3(ResultLB.x, ResultLB.y, m_Pos.z));

			RTIndexX = TileMap->GetTileEmptyIndexX(Vector3(ResultRT.x, ResultRT.y, m_Pos.z));
			RTIndexY = TileMap->GetTileEmptyIndexY(Vector3(ResultRT.x, ResultRT.y, m_Pos.z));

			// ���� �����ϱ�
			LBIndexX = LBIndexX < 0 ? 0 : LBIndexX;
			LBIndexY = LBIndexY < 0 ? 0 : LBIndexY;

			RTIndexX = RTIndexX >= TileMap->GetTileCountX() ? TileMap->GetTileCountX() - 1 : RTIndexX;
			RTIndexY = RTIndexY >= TileMap->GetTileCountY() ? TileMap->GetTileCountY() - 1 : RTIndexY;

			// �����ʿ��� ���� ������ Ÿ���� �����Ѵ�.
			for (int row = LBIndexY; row <= RTIndexY; row++)
			{
				for (int col = RTIndexX; col >= LBIndexX; col--)
				{
					Index = row * TileMap->GetTileCountX() + col;

					// Wall �� �ƴ϶�� Pass
					if (TileMap->GetTileEmpty(Index)->GetTileType() != Tile_Type::Wall)
						continue;

					TilePos = TileMap->GetTileEmpty(Index)->GetWorldPos();


					if (CurLB.x <= TilePos.x + TileSize.x &&
						CurRT.x >= TilePos.x &&
						CurLB.y <= TilePos.y + TileSize.y &&
						CurRT.y >= TilePos.y)
					{
						if (TilePos.y + TileSize.y - 150.f <= ResultLB.y &&
							ResultLB.y <= TilePos.y + TileSize.y + 150.f)
							continue;

						return true;
					}
				}
			}
		}
	}
	return false;
}

bool CAttackEffect::CheckBottomCollision()
{
	Vector3 m_Pos = GetWorldPos();

	CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

	if (!TileMap)
		return false;

	if (!m_BottomCollisionApplied)
		return false;

	if (m_Pos.y - m_PrevPos.y < 0.f)
	{
		CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

		Vector3 Pivot = GetPivot();
		Vector3 WorldScale = GetWorldScale();
		Vector3 TileSize = TileMap->GetTileEmptySize();

		float PrevBottom = m_PrevPos.y - Pivot.y * WorldScale.y;
		float CurBottom = m_Pos.y - Pivot.y * WorldScale.y;

		float PrevLeft = m_PrevPos.x - Pivot.x * WorldScale.x;
		float CurLeft = m_Pos.x - Pivot.x * WorldScale.x;

		float PrevRight = PrevLeft + WorldScale.x;
		float CurRight = CurLeft + WorldScale.x;

		float ResultLeft = PrevLeft < CurLeft ? PrevLeft : CurLeft;
		float ResultRight = PrevRight > CurRight ? PrevRight : CurRight;

		float ResultTop = CurBottom > PrevBottom ? CurBottom : PrevBottom;
		float ResultBottom = CurBottom < PrevBottom ? CurBottom : PrevBottom;

		// �ش� ��ġ�� Tile�� ���ؿ´�.
		int LeftIdx = -1, TopIdx = -1, RightIdx = -1, BottomIdx = -1;

		Vector3 LB = Vector3(ResultLeft, ResultBottom, 0.f);
		Vector3 RT = Vector3(ResultRight, ResultTop, 0.f);

		// LeftIdx = TileMap->GetTileEmptyIndexX(ResultLeft);
		LeftIdx = TileMap->GetTileEmptyIndexX(LB);
		// RightIdx = TileMap->GetTileEmptyIndexX(ResultRight);
		RightIdx = TileMap->GetTileEmptyIndexX(RT);

		// TopIdx = TileMap->GetTileEmptyIndexX(ResultTop);
		TopIdx = TileMap->GetTileEmptyIndexY(RT);

		// BottomIdx = TileMap->GetTileEmptyIndexX(ResultBottom);
		BottomIdx = TileMap->GetTileEmptyIndexY(LB);

		if (LeftIdx < 0)
			LeftIdx = 0;

		if (BottomIdx < 0)
			BottomIdx = 0;

		if (RightIdx >= TileMap->GetTileCountX())
			RightIdx = TileMap->GetTileCountX() - 1;

		if (TopIdx >= TileMap->GetTileCountY())
			TopIdx = TileMap->GetTileCountY() - 1;

		bool Check = false;

		// ������ �Ʒ��� �ݺ��Ѵ�
		for (int row = TopIdx; row >= BottomIdx; row--)
		{
			for (int col = LeftIdx; col <= RightIdx; col++)
			{
				// ���� ��ġ�� Bottom��, Ÿ���� Top ���� Ŭ ��� �����Ѵ�
				// ��, ���� �����ϴµ�, �� ���� Tile�� �����ϴ� ��
				// �ٴ� üũ�� ������ ���� üũ�ϱ� �����̴�.
				if (TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Block &&
					TileMap->GetTileEmpty(col, row)->GetPos().y + TileSize.y > PrevBottom)
					continue;

				// ������ ���̶��. ��, �ٴڿ� ��� �ٸ�
				if (TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Wall ||
					TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Block)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void CAttackEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (CheckSideCollision())
	{
		Destroy();
	}

	if (CheckBottomCollision())
	{
		BottomCollisionSpecificAction();
	}

	m_PrevPos = GetWorldPos();

}

void CAttackEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}