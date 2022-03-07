#include "LifeObject.h"

#include "../Component/TileEmptyComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"

CLifeObject::CLifeObject():
	m_MoveSpeed(200.f),
	// m_IsGround(true),
	// m_PhysicsSimulate(true),
	m_PhysicsSimulate(true),
	m_CollisionDisabled(false),
	m_IsGround(false),
	m_FallTime(0.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(90.f),
	m_FallVelocity(0.f),
	m_FallVelocityMax(1000.f),
	m_GravityAccel(10.f),
	m_FloorCheck(false),
	m_SideWallCheck(true),	
	m_IsSideCollided(false),
	m_JumpAccel(2.f),
	m_JumpAccelAccTime(0.f),
	m_CollisionOffset(0.001f),
	m_GroundOffSet(1.f),
	m_ApplyBottomCollision(true),
	m_PrevPos{},
	m_PhysicApplyDelayTime(1.5f),
	m_IsGroundObject(false)

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
	// Edit Mode 라면 떨어지는 효과를 적용하지 않는다
	if (m_Scene->IsEditMode())
		return;

	if (m_CollisionDisabled)
		return;

	// 중력 적용 효과를 얼마 시간 이후에 적용한다. 
	if (m_PhysicApplyDelayTime >= 0.f)
	{
		m_PhysicApplyDelayTime -= DeltaTime;

		return;
	}

	if ((!m_IsGround && m_PhysicsSimulate) || (!m_IsBottomCollided && m_PhysicsSimulate && m_IsGroundObject))
	{
		// 떨어지는 시간 누적 
		m_FallTime += DeltaTime * m_GravityAccel;

		float Velocity = 0.f;

		// 점프한 상태라면, 초기 점프 속도 * 시간 --> 위로 올라간 거리를 세팅하는 것 
		if (m_Jump)
		{
			Velocity = m_JumpVelocity * m_FallTime;

			// 만약 점프 중에 화면 밖으로 벗어난다면 Velocity는 별도로 처리해야 한다.
			float ObjectHeadPosY = GetWorldPos().y + GetWorldScale().y;

			float WorldResolutionY = m_Scene->GetWorldResolution().y;

			if (WorldResolutionY - ObjectHeadPosY < 1.f)
			{
				// Velocity = 0.f;
			}
		}

		// 내려간 거리
		m_FallVelocity = 0.5f * GRAVITY * m_FallTime * m_FallTime * m_JumpAccel;

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
	if (m_CollisionDisabled)
		return;

	bool BottomCollision = false;

	Vector3 m_Pos = GetWorldPos();

	CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

	if (!TileMap)
	{
		m_IsBottomCollided = false;
		return;
	}

	if ((m_PhysicsSimulate && m_Pos.y - m_PrevPos.y <= 0.f) || 
		(m_ApplyBottomCollision && m_Pos.y - m_PrevPos.y <= 0.f))
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

		// 위에서 아래로 반복한다
		for (int row = TopIdx; row >= BottomIdx; row--)
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
					SetObjectLand();

					// m_JumpAccelAccTime = 0.f;

					Vector3 CurrentPos = m_Pos;

					Vector3 TilePos = TileMap->GetTileEmpty(col, row)->GetWorldPos() +TileSize;

					// 위치 정보 세팅
					float NewPosY = TilePos.y + Pivot.y * WorldScale.y;

					SetWorldPos(m_Pos.x, NewPosY, m_Pos.z);

					BottomCollision = true;

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

	m_IsBottomCollided = BottomCollision;
}

void CLifeObject::CheckCeilingCollision()
{}

void CLifeObject::CheckSideCollision()
{
	if (m_CollisionDisabled)
		return;

	CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

	if (!TileMap)
		return;

	Vector3 m_Pos = GetWorldPos();

	float DirX = m_Pos.x - m_PrevPos.x;

	bool SideCollision = false;

	if (DirX != 0 && m_SideWallCheck)
	{
		// 이전 위치, 현재 위치 까지의 이동 범위 안의 모든 Tile 들을 고려할 것이다

		Vector3 WorldScale = GetWorldScale();
		Vector3 Pivot = GetPivot();
		Vector3 TileSize = TileMap->GetTileEmptySize();

		Vector3 CurLB = m_Pos - Pivot * WorldScale;
		Vector3 CurRT = CurLB + WorldScale;

		Vector3 PrevLB = m_PrevPos - Pivot * WorldScale;
		Vector3 PrevRT = PrevLB + WorldScale;

		int LBIndexX = -1, LBIndexY = -1, RTIndexX = -1, RTIndexY = -1;

		Vector3 ResultLB, ResultRT;

		// 오른쪽으로 이동할 때 --> 왼쪽타일에서 오른쪽 타일 순서대로 조사하기
		if (DirX >  0.f)
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

			// 범위 고려하기
			LBIndexX = LBIndexX < 0 ? 0 : LBIndexX;
			LBIndexY = LBIndexY < 0 ? 0 : LBIndexY;

			RTIndexX = RTIndexX >= TileMap->GetTileCountX() ? TileMap->GetTileCountX() - 1 : RTIndexX;
			RTIndexY = RTIndexY >= TileMap->GetTileCountY() ? TileMap->GetTileCountY() - 1 : RTIndexY;

			for (int row = LBIndexY; row <= RTIndexY; row++)
			{
				for (int col = LBIndexX; col <= RTIndexX; col++)
				{
					Index = row * TileMap->GetTileCountX() + col;

					// 예외 처리
					/*
					if (TileMap->GetTile(Index)->GetTileOption() !=
						ETileOption::Wall ||
						!TileMap->GetTile(Index)->GetSideCollision())
						continue;
					 */
					if (TileMap->GetTileEmpty(Index)->GetTileType() != Tile_Type::Wall)
						continue;

					Vector3 TilePos = TileMap->GetTileEmpty(Index)->GetWorldPos();

					// Object 가 Tile 위에 거의 붙어있을 경우에는,
					// 충돌 처리를 하지 않을 것이다
					if (TilePos.y + TileSize.y - m_GroundOffSet <= ResultLB.y && 
						ResultLB.y <= TilePos.y + TileSize.y + m_GroundOffSet)
						continue;

					// 현재 코드 상에서는
					// Side Collision 먼저 , Bottom Collision
					// 만약 너무 깊이 파고든다면, Side Collision 무시하고, 중력 적용에 따른
					// 위로 밀어내기 만을 적용할 예정이다
					if (CurRT.x - TilePos.x > 10.f)
						continue;

					// 렉트 충돌
					if (CurLB.x <= TilePos.x + TileSize.x  &&
						CurRT.x >= TilePos.x &&
						CurLB.y <= TilePos.y + TileSize.y &&
						CurRT.y >= TilePos.y 
						)
					{
						SideCollision = true;

						// 현재 오른쪽으로 이동 중 --> 왼쪽으로 밀어낼 것이다
						float MoveX = TilePos.x - CurRT.x - m_CollisionOffset;

						m_Pos.x += MoveX;

						SetWorldPos(m_Pos);

						// break 문이 필요한가 ?
						break;
					}
				}
				if (SideCollision)
					break;
			}
		}
		// 만약 왼쪽으로 이동 중이라면 
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

			// 범위 고려하기
			LBIndexX = LBIndexX < 0 ? 0 : LBIndexX;
			LBIndexY = LBIndexY < 0 ? 0 : LBIndexY;

			RTIndexX = RTIndexX >= TileMap->GetTileCountX() ? TileMap->GetTileCountX() - 1 : RTIndexX;
			RTIndexY = RTIndexY >= TileMap->GetTileCountY() ? TileMap->GetTileCountY() - 1 : RTIndexY;

			// 오른쪽에서 왼쪽 순으로 타일을 조사한다.
			for (int row = LBIndexY; row <= RTIndexY; row++)
			{
				for (int col = RTIndexX; col >= LBIndexX; col--)
				{
					Index = row * TileMap->GetTileCountX() + col;

					// Wall 이 아니라면 Pass
					if (TileMap->GetTileEmpty(Index)->GetTileType() != Tile_Type::Wall)
						continue;

					TilePos = TileMap->GetTileEmpty(Index)->GetWorldPos();

					// 마찬가지로, 바닥에 붙어있다면 체크하지 않는다.
					if (TilePos.y + TileSize.y - m_GroundOffSet <= ResultLB.y &&
						ResultLB.y <= TilePos.y + TileSize.y + m_GroundOffSet)
						continue;

					// 너무 깊이 파고들면 Side Collision 은 무시하고, 중력 적용에 따른
					// 위로 밀어내기 만을 적용할 예정이다.
					if (TilePos.x + TileSize.x - CurLB.x > 10.f)
						continue;

					if (CurLB.x <= TilePos.x + TileSize.x && 
						CurRT.x >= TilePos.x &&
						CurLB.y <= TilePos.y + TileSize.y &&
						CurRT.y >= TilePos.y)
					{
						SideCollision = true;

						float MoveX = TilePos.x + TileSize.x - CurLB.x + m_CollisionOffset;

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

	m_IsSideCollided = SideCollision;
}

void CLifeObject::CheckOutsideWorldResolution()
{
	// todo : Up
	Vector3 OriginalPos = GetWorldPos();
	Vector3 WorldScale = GetWorldScale();
	Vector3 Pivot = GetPivot();

	Vector2 WorldResolution = m_Scene->GetWorldResolution();

	if (OriginalPos.y + WorldScale.y * Pivot.y >= WorldResolution.y)
		OriginalPos.y = WorldResolution.y - WorldScale.y * Pivot.y - m_CollisionOffset;

	// todo : Down ( 나중에 ) --> 바꾸기 
	if (OriginalPos.y - WorldScale.y * Pivot.y < 0.f)
		OriginalPos.y = WorldScale.y * Pivot.y;

	// Left
	if (OriginalPos.x - WorldScale.x * Pivot.x < 0.f)
		OriginalPos.x = WorldScale.x * Pivot.x + m_CollisionOffset;

	// Right
	if (OriginalPos.x + WorldScale.x * Pivot.x >= WorldResolution.x)
		OriginalPos.x = WorldResolution.x - WorldScale.x * Pivot.x - m_CollisionOffset;

	SetWorldPos(OriginalPos);
}

void CLifeObject::SetObjectLand()
{
	m_FallTime = 0.f;
	m_IsGround = true;
	m_Jump = false;
}

void CLifeObject::Start()
{
	CGameObject::Start();

	m_FallStartY = GetWorldPos().y;

	m_PrevPos.y = GetWorldPos().y;

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

	CheckOutsideWorldResolution();
}

void CLifeObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	// 1) 중력 적용 이전에 Side Collision 먼저 적용해야 한다.
	CheckSideCollision();

	CheckBottomCollision();



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
