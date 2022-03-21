#include "Apple.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "BossTree.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CApple::CApple()
{}

CApple::CApple(const CApple& Attack) : CAttackEffect(Attack)
{}

CApple::~CApple()
{}

void CApple::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CApple::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}


void CApple::Start()
{
	CAttackEffect::Start();
}

bool CApple::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BossTreeApple", TEXT("Boss_Tree_AttackFar_Apple.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(120.f, 120.f, 1.f);

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, (CAttackEffect*)this, &CAttackEffect::MonsterAttackCollisionCallback);

	return true;
}

void CApple::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);
}

void CApple::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}
