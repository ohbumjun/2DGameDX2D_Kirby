
#include "Bullet.h"
#include "Component/SpriteComponent.h"

CBullet::CBullet()	:
	m_Distance(600.f)
{
}

CBullet::CBullet(const CBullet& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("BulletSprite");
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

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	float	Dist = 500.f * DeltaTime;

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
