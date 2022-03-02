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
		// 떨어지는 시간 누적 
		m_FallTime += DeltaTime * m_GravityAccel;

		float Velocity = 0.f;

		// 점프한 상태라면, 초기 점프 속도 * 시간 --> 위로 올라간 거리를 세팅하는 것 
		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		// 내려간 거리
		// m_FallVelocity = 0.5f * GRAVITY * m_FallTime * m_FallTime * m_JumpAccel;
		m_FallVelocity = 0.5f * GRAVITY * m_FallTime * m_FallTime * 0.1f;

		// 최대 낙하 속도를 조절한다.
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

		// 해당 위치의 Tile을 구해온다.
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

		// 아래에서 위로 반복한다
		for (int row = BottomIdx; row <= TopIdx; row++)
		{
			for (int col = LeftIdx; col <= RightIdx; col++)
			{
				// 이전 위치의 Bottom이, 타일의 Top 보다 클 경우 무시한다
				// 즉, 내가 점프하는데, 그 위에 Tile이 존재하는 것
				// 바닥 체크는 내려갈 때만 체크하기 위함이다.
				// if (TileMap->GetTileEmpty(col, row)->GetPos().y + TileSize.y > PrevBottom)
				//	continue;

				// 못가는 곳이라면. 즉, 바닥에 닿는 다면
				if (TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Wall)
				{
					// 땅에 닿음
					Check = true;

					// 땅에 닿음으로써 초기화 해줘야 할 변수들
					m_FallTime = 0.f;
					m_IsGround = true;
					m_Jump = false;
					// m_JumpAccelAccTime = 0.f;

					Vector3 CurrentPos = m_Pos;

					Vector3 TilePos = TileMap->GetTileEmpty(col, row)->GetWorldPos() +TileSize;

					// 위치 정보 세팅
					SetWorldPos(m_Pos.x, TilePos.y, m_Pos.z);

					break;
				}
			}

			if (Check)
				break;
		}

		// 만약 바닥에 닿지 않았다면
		if (!Check)
		{
			// 그런데 m_IsGround는 true 라면
			// 이제 막 다시 떨어지기 시작하는 것이다
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
