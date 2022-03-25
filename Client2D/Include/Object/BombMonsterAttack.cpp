#include "BombMonsterAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "BeamMonster.h"
#include "FightMonster.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CBombMonsterAttack::CBombMonsterAttack() :
	m_AttackDistLimit(0.f),
	m_AttackDistLimitMax(1000.f)
{}

CBombMonsterAttack::CBombMonsterAttack(const CBombMonsterAttack& Attack) : CAttackEffect(Attack)
{}

CBombMonsterAttack::~CBombMonsterAttack()
{}

void CBombMonsterAttack::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->ChangeAnimation("EffectRight");
}

void CBombMonsterAttack::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->ChangeAnimation("EffectLeft");
}

void CBombMonsterAttack::Start()
{
	CAttackEffect::Start();
}

bool CBombMonsterAttack::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BombAttackEffect", TEXT("Ability_Bomb_ThrowBomb.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(200.f, 200.f, 1.f);

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.3f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, (CAttackEffect*)this, &CAttackEffect::MonsterAttackCollisionCallback);
	m_Collider->AddCollisionCallback(Collision_State::Begin, this, &CBombMonsterAttack::ExplodeEffect);

	return true;
}

void CBombMonsterAttack::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	float MoveDist = std::abs(m_AttackDir.x) * DeltaTime * 500.f;

	AddWorldPos(Vector3(m_AttackDir.x, 0.f, 0.f) * DeltaTime * 500.f);

	if (m_AttackDistLimit < m_AttackDistLimitMax)
	{
		m_AttackDistLimit += MoveDist;
	}
	if (m_AttackDistLimit >= m_AttackDistLimitMax)
	{
		Destroy();

		m_MonsterOwner->SetAttackEnd();

		CollisionResult Result;
		ExplodeEffect(Result);
	}
}

void CBombMonsterAttack::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

void CBombMonsterAttack::SetExplodeAnimation()
{}

void CBombMonsterAttack::ExplodeEffect(const CollisionResult& Result)
{
	CBombMonsterAttack* ExplodeEffect = m_Scene->CreateGameObject<CBombMonsterAttack>("Explode");
	ExplodeEffect->SetRightAttackDir();
	ExplodeEffect->SetAttackDirX(0.f);
	ExplodeEffect->SetWorldPos(GetWorldPos());
	ExplodeEffect->SetLifeTime(0.2f);
}
