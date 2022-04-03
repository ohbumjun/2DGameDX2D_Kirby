#include "DynaCloseAttack.h"
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

CDynaCloseAttack::CDynaCloseAttack() :
	m_AttackDistLimit(0.f),
	m_AttackDistLimitMax(1000.f)
{}

CDynaCloseAttack::~CDynaCloseAttack()
{}

void CDynaCloseAttack::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->ChangeAnimation("EffectRight");
}

void CDynaCloseAttack::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->ChangeAnimation("EffectLeft");
}

void CDynaCloseAttack::Start()
{
	CAttackEffect::Start();
}

bool CDynaCloseAttack::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"DynaCloseAttackEffect", TEXT("Boss_Dyna_CloseAttack.anim"));

	float AnimDelayTime = AnimationInstance->GetCurrentAnimation()->GetPlayTime()
		/ AnimationInstance->GetCurrentAnimation()->GetFrameCount();

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(200.f, 200.f, 1.f);

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.3f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, (CAttackEffect*)this, &CAttackEffect::MonsterAttackCollisionCallback);

	return true;
}

void CDynaCloseAttack::Update(float DeltaTime)
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
	}
}

void CDynaCloseAttack::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}
