#include "BulletTornaido.h"
#include "Component/SpriteComponent.h"

CBulletTornaido::CBulletTornaido()
{
}

CBulletTornaido::CBulletTornaido(const CBulletTornaido& obj) :
	CGameObject(obj)
{
	m_Root   = static_cast<CSceneComponent*>(FindComponent("Root"));
	m_Root1  = static_cast<CSceneComponent*>(FindComponent("Root1"));
	m_Sprite = static_cast<CSpriteComponent*>(FindComponent("BulletSprite"));
}

CBulletTornaido::~CBulletTornaido()
{
}

bool CBulletTornaido::Init()
{
	m_Root   = CreateComponent<CSceneComponent>("Root");
	m_Root1  = CreateComponent<CSceneComponent>("Root1");
	m_Sprite = CreateComponent<CSpriteComponent>("BulletSprite");

	SetRootComponent(m_Root);

	m_Root->AddChild(m_Root1);
	m_Root1->AddChild(m_Sprite);

	m_Sprite->SetRelativeScale(50.f, 50.f, 1.f);
	m_Sprite->SetRelativePos(50.f, 0.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetInheritRotZ(true);

	return true;
}

void CBulletTornaido::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	AddRelativePos(GetWorldAxis(AXIS_Y) * 500.f * DeltaTime);

	m_Root1->AddRelativeRotationZ(720.f * DeltaTime);
}

void CBulletTornaido::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBulletTornaido* CBulletTornaido::Clone()
{
	return new CBulletTornaido(*this);
}
