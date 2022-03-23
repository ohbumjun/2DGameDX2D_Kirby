#include "Ladder.h"
#include "../EditorManager.h"
#include "Component/TileEmptyComponent.h"
#include "Scene/Scene.h"
#include "Component/ColliderBox2D.h"

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
}

bool CLadder::Init()
{
	if (!CGameObject::Init())
		return false;

	// m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody->SetCollisionProfile("MonsterAttack");
	SetWorldScale(70.f, 70.f, 1.f); //
	m_ColliderBody->SetExtend(50.f, 50.f);
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CLadder::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	m_ColliderBody->SetExtend(m_RootComponent->GetWorldScale().x * 0.5f, m_RootComponent->GetWorldScale().y * 0.5f);
}

void CLadder::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
