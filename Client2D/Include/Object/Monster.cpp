#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "MonsterAnimation.h"

CMonster::CMonster() :
	m_HP(15.f)
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& Monster)
{}

CMonster::~CMonster()
{}

void CMonster::Damage(float Damage)
{
	m_HP -= Damage;
	if (m_HP < 0.f)
		Destroy();
}

bool CMonster::Init()
{

	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("MonsterSprite");
	m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody->SetCollisionProfile("Monster");

	SetRootComponent(m_Sprite);

	m_Sprite->AddChild(m_ColliderBody);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);
}

void CMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMonster::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}
