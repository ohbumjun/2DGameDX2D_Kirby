#include "HammerGorillaFarAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "MiddleBossHammer.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"
#include "Scene/CameraManager.h"

CHammerGorillaFarAttack::CHammerGorillaFarAttack() :
	m_AttackDistLimit(0.f),
	m_AttackDistLimitMax(550.f),
	m_CreateMultipleAfter(false)
{}

CHammerGorillaFarAttack::CHammerGorillaFarAttack(const CHammerGorillaFarAttack& Attack) : CAttackEffect(Attack)
{}

CHammerGorillaFarAttack::~CHammerGorillaFarAttack()
{}

void CHammerGorillaFarAttack::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CHammerGorillaFarAttack::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}

void CHammerGorillaFarAttack::MakeMultipleAttackEffect()
{
	if (!m_CreateMultipleAfter)
		return;

	CHammerGorillaFarAttack* AttackEffect = nullptr;

	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect();

	float Number0To1 = (float)rand() / (float)RAND_MAX;

	// ������ 2��
	for (int i = 0; i < 3;i++)
	{
		Vector3 TargetPos = {};
		TargetPos.x = GetWorldPos().x + cosf(DegreeToRadian((90.f - 35.f * i + Number0To1 * 40.f))) * 30.f;
		TargetPos.y = GetWorldPos().y + sinf(DegreeToRadian((90.f - 35.f * i + Number0To1 * 40.f))) * 30.f;

		Vector3 TraceDir = TargetPos - GetWorldPos();

		TraceDir.Normalize();

		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");

		AttackEffect->SetRightAttackDir(TraceDir.y);
		// AttackEffect->SetRightAttackDir(0.f);

		AttackEffect->SetJumpVelocity(10.f + i * 30.f);

		AttackEffect->SetWorldScale(30.f, 30.f, 1.f);

		AttackEffect->SetWorldPos(GetWorldPos());

		AttackEffect->JumpStart();

		AttackEffect->SetPhysicsSimulate(true);

		AttackEffect->SetMonsterOwner(m_MonsterOwner);
	}

	// ���� 2��
	for (int i = 0; i < 3; i++)
	{
		Vector3 TargetPos = {};

		TargetPos.x = GetWorldPos().x + cosf(DegreeToRadian(90.f + 35.f * i + Number0To1 * 40.f)) * 30.f;
		TargetPos.y = GetWorldPos().y + sinf(DegreeToRadian(90.f + 30.f * i)) * 30.f;

		Vector3 TraceDir = TargetPos - GetWorldPos();

		TraceDir.Normalize();

		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");

		AttackEffect->SetLeftAttackDir(TraceDir.y);
		// AttackEffect->SetLeftAttackDir(0.f);

		AttackEffect->SetJumpVelocity(10.f + i * 30.f);

		AttackEffect->SetWorldScale(30.f, 30.f, 1.f);

		AttackEffect->SetWorldPos(GetWorldPos());

		AttackEffect->JumpStart();

		AttackEffect->SetPhysicsSimulate(true);

		AttackEffect->SetMonsterOwner(m_MonsterOwner);
	}
}

void CHammerGorillaFarAttack::Start()
{
	CAttackEffect::Start();
}

bool CHammerGorillaFarAttack::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"HammerMiddleBossFarAttack", TEXT("Effect_MiddleBossHammerFarAttack.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(80.f, 80.f, 1.f);

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, (CAttackEffect*)this, &CAttackEffect::MonsterAttackCollisionCallback);

	return true;
}

void CHammerGorillaFarAttack::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	float MoveDist = std::abs(m_AttackDir.x) * DeltaTime * 350.f;

	AddWorldPos(Vector3(m_AttackDir.x, m_AttackDir.y, 0.f) * DeltaTime * 350.f);

	if (m_AttackDistLimit < m_AttackDistLimitMax)
	{
		m_AttackDistLimit += MoveDist;
	}

	if (m_AttackDistLimit >= m_AttackDistLimitMax)
	{
		Destroy();

		m_MonsterOwner->SetAttackEnd();

		MakeMultipleAttackEffect();
	}
}

void CHammerGorillaFarAttack::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}
