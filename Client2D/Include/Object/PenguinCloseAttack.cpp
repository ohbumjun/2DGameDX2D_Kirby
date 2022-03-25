#include "PenguinCloseAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "MiddleBossHammer.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CPenguinCloseAttack::CPenguinCloseAttack()
{}

CPenguinCloseAttack::CPenguinCloseAttack(const CPenguinCloseAttack& Attack) : CAttackEffect(Attack)
{}

CPenguinCloseAttack::~CPenguinCloseAttack()
{}

void CPenguinCloseAttack::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CPenguinCloseAttack::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}


void CPenguinCloseAttack::Start()
{
	CAttackEffect::Start();

	m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectRight")->SetPlayTime(1.5f);
	m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectLeft")->SetPlayTime(1.5f);
}

bool CPenguinCloseAttack::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"PenguinCloseAttack", TEXT("Boss_Penguin_CloseStarAttack.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(160.f, 160.f, 1.f);

	SetLifeTime(1.5f);

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, (CAttackEffect*)this, &CAttackEffect::MonsterAttackCollisionCallback);

	return true;
}

void CPenguinCloseAttack::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	AddWorldPos(Vector3(m_AttackDir.x, m_AttackDir.y, 0.f) * DeltaTime * 350.f);

	AddRelativeRotationZ(360.f * DeltaTime);
}

void CPenguinCloseAttack::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}
