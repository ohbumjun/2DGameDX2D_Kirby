#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "MonsterAnimation.h"

CMonster::CMonster()
{}

CMonster::CMonster(const CMonster& Monster)
{}

CMonster::~CMonster()
{}

bool CMonster::Init()
{

	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("MonsterSprite");
	m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");

	SetRootComponent(m_Sprite);

	m_Sprite->AddChild(m_ColliderBody);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(100.f, 100.f, 1.f);
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
