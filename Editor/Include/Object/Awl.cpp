#include "Awl.h"
#include "../EditorManager.h"
#include "Component/TileEmptyComponent.h"
#include "Scene/Scene.h"
#include "Component/ColliderBox2D.h"

class CColliderCircle;

CAwl::CAwl()
{
	SetTypeID<CAwl>();
}

CAwl::~CAwl()
{}

void CAwl::Start()
{
	CGameObject::Start();
}

bool CAwl::Init()
{
	if (!CGameObject::Init())
		return false;

	// m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody->SetCollisionProfile("MonsterAttack");
	SetWorldScale(100.f, 100.f, 1.f); //
	m_ColliderBody->SetExtend(50.f, 50.f);

	return true;
}

void CAwl::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	m_ColliderBody->SetExtend(m_RootComponent->GetWorldScale().x * 0.5f, m_RootComponent->GetWorldScale().y * 0.5f);
}

void CAwl::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
