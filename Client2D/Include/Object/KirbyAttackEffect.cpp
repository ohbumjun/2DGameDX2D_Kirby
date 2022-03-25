#include "KirbyAttackEffect.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "FireMonster.h"
#include "Player2D.h"
#include "Block.h"
#include "FireAttackBackEffect.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CKirbyAttackEffect::CKirbyAttackEffect() :
	m_AttackDistLimit(0.f),
	m_AttackDistLimitMax(500.f),
	m_AttackObjectSpeed(500.f)
{}

CKirbyAttackEffect::CKirbyAttackEffect(const CKirbyAttackEffect& Attack) : CAttackEffect(Attack)
{}

CKirbyAttackEffect::~CKirbyAttackEffect()
{}

void CKirbyAttackEffect::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CKirbyAttackEffect::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}

void CKirbyAttackEffect::ApplyJumpEffect()
{
	m_PhysicsSimulate = true;
	m_Jump = true;
	m_JumpVelocity = 60.f;
	m_FallStartY = GetWorldPos().y;
	m_FallTime = 0.f;
}

void CKirbyAttackEffect::SetAttackType(KirbyAttackEffect_Type Type)
{
	if (Type == m_AttackType)
		return;

	m_AttackType = Type;

	CAnimationSequence2DInstance* AnimationInstance = nullptr;

	switch(m_AttackType)
	{
	case KirbyAttackEffect_Type::Fire :
		{
		m_AttackDistLimitMax = 1000.f;
		m_AttackObjectSpeed = 500.f;

		m_MainSprite->SetWorldScale(170.f, 170.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyNormalFireAttack", TEXT("Kirby_Fire_Effect_NormalAttack.anim"));
		m_MainSprite->SetAnimationInstance(AnimationInstance);
		}
		break;
	case KirbyAttackEffect_Type::Fight:
	{
		m_AttackDistLimitMax = 500.f;
		m_AttackObjectSpeed = 1200.f;

		m_MainSprite->SetWorldScale(140.f, 140.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyNormalFightAttack", TEXT("Kirby_Fight_Effect_NormalAttack.anim"));
		m_MainSprite->SetAnimationInstance(AnimationInstance);
	}
		break;
	case KirbyAttackEffect_Type::Beam:
	{
		m_MainSprite->SetWorldScale(40.f, 40.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

		m_AttackDistLimitMax = 700.f;
		m_AttackObjectSpeed = 400.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyNormalBeamAttackEffect", TEXT("Kirby_Beam_Effect_NormalAttack.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_SideCollisionApplied = false;
	}
		break;
	case KirbyAttackEffect_Type::BeamSpark:
	{
		m_MainSprite->SetWorldScale(30.f, 30.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

		m_AttackDistLimitMax = 400.f;
		m_AttackObjectSpeed = 200.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyBeamFallAttackEffect", TEXT("Kirby_Beam_Effect_UpAttackEffect.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_MainSprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayScale(3.f);

		m_SideCollisionApplied = false;
	}
	break;
	case KirbyAttackEffect_Type::FightFall:
	{
		m_MainSprite->SetWorldScale(110.f, 110.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

		m_AttackDistLimitMax = 1000.f;
		m_AttackObjectSpeed = 800.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyFightFallAttackEffect", TEXT("Kirby_Fight_Effect_FallDownAttack.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);
	}
	break;
	case KirbyAttackEffect_Type::FireFall:
	{
		m_MainSprite->SetWorldScale(50.f, 50.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.6f);

		m_AttackDistLimitMax = 900.f;
		m_AttackObjectSpeed = 750.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyFireFallAttackEffect", TEXT("Kirby_Fire_Effect_ComeDownFireEffect.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_BottomCollisionApplied = true;
	}
	case KirbyAttackEffect_Type::Bomb:
	{
		m_MainSprite->SetWorldScale(70.f, 70.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.6f);

		m_AttackDistLimitMax = 900.f;
		m_AttackObjectSpeed = 750.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyBombAttackEffect", TEXT("Kirby_Bomb_BombThrowAttack.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_BottomCollisionApplied = true;
	}

	case KirbyAttackEffect_Type::BombFall:
	{
		m_MainSprite->SetWorldScale(50.f, 50.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.6f);

		m_AttackDistLimitMax = 900.f;
		m_AttackObjectSpeed = 750.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyFireFallAttackEffect", TEXT("Kirby_Fire_Effect_ComeDownFireEffect.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_BottomCollisionApplied = true;
	}
	break;
	}
}

void CKirbyAttackEffect::BottomCollisionSpecificAction()
{
	// Attack Back Effect
	CFireAttackBackEffect* BackEffect = m_Scene->CreateGameObject<CFireAttackBackEffect>("BackFire");

	BackEffect->SetWorldPos(GetWorldPos());

	BackEffect->SetWorldScale(60.f, 60.f, 1.f);

	BackEffect->GetColliderBody()->SetInfo(Vector2(0.f, 0.f), BackEffect->GetWorldScale().x * 0.5f);

	BackEffect->AddRelativeRotationZ(90.f);

	BackEffect->SetLifeTime(0.4f);

	Destroy();
}

void CKirbyAttackEffect::Start()
{
	CAttackEffect::Start();
}

bool CKirbyAttackEffect::Init()
{
	if (!CAttackEffect::Init())
		return false;

	m_AttackType = KirbyAttackEffect_Type::Fire;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"KirbyNormalFireAttack", TEXT("Kirby_Fire_Effect_NormalAttack.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(170.f, 170.f, 1.f);

	m_Collider->SetCollisionProfile("PlayerAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, this, &CKirbyAttackEffect::CollisionCallback);

	return true;
}

void CKirbyAttackEffect::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	float MoveDist = (Vector3(m_AttackDir.x, m_AttackDir.y, 0.f) * DeltaTime * m_AttackObjectSpeed).Length();

	AddWorldPos(Vector3(m_AttackDir.x, m_AttackDir.y, 0.f) * DeltaTime * m_AttackObjectSpeed);

	if (m_AttackDistLimit < m_AttackDistLimitMax)
	{
		m_AttackDistLimit += MoveDist;
	}
	if (m_AttackDistLimit >= m_AttackDistLimitMax)
	{
		Destroy();
	}
}

void CKirbyAttackEffect::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

void CKirbyAttackEffect::CollisionCallback(const CollisionResult& Result)
{
	Destroy();

	// 현재 충돌한 물체가 Block 이라면
	if (Result.Dest->GetGameObject()->CheckType<CBlock>())
	{
		// Attack Back Effect
		CFireAttackBackEffect* BackEffect = m_Scene->CreateGameObject<CFireAttackBackEffect>("BackFire");

		BackEffect->SetWorldPos(GetWorldPos());

		BackEffect->SetWorldScale(60.f, 60.f, 1.f);

		BackEffect->GetColliderBody()->SetInfo(Vector2(0.f, 0.f), BackEffect->GetWorldScale().x * 0.5f);

		BackEffect->AddRelativeRotationZ(90.f);

		BackEffect->SetLifeTime(0.4f);
	}

	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner)
	{
		CMonster* DestMonster = dynamic_cast<CMonster*>(Owner);

		if (!DestMonster)
			return;

		// Player 의 Attack + KirbyOwner 의 Attack
		float DamageAmount = m_KirbyOwner->GetExtraAttackAbility() + m_KirbyOwner->GetPlayer()->GetAttackAbility();

		// HP Bar 달게 하기
		DestMonster->Damage(DamageAmount);

		DestMonster->SetBeingHit(true);

		DestMonster->SetAIState(Monster_AI::Hit);

		if (m_AttackDir.x < 0)
			DestMonster->SetObjectMoveDir(Vector3(-1.f, 0.f, 0.f));
		else
			DestMonster->SetObjectMoveDir(Vector3(1.f, 0.f, 0.f));

		// DestMonster->Damage(2.f);

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");

			DamageFont->SetDamage((int)DamageAmount);
		}
	}
}
