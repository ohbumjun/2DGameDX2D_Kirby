#include "Bullet.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"
#include "UI/UINumberWidget.h"
#include "UI/UIDamageFont.h"
#include "Monster.h"

CBullet::CBullet() :
	m_Distance(600.f)
{
}

CBullet::CBullet(const CBullet& obj) :
	CGameObject(obj)
{
	m_Sprite   = static_cast<CSpriteComponent*>(FindComponent("BulletSprite"));
	m_Distance = obj.m_Distance;
}

CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("BulletSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRelativeScale(50.f, 50.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	m_Body = CreateComponent<CColliderCircle>("ColliderCircle");
	m_Sprite->AddChild(m_Body);
	m_Body->AddCollisionCallback(Collision_State::Begin, this, &CBullet::BulletCollision);

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	float Dist = 500.f * DeltaTime;

	m_Distance -= Dist;

	if (m_Distance <= 0.f)
	{
		Destroy();
	}

	AddRelativePos(GetWorldAxis(AXIS_Y) * Dist);
}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::BulletCollision(const CollisionResult& Result)
{
	Destroy();

	CColliderComponent* CollisionDest = Result.Dest;
	CGameObject* Owner = CollisionDest->GetGameObject();
	CWidgetComponent* ObjectWindow = nullptr;
	if (Owner)
	{
		// HP Bar 달게 하기
		if (Owner->GetTypeID() == typeid(CMonster).hash_code())
		{
			CMonster* DestMonster = (CMonster*)Owner;
			DestMonster->Damage(2.f);
		}

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();
		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");
		}
	}
}
