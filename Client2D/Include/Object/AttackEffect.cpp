#include "AttackEffect.h"
#include "Player2D.h"
#include "GameObject/LifeObject.h"
#include "Component/TileEmptyComponent.h"
#include "Component/SpriteComponent.h"
#include "Scene/Scene.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CAttackEffect::CAttackEffect()  :
	m_SideCollisionApplied(true),
	m_BottomCollisionApplied(false),
	m_NormalBottomCheckApplied(false),
	m_Jump(false),
	m_JumpVelocity(20.f),
	m_EffectMoveSpeed(500.f),
	m_PhysicsSimulate(false),
	m_AttackDir(1.f, 0.f)
{
}

CAttackEffect::CAttackEffect(const CAttackEffect& obj)
{}

CAttackEffect::~CAttackEffect()
{}

void CAttackEffect::BottomCollisionSpecificAction()
{
	Destroy();
}

void CAttackEffect::SideCollisionSpecificAction()
{
	Destroy();
}

void CAttackEffect::SetRightAttackDir(float YDir)
{
	m_AttackDir.x = 1.f;
	m_AttackDir.y = YDir;
}

void CAttackEffect::SetLeftAttackDir(float YDir)
{
	m_AttackDir.x = -1.f;
	m_AttackDir.y = YDir;
}

void CAttackEffect::SetAttackDirX(float XDir)
{
	m_AttackDir.x = XDir;
}

bool CAttackEffect::CheckSideCollision()
{
	if (!m_SideCollisionApplied)
		return false;

	CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

	if (!TileMap)
		return false;

	Vector3 m_Pos = GetWorldPos();

	// 막 해당 Object가 만들어진 상태 
	if (m_PrevPos.x == 0.f && m_PrevPos.y == 0.f)
	{
		m_PrevPos = m_Pos;
	}

	float DirX = m_Pos.x - m_PrevPos.x;

	if (DirX != 0)
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
					if (TileMap->GetTileEmpty(Index)->GetTileType() != Tile_Type::Wall)
						continue;

					Vector3 TilePos = TileMap->GetTileEmpty(Index)->GetWorldPos();

					// 렉트 충돌
					if (CurLB.x <= TilePos.x + TileSize.x &&
						CurRT.x >= TilePos.x &&
						CurLB.y <= TilePos.y + TileSize.y &&
						CurRT.y >= TilePos.y
						)
					{
						if (m_NormalBottomCheckApplied == false &&
							TilePos.y + TileSize.y - GetWorldScale().y <= ResultLB.y &&
							ResultLB.y <= TilePos.y + TileSize.y + GetWorldScale().y)
							continue;
						
						return true;
					}
				}
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


					if (CurLB.x <= TilePos.x + TileSize.x &&
						CurRT.x >= TilePos.x &&
						CurLB.y <= TilePos.y + TileSize.y &&
						CurRT.y >= TilePos.y)
					{
						if (m_NormalBottomCheckApplied == false &&
							TilePos.y + TileSize.y - GetWorldScale().y <= ResultLB.y &&
							ResultLB.y <= TilePos.y + TileSize.y + GetWorldScale().y)
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

		// 해당 위치의 Tile을 구해온다.
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

		// 위에서 아래로 반복한다
		for (int row = TopIdx; row >= BottomIdx; row--)
		{
			for (int col = LeftIdx; col <= RightIdx; col++)
			{
				// 이전 위치의 Bottom이, 타일의 Top 보다 클 경우 무시한다
				// 즉, 내가 점프하는데, 그 위에 Tile이 존재하는 것
				// 바닥 체크는 내려갈 때만 체크하기 위함이다.
				if (TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Block &&
					TileMap->GetTileEmpty(col, row)->GetPos().y + TileSize.y > PrevBottom)
					continue;

				// 못가는 곳이라면. 즉, 바닥에 닿는 다면
				if (TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Wall ||
					TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Block)
				{
					CTileEmpty* Tile =  TileMap->GetTileEmpty(col, row);

					float NewYPos = Tile->GetWorldPos().y + TileSize.y * Pivot.y;

					SetWorldPos(m_Pos.x, NewYPos, m_Pos.z);

					return true;
				}
			}
		}
	}

	return false;
}

void CAttackEffect::UpdateGravityEffect(float DeltaTime)
{
	if (m_PhysicsSimulate)
	{
		float Velocity = 0.f;

		m_FallTime += DeltaTime * 10.f;

		if (m_Jump)
		{
			Velocity = m_JumpVelocity * m_FallTime;
		}

		// float FallDistance = GRAVITY * 0.5f * m_FallTime * m_FallTime;
		float FallDistance = GRAVITY * 0.5f * m_FallTime * m_FallTime;

		SetWorldPos(GetWorldPos().x, m_FallStartY + (Velocity - FallDistance), GetWorldPos().z);
	}
}

void CAttackEffect::MonsterAttackCollisionCallback(const CollisionResult& Result)
{
	Destroy();

	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner == m_Scene->GetPlayerObject())
	{
		CPlayer2D* Player = (CPlayer2D*)Owner;

		// HP Bar 달게 하기
		float DamageFloat = m_MonsterOwner->GetAttackAbility();

		Player->Damage(m_MonsterOwner->GetAttackAbility());

		Player->SetIsBeingHit();

		if (m_AttackDir.x > 0)
			Player->SetBeingHitDirection(m_AttackDir.x);
		else
			Player->SetBeingHitDirection(m_AttackDir.x);

		// DestMonster->Damage(2.f);

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");
			DamageFont->SetDamage((int)m_MonsterOwner->GetAttackAbility());
		}
	}
}

void CAttackEffect::Start()
{
	CGameObject::Start();
}

bool CAttackEffect::Init()
{
	if (!CGameObject::Init())
		return false;

	m_MainSprite = CreateComponent<CSpriteComponent>("MainSprite");
	m_MainSprite->SetRenderState("AlphaBlend");
	SetRootComponent(m_MainSprite);
	m_MainSprite->SetPivot(0.5f, 0.5f, 0.0f);

	m_Collider = CreateComponent<CColliderCircle>("FirstCollider");
	m_MainSprite->AddChild(m_Collider);

	return true;
}

void CAttackEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (CheckSideCollision())
	{
		SideCollisionSpecificAction();
	}

	if (CheckBottomCollision())
	{
		BottomCollisionSpecificAction();
	}

	UpdateGravityEffect(DeltaTime);


	m_PrevPos = GetWorldPos();

}

void CAttackEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
