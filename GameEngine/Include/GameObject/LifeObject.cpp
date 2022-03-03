#include "LifeObject.h"

#include "../Component/TileEmptyComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"

CLifeObject::CLifeObject():
	m_MoveSpeed(200.f),
	// m_IsGround(true),
	// m_PhysicsSimulate(true),
	m_PhysicsSimulate(true),
	m_IsGround(false),
	m_FallTime(0.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(0.f),
	m_FallVelocity(0.f),
	m_FallVelocityMax(1000.f),
	m_GravityAccel(10.f),
	m_FloorCheck(false),
	m_SideWallCheck(true),	
	m_JumpAccel(1.1f),
	m_JumpAccelAccTime(0.f),
	m_PrevPos{}
{}

CLifeObject::CLifeObject(const CLifeObject& obj) : CGameObject(obj)
{
	*this = obj;

	m_PrevPos.y = -1.f;
}

CLifeObject::~CLifeObject()
{}

void CLifeObject::UpdateWhileOffGround(float DeltaTime)
{
	if (!m_IsGround && m_PhysicsSimulate)
	{
		// �������� �ð� ���� 
		m_FallTime += DeltaTime * m_GravityAccel;

		float Velocity = 0.f;

		// ������ ���¶��, �ʱ� ���� �ӵ� * �ð� --> ���� �ö� �Ÿ��� �����ϴ� �� 
		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		// ������ �Ÿ�
		m_FallVelocity = 0.5f * GRAVITY * m_FallTime * m_FallTime * m_JumpAccel;
		// m_FallVelocity = 0.5f * GRAVITY * m_FallTime * m_FallTime * 0.01f;

		// �ִ� ���� �ӵ��� �����Ѵ�.
		if (Velocity - m_FallVelocity < m_FallVelocityMax * -1.f)
		{
			m_FallVelocity = Velocity + m_FallVelocityMax;
		}

		float CurrentYPos = m_FallStartY + (Velocity - m_FallVelocity);

		SetWorldPos(GetWorldPos().x, CurrentYPos, GetWorldPos().z);
	}
}

void CLifeObject::CheckBottomCollision()
{
	Vector3 m_Pos = GetWorldPos();

	if (m_PhysicsSimulate && m_Pos.y - m_PrevPos.y <= 0.f)
	{
		CTileEmptyComponent* TileMap =  m_Scene->GetTileEmptyComponent();

		Vector3 Pivot = GetPivot();
		Vector3 WorldScale = GetWorldScale();
		Vector3 TileSize = TileMap->GetTileEmptySize();

		float PrevBottom = m_PrevPos.y - Pivot.y * WorldScale.y;
		float CurBottom  = m_Pos.y - Pivot.y * WorldScale.y;

		float PrevLeft = m_PrevPos.x - Pivot.x * WorldScale.x;
		float CurLeft  = m_Pos.x - Pivot.x * WorldScale.x;

		float PrevRight  = PrevLeft + WorldScale.x;
		float CurRight   = CurLeft +  WorldScale.x;

		float ResultLeft   = PrevLeft < CurLeft ? PrevLeft : CurLeft;
		float ResultRight = PrevRight > CurRight ? PrevRight : CurRight;

		float ResultTop      = CurBottom > PrevBottom ? CurBottom : PrevBottom;
		float ResultBottom = CurBottom < PrevBottom ? CurBottom : PrevBottom;

		// �ش� ��ġ�� Tile�� ���ؿ´�.
		int LeftIdx = -1, TopIdx = -1, RightIdx = -1, BottomIdx = -1;

		Vector3 LB = Vector3(ResultLeft, ResultBottom,0.f);
		Vector3 RT = Vector3(ResultRight, ResultTop,0.f);

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

		// �Ʒ����� ���� �ݺ��Ѵ�
		for (int row = BottomIdx; row <= TopIdx; row++)
		{
			for (int col = LeftIdx; col <= RightIdx; col++)
			{
				// ���� ��ġ�� Bottom��, Ÿ���� Top ���� Ŭ ��� �����Ѵ�
				// ��, ���� �����ϴµ�, �� ���� Tile�� �����ϴ� ��
				// �ٴ� üũ�� ������ ���� üũ�ϱ� �����̴�.
				// if (TileMap->GetTileEmpty(col, row)->GetPos().y + TileSize.y > PrevBottom)
				//	continue;

				// ������ ���̶��. ��, �ٴڿ� ��� �ٸ�
				if (TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Wall)
				{
					// ���� ����
					Check = true;

					// ���� �������ν� �ʱ�ȭ ����� �� ������
					m_FallTime = 0.f;
					m_IsGround = true;
					m_Jump = false;
					// m_JumpAccelAccTime = 0.f;

					Vector3 CurrentPos = m_Pos;

					Vector3 TilePos = TileMap->GetTileEmpty(col, row)->GetWorldPos() +TileSize;

					// ��ġ ���� ����
					float NewPosY = TilePos.y + Pivot.y * WorldScale.y;
					SetWorldPos(m_Pos.x, NewPosY, m_Pos.z);

					break;
				}
			}

			if (Check)
				break;
		}

		// ���� �ٴڿ� ���� �ʾҴٸ�
		if (!Check)
		{
			// �׷��� m_IsGround�� true ���
			// ���� �� �ٽ� �������� �����ϴ� ���̴�
			if (m_IsGround)
			{
				m_FallTime = 0.f;
				m_FallStartY = GetWorldPos().y;
			}

			m_IsGround = false;
		}
	}
}

void CLifeObject::CheckCeilingCollision()
{}

void CLifeObject::CheckSideCollision()
{
	Vector3 m_Pos = GetWorldPos();

	float DirX = m_Pos.x - m_PrevPos.x;

	bool SideCollision = false;

	if (DirX != 0 && m_SideWallCheck)
	{
		// ���� ��ġ, ���� ��ġ ������ �̵� ���� ���� ��� Tile ���� ����� ���̴�

		CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

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
		if (DirX >  0.f)
		{
			int Index = -1;
			Vector3 TilePos = {};

			ResultLB.x = PrevLB.x;
			ResultLB.y = CurLB.y < PrevLB.y ? CurLB.y : PrevLB.y;

			ResultRT.x = CurRT.x;
			ResultRT.y = CurRT.y > PrevRT.y ? CurRT.y : PrevRT.y;

			LBIndexX = TileMap->GetTileEmptyIndexX(Vector3(ResultLB.x, ResultLB.y, m_Pos.z));
			LBIndexY = TileMap->GetTileEmptyIndexY(Vector3(ResultLB.x, ResultLB.y, m_Pos.z));

			RTIndexX = TileMap->GetTileEmptyIndexX(Vector3(ResultRT.x, ResultRT.y, m_Pos.z));
			RTIndexY = TileMap->GetTileEmptyIndexY(Vector3(ResultRT.x, ResultRT.y, m_Pos.z));

			// ���� ����ϱ�
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
					/*
					if (TileMap->GetTile(Index)->GetTileOption() !=
						ETileOption::Wall ||
						!TileMap->GetTile(Index)->GetSideCollision())
						continue;
					 */
					if (TileMap->GetTileEmpty(Index)->GetTileType() != Tile_Type::Wall)
						continue;

					TilePos = TileMap->GetTileEmpty(Index)->GetWorldPos();

					// Object �� Tile ���� ���� �پ����� ��쿡��,
					// �浹 ó���� ���� ���� ���̴�
					if (TilePos.y - 0.001f <= CurLB.y && CurLB.y <= TilePos.y + 0.001f)
						continue;

					// ���� �ڵ� �󿡼���
					// Side Collision ���� , Bottom Collision
					// ���� �ʹ� ���� �İ��ٸ�, Side Collision �����ϰ�, �߷� ���뿡 ����
					// ���� �о�� ���� ������ �����̴�
					if (CurRT.x - TilePos.x > 8.f)
						continue;

					// ��Ʈ �浹
					if (CurLB.x <= TilePos.x + TileSize.x  &&
						CurRT.x >= TilePos.x &&
						CurLB.y <= TilePos.y + TileSize.y &&
						CurRT.y >= TilePos.y 
						)
					{
						SideCollision = true;

						// ���� ���������� �̵� �� --> �������� �о ���̴�
						float MoveX = TilePos.x - CurRT.x - 0.01f;

						m_Pos.x += MoveX;

						SetWorldPos(m_Pos);

						// break ���� �ʿ��Ѱ� ?
						break;
					}
				}
				if (SideCollision)
					break;
			}
		}
		// ���� �������� �̵� ���̶�� 
		else
		{
			int Index = -1;
			Vector3 TilePos = {};

			ResultLB.x = CurLB.x;
			ResultLB.y = CurLB.y < PrevLB.y ? CurLB.y : PrevLB.y;

			ResultRT.x = PrevRT.x;
			ResultRT.y = CurRT.y > PrevRT.y ? CurRT.y : PrevRT.y;

			LBIndexX = TileMap->GetTileEmptyIndexX(Vector3(ResultLB.x, ResultLB.y, m_Pos.z));
			LBIndexY = TileMap->GetTileEmptyIndexY(Vector3(ResultLB.x, ResultLB.y, m_Pos.z));

			RTIndexX = TileMap->GetTileEmptyIndexX(Vector3(ResultRT.x, ResultRT.y, m_Pos.z));
			RTIndexY = TileMap->GetTileEmptyIndexY(Vector3(ResultRT.x, ResultRT.y, m_Pos.z));

			// ���� ����ϱ�
			LBIndexX = LBIndexX < 0 ? 0 : LBIndexX;
			LBIndexY = LBIndexY < 0 ? 0 : LBIndexY;

			RTIndexX = RTIndexX >= TileMap->GetTileCountX() ? TileMap->GetTileCountX() - 1 : RTIndexX;
			RTIndexY = RTIndexY >= TileMap->GetTileCountY() ? TileMap->GetTileCountY() - 1 : RTIndexY;

			for (int row = LBIndexY; row <= RTIndexY; row++)
			{
				for (int col = LBIndexX; col <= RTIndexX; col++)
				{
					Index = row * TileMap->GetTileCountX() + col;

					// Wall �� �ƴ϶�� Pass
					if (TileMap->GetTileEmpty(Index)->GetTileType() != Tile_Type::Wall)
						continue;

					TilePos = TileMap->GetTileEmpty(Index)->GetWorldPos();

					// ����������, �ٴڿ� �پ��ִٸ� üũ���� �ʴ´�.
					if (TilePos.y - 0.001f <= CurLB.y && CurLB.y <= TilePos.y + 0.001f)
						continue;

					// �ʹ� ���� �İ��� Side Collision �� �����ϰ�, �߷� ���뿡 ����
					// ���� �о�� ���� ������ �����̴�.
					if (TilePos.x + TileSize.x - CurLB.x > 8.f)
						continue;

					if (CurLB.x <= TilePos.x + TileSize.x && 
						CurRT.x >= TilePos.x &&
						CurLB.y <= TilePos.y + TileSize.y &&
						CurRT.y >= TilePos.y)
					{
						SideCollision = true;

						float MoveX = TilePos.x + TileSize.x - CurLB.x + 0.001f;

						m_Pos.x += MoveX;

						SetWorldPos(m_Pos);

						break;
					}
				}
				if (SideCollision)
					break;
			}
		}
	}
}

void CLifeObject::Start()
{
	CGameObject::Start();

	m_FallStartY = GetWorldPos().y;
}

bool CLifeObject::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CLifeObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// 1) �߷� ���� ������ Side Collision ���� �����ؾ� �Ѵ�.
	CheckSideCollision();

	UpdateWhileOffGround(DeltaTime);

	CheckBottomCollision();
}

void CLifeObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	m_PrevPos = GetWorldPos();
}

void CLifeObject::PrevRender()
{
	CGameObject::PrevRender();
}

void CLifeObject::Render()
{
	CGameObject::Render();
}

void CLifeObject::PostRender()
{
	CGameObject::PostRender();

}
