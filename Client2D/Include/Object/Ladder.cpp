#include "Ladder.h"
#include "Component/TileEmptyComponent.h"
#include "Scene/Scene.h"
#include "Component/ColliderBox2D.h"
#include "Player2D.h"
#include "UI/UIDamageFont.h"

class CColliderCircle;

CLadder::CLadder()
{
	SetTypeID<CLadder>();
}

CLadder::~CLadder()
{}

void CLadder::Start()
{
	CGameObject::Start();

	m_ColliderBody = FindComponentByType<CColliderBox2D>();

	m_ColliderBody->SetExtend(m_RootComponent->GetWorldScale().x * 0.5f, 
		m_RootComponent->GetWorldScale().y * 0.5f);

	if (m_ColliderBody)
	{
		m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CLadder::SetBeginCollisionCallback);
		m_ColliderBody->AddCollisionCallback(Collision_State::End, this, &CLadder::SetEndCollisionCallback);
		m_ColliderBody->SetCollisionProfile("Monster");
	}
}

bool CLadder::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CLadder::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// m_ColliderBody->SetExtend(m_RootComponent->GetWorldScale().x * 0.5f, m_RootComponent->GetWorldScale().y * 0.5f);
}

void CLadder::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CLadder::SetBeginCollisionCallback(const CollisionResult& Result)
{
	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner == m_Scene->GetPlayerObject())
	{
		CPlayer2D* Player = (CPlayer2D*)Owner;

		Player->SetLadderState(true);
	}
}

void CLadder::SetEndCollisionCallback(const CollisionResult& Result)
{
	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner == m_Scene->GetPlayerObject())
	{
		CPlayer2D* Player = (CPlayer2D*)Owner;

		Player->SetLadderState(false);
		
	}
}