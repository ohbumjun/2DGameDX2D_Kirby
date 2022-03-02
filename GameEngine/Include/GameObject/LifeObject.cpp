#include "LifeObject.h"

#include "../Component/TileEmptyComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"

CLifeObject::CLifeObject():
	m_MoveSpeed(200.f),
	m_PhysicsSimulate(false),
	// m_IsGround(true),
	// m_PhysicsSimulate(true),
	m_IsGround(false),
	m_FallTime(0.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(0.f),
	m_FallVelocity(0.f),
	m_FallVelocityMax(1000.f),
	m_GravityAccel(10.f),
	m_FloorCheck(false),
	m_SideWallCheck(false),	// Jump
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
		// m_FallVelocity = 0.5f * GRAVITY * m_FallTime * m_FallTime * m_JumpAccel;
		m_FallVelocity = 0.5f * GRAVITY * m_FallTime * m_FallTime * 0.1f;

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
		Vector3 RT = LB + Vector3(ResultRight, ResultTop,0.f);

		// LeftIdx = TileMap->GetTileEmptyIndexX(ResultLeft);
		LeftIdx = TileMap->GetTileEmptyIndexX(LB);
		// RightIdx = TileMap->GetTileEmptyIndexX(ResultRight);
		RightIdx = TileMap->GetTileEmptyIndexX(LB);

		// TopIdx = TileMap->GetTileEmptyIndexX(ResultTop);
		TopIdx = TileMap->GetTileEmptyIndexX(RT);
		// BottomIdx = TileMap->GetTileEmptyIndexX(ResultBottom);
		BottomIdx = TileMap->GetTileEmptyIndexX(RT);

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
					SetWorldPos(m_Pos.x, TilePos.y, m_Pos.z);

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
{}

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
