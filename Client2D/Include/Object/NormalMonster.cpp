#include "NormalMonster.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderComponent.h"
#include "UI/UIDamageFont.h"

CNormalMonster::CNormalMonster()
{
	// SetTypeID<CNormalMonster>();

	m_MonsterType = Monster_Type::Normal;

	m_AttackAbility = 20.f;
	m_HPMax = 100.f;
	m_HP = 100.f;
	m_AttackDistance = 30.f;
}

CNormalMonster::CNormalMonster(const CNormalMonster& Monster) : CMonster(Monster)
{}

CNormalMonster::~CNormalMonster()
{}

void CNormalMonster::Start()
{
	CMonster::Start();

	m_MonsterType = Monster_Type::Normal;

	m_AttackAbility = 20.f;
	m_HPMax = 100.f;
	m_HP = 100.f;

	// Death Animation 이후 바로 Destroy 세팅한다
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightDeath")->SetPlayTime(0.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightDeath")->SetEndFunction((CRef*)this, &CRef::Destroy);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftDeath")->SetPlayTime(0.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftDeath")->SetEndFunction((CRef*)this, &CRef::Destroy);

	m_ColliderBody->AddCollisionCallback(Collision_State::Begin,(CMonster*)this, &CNormalMonster::OnMonsterBodyCollisionBegin);
}

bool CNormalMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CNormalMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CNormalMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CNormalMonster* CNormalMonster::Clone()
{
	return new CNormalMonster(*this);
}

void CNormalMonster::Save(FILE* pFile)
{
	CMonster::Save(pFile);
}

void CNormalMonster::Load(FILE* pFile)
{
	CMonster::Load(pFile);
}