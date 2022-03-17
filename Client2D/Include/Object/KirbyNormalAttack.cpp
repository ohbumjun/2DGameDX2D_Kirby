#include "KirbyNormalAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "FireMonster.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CKirbyNormalAttack::CKirbyNormalAttack() :
	m_AttackDir(1.f, 0.f),
	m_AttackDistLimit(0.f),
	m_AttackDistLimitMax(500.f),
	m_AttackObjectSpeed(500.f)
{}

CKirbyNormalAttack::CKirbyNormalAttack(const CKirbyNormalAttack& Attack) : CAttackEffect(Attack)
{}

CKirbyNormalAttack::~CKirbyNormalAttack()
{}

void CKirbyNormalAttack::SetRightAttackDir(float YDir)
{
	m_AttackDir.x = 1.f;
	m_AttackDir.y = YDir;
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CKirbyNormalAttack::SetLeftAttackDir(float YDir)
{
	m_AttackDir.x = -1.f;
	m_AttackDir.y = YDir;
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}

void CKirbyNormalAttack::SetAttackType(KirbyNormalAttack_Type Type)
{
	if (Type == m_AttackType)
		return;

	m_AttackType = Type;

	CAnimationSequence2DInstance* AnimationInstance = nullptr;

	switch(m_AttackType)
	{
	case KirbyNormalAttack_Type::Fire :
		{
		m_AttackDistLimitMax = 1000.f;
		m_AttackObjectSpeed = 500.f;

		m_Sprite->SetWorldScale(170.f, 170.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.4f);

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"FireAttackEffect", TEXT("Kirby_Fire_Effect_NormalAttack.anim"));
		m_Sprite->SetAnimationInstance(AnimationInstance);
		}
		break;
	case KirbyNormalAttack_Type::Fight:
	{
		m_AttackDistLimitMax = 500.f;
		m_AttackObjectSpeed = 1200.f;

		m_Sprite->SetWorldScale(140.f, 140.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.4f);

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"FightAttackEffect", TEXT("Kirby_Fight_Effect_NormalAttack.anim"));
		m_Sprite->SetAnimationInstance(AnimationInstance);
	}
		break;
	case KirbyNormalAttack_Type::Beam:
	{
		m_Sprite->SetWorldScale(40.f, 40.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.4f);

		m_AttackDistLimitMax = 700.f;
		m_AttackObjectSpeed = 400.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"BeamAttackEffect", TEXT("Kirby_Beam_Effect_NormalAttack.anim"));

		m_Sprite->SetAnimationInstance(AnimationInstance);

		m_SideCollisionApplied = false;
	}
		break;
	case KirbyNormalAttack_Type::FightFall:
	{
		m_Sprite->SetWorldScale(90.f, 90.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.4f);

		m_AttackDistLimitMax = 1000.f;
		m_AttackObjectSpeed = 600.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"FightFallAttackEffect", TEXT("Kirby_Fight_Effect_FallDownAttack.anim"));

		m_Sprite->SetAnimationInstance(AnimationInstance);

		m_SideCollisionApplied = false;
	}
	break;
	case KirbyNormalAttack_Type::FireFall:
	{
		m_Sprite->SetWorldScale(50.f, 50.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.6f);

		m_AttackDistLimitMax = 1000.f;
		m_AttackObjectSpeed = 900.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"FireFallAttackEffect", TEXT("Kirby_Fire_Effect_ComeDownFireEffect.anim"));

		m_Sprite->SetAnimationInstance(AnimationInstance);
	}
	break;
	}
}

void CKirbyNormalAttack::SetAttackDirX(float XDir)
{
	m_AttackDir.x = XDir;
}

void CKirbyNormalAttack::Start()
{
	CGameObject::Start();
}

bool CKirbyNormalAttack::Init()
{
	if (!CGameObject::Init())
		return false;

	m_AttackType = KirbyNormalAttack_Type::Fire;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"FireAttackEffect", TEXT("Kirby_Fire_Effect_NormalAttack.anim"));

	m_Sprite = CreateComponent<CSpriteComponent>("MainSprite");
	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->SetWorldScale(170.f, 170.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	m_Collider = CreateComponent<CColliderCircle>("FirstCollider");
	m_Sprite->AddChild(m_Collider);
	m_Collider->SetCollisionProfile("PlayerAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, this, &CKirbyNormalAttack::CollisionCallback);

	return true;
}

void CKirbyNormalAttack::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	float MoveDist = m_AttackDir.x * DeltaTime * m_AttackObjectSpeed;

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

void CKirbyNormalAttack::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

void CKirbyNormalAttack::CollisionCallback(const CollisionResult& Result)
{
	Destroy();

	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner)
	{
		CMonster* DestMonster = dynamic_cast<CMonster*>(Owner);

		if (!DestMonster)
			return;

		// HP Bar 달게 하기
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
		}
	}
}
