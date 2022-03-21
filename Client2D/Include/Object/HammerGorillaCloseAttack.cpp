#include "HammerGorillaCloseAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "MiddleBossHammer.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CHammerGorillaCloseAttack::CHammerGorillaCloseAttack() 
{}

CHammerGorillaCloseAttack::CHammerGorillaCloseAttack(const CHammerGorillaCloseAttack& Attack) : CAttackEffect(Attack)
{}

CHammerGorillaCloseAttack::~CHammerGorillaCloseAttack()
{}

void CHammerGorillaCloseAttack::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CHammerGorillaCloseAttack::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}


void CHammerGorillaCloseAttack::Start()
{
	CAttackEffect::Start();
}

bool CHammerGorillaCloseAttack::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"HammerGorillaCloseAttack", TEXT("Kirby_Fire_Effect_NormalAttack.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(150.f, 150.f, 1.f);

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, (CAttackEffect*)this, &CAttackEffect::MonsterAttackCollisionCallback);

	return true;
}

void CHammerGorillaCloseAttack::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	AddWorldPos(Vector3(m_AttackDir.x, m_AttackDir.y, 0.f) * DeltaTime * 350.f);
}

void CHammerGorillaCloseAttack::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}
