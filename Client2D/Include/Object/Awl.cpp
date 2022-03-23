#include "Awl.h"
#include "Component/TileEmptyComponent.h"
#include "Scene/Scene.h"
#include "Component/ColliderBox2D.h"
#include "Player2D.h"
#include "UI/UIDamageFont.h"

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

	m_ColliderBody = FindComponentByType<CColliderBox2D>();

	m_ColliderBody->SetExtend(m_RootComponent->GetWorldScale().x * 0.5f,
		m_RootComponent->GetWorldScale().y * 0.5f);

	if (m_ColliderBody)
	{
		m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CAwl::SetCollisionCallback);
		m_ColliderBody->SetCollisionProfile("MonsterAttack");
	}
}

bool CAwl::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CAwl::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// m_ColliderBody->SetExtend(m_RootComponent->GetWorldScale().x * 0.5f, m_RootComponent->GetWorldScale().y * 0.5f);
}

void CAwl::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CAwl::SetCollisionCallback(const CollisionResult& Result)
{
	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner == m_Scene->GetPlayerObject())
	{
		CPlayer2D* Player = (CPlayer2D*)Owner;

		// HP Bar 달게 하기
		Player->Damage(90.f);

		Player->SetIsBeingHit();

		// 오른쪽으로 가고 있었다면 --> 왼쪽으로 튕겨내기
		if (Player->GetObjectMoveDir().x > 0)
			Player->SetBeingHitDirection(-1.f);
		// 왼쪽으로 가고 있었다면 --> 오른쪽으로 튕겨내기
		else
			Player->SetBeingHitDirection(1.f);

		// DestMonster->Damage(2.f);

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");
			DamageFont->SetDamage(90);
		}
	}
}