#include "KirbyAttackEffect.h"
#include "../Component/BombKirbyState.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/CameraManager.h"
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
	m_DestroyWhenCollide(true),
	m_CollideShakeEffect(false),
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

void CKirbyAttackEffect::ApplyFallEffect()
{
	m_PhysicsSimulate = true;
	m_Jump = true;
	m_FallStartY = GetWorldPos().y;
	m_FallTime = 0.f;
}

void CKirbyAttackEffect::ApplyBombFallAttackEnd()
{
	// BombFall의 경우, Player 에게
		// 1) 카메라 돌려주고
		// 2) Physics 다시 세팅한다.
	if (m_AttackType == KirbyAttackEffect_Type::BombFall)
	{
		m_Scene->GetCameraManager()->ReturnCamera();

		m_Camera = nullptr;

		CBombKirbyState* OwnerKirby = dynamic_cast<CBombKirbyState*>(m_KirbyOwner);

		if (!OwnerKirby)
			return;

		OwnerKirby->SetFallAttackEnd();
	}
}

void CKirbyAttackEffect::MakeBombExplodeEffect()
{
	if (m_AttackType == KirbyAttackEffect_Type::Bomb ||
		m_AttackType == KirbyAttackEffect_Type::BombFall ||
		m_AttackType == KirbyAttackEffect_Type::BombSpecial )
	{
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Bomb);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetAttackDirX(0.f);
		AttackEffect->m_MainSprite->GetAnimationInstance()->ChangeAnimation("Explode");
		AttackEffect->m_MainSprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayTime(0.5f);
		AttackEffect->SetWorldScale(160.f, 180.f, 1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x, GetWorldPos().y - GetWorldScale().y * GetPivot().y, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(m_KirbyOwner);
		AttackEffect->m_Collider->Enable(false);
		AttackEffect->SetLifeTime(0.5f);
	}
}

void CKirbyAttackEffect::CreateAttackBackEffect()
{
	CFireAttackBackEffect* BackEffect = m_Scene->CreateGameObject<CFireAttackBackEffect>("BackFire");

	BackEffect->SetWorldPos(GetWorldPos().x, GetWorldPos().y - GetWorldScale().y * GetPivot().y, GetWorldPos().z);

	BackEffect->SetWorldScale(60.f, 60.f, 1.f);

	BackEffect->GetColliderBody()->SetInfo(Vector2(0.f, 0.f), BackEffect->GetWorldScale().x * 0.8f);

	BackEffect->AddRelativeRotationZ(90.f);

	BackEffect->SetLifeTime(0.4f);
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
		m_MainSprite->SetWorldScale(60.f, 60.f, 1.f);

		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.6f);

		m_AttackDistLimitMax = 700.f;
		m_AttackObjectSpeed = 400.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyNormalBeamAttackEffect", TEXT("Kirby_Beam_Effect_NormalAttack.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_SideCollisionApplied = false;

		m_BottomCollisionApplied = false;
	}
	break;
	case KirbyAttackEffect_Type::BeamSpecial:
	{
		m_MainSprite->SetWorldScale(200.f, 200.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

		m_AttackDistLimitMax = 1200.f;
		m_AttackObjectSpeed = 900.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyNormalBeamAttackEffect", TEXT("Kirby_Beam_Effect_NormalAttack.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_SideCollisionApplied = false;

		m_BottomCollisionApplied = true;
	}
		break;
	case KirbyAttackEffect_Type::BeamSpark:
	{
		m_MainSprite->SetWorldScale(30.f, 30.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

		m_AttackDistLimitMax = 700.f;
		m_AttackObjectSpeed = 500.f;

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
		m_MainSprite->SetWorldScale(70.f, 70.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.6f);

		m_AttackDistLimitMax = 1100.f;
		m_AttackObjectSpeed = 900.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyFireFallAttackEffect", TEXT("Kirby_Fire_Effect_ComeDownFireEffect.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_BottomCollisionApplied = true;
	}
		break;
	case KirbyAttackEffect_Type::Bomb:
	{
		m_MainSprite->SetWorldScale(80.f, 80.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

		m_AttackDistLimitMax = 750.f;
		m_AttackObjectSpeed = 600.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyNormalBombAttackEffect", TEXT("Ability_Bomb_ThrowBomb.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectRight")->SetPlayTime(0.1f);
		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectLeft")->SetPlayTime(0.1f);

		m_BottomCollisionApplied = true;
	}
		break;
	case KirbyAttackEffect_Type::BombFall:
	{
		m_MainSprite->SetWorldScale(250.f, 250.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

		m_AttackDistLimitMax = 1300.f;
		m_AttackObjectSpeed = 600.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyFallBombAttackEffect", TEXT("Ability_Bomb_ThrowBomb.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectRight")->SetPlayTime(0.1f);
		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectLeft")->SetPlayTime(0.1f);

		m_BottomCollisionApplied = true;
		
	}
		break;
	case KirbyAttackEffect_Type::BombSpecial:
	{
		m_MainSprite->SetWorldScale(220.f, 220.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.5f);

		m_AttackDistLimitMax = 1000.f;
		m_AttackObjectSpeed = 800.f;

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyFallBombAttackEffect", TEXT("Ability_Bomb_ThrowBomb.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectRight")->SetPlayTime(0.1f);
		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectLeft")->SetPlayTime(0.1f);

		m_BottomCollisionApplied = true;
	}
	break;
	case KirbyAttackEffect_Type::Sword:
	{
		m_AttackDistLimitMax = 1000.f;
		m_AttackObjectSpeed = 700.f;

		m_MainSprite->SetWorldScale(170.f, 170.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyNormalSwordAttackEffect", TEXT("Ability_Sword_AttackEffect.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectRight")->SetPlayTime(0.3f);
		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectLeft")->SetPlayTime(0.3f);
	}
		break;
	case KirbyAttackEffect_Type::SwordFall:
	{
		m_AttackDistLimitMax = 1000.f;
		m_AttackObjectSpeed = 900.f;

		m_MainSprite->SetWorldScale(250.f, 250.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.5f);

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyFallSwordAttackEffect", TEXT("Kirby_Cutter_Boomerang.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectRight")->SetPlayTime(0.5f);
		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectLeft")->SetPlayTime(0.5f);
	}
		break;
	case KirbyAttackEffect_Type::FightSpecial:
	{
		m_AttackDistLimitMax = 2100.f;
		m_AttackObjectSpeed = 700.f;

		m_MainSprite->SetWorldScale(85.f, 85.f, 1.f);
		m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.5f);

		AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
			"KirbyFightSpecialEffect", TEXT("Kirby_Fight.anim"));

		m_MainSprite->SetAnimationInstance(AnimationInstance);

		m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("RightSpecialEffect");

		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftSpecialEffect")->SetPlayTime(2.0f);
		m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("RightSpecialEffect")->SetPlayTime(2.0f);

		m_SideCollisionApplied = false;

		m_DestroyWhenCollide = false;
	}
	break;
	}
}

void CKirbyAttackEffect::ApplyCameraMove()
{
	if (m_Camera)
	{
		m_Scene->GetCameraManager()->KeepCamera();
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
	}
}

void CKirbyAttackEffect::BottomCollisionSpecificAction()
{
	// Bomb 일 경우에는, 다른 Effect를 줄 것이다
	if (m_AttackType == KirbyAttackEffect_Type::Bomb || 
		m_AttackType == KirbyAttackEffect_Type::BombFall || 
		m_AttackType == KirbyAttackEffect_Type::BombSpecial)
	{
		MakeBombExplodeEffect();

		// BombFall의 경우, Player 에게
		// 1) 카메라 돌려주고
		// 2) Physics 다시 세팅한다.
		ApplyBombFallAttackEnd();
	}
	else
	{
		// Attack Back Effect
		CFireAttackBackEffect* BackEffect = m_Scene->CreateGameObject<CFireAttackBackEffect>("BackFire");

		BackEffect->SetWorldPos(GetWorldPos().x, GetWorldPos().y - GetWorldScale().y * GetPivot().y , GetWorldPos().z);

		BackEffect->SetWorldScale(GetWorldScale().x * 0.8f, GetWorldScale().y * 0.8f, 1.f);

		BackEffect->GetColliderBody()->SetInfo(Vector2(0.f, 0.f), BackEffect->GetWorldScale().x * 0.5f);

		BackEffect->AddRelativeRotationZ(90.f);

		BackEffect->SetLifeTime(0.4f);
	}

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

	m_Camera = CreateComponent<CCameraComponent>("BombFallCamera");

	m_MainSprite->AddChild(m_Camera);

	m_Camera->OnViewportCenter();

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
		ApplyBombFallAttackEnd();

		MakeBombExplodeEffect();

		Destroy();
	}
}

void CKirbyAttackEffect::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

void CKirbyAttackEffect::CollisionCallback(const CollisionResult& Result)
{
	ApplyBombFallAttackEnd();

	if (Result.Dest->GetGameObject()->CheckType<CBlock>())
	{
		// Attack Back Effect
		CreateAttackBackEffect();
		return;
	}

	if (m_DestroyWhenCollide)
		Destroy();

	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner)
	{
		CMonster* DestMonster = dynamic_cast<CMonster*>(Owner);

		if (!DestMonster)
			return;

		// 이미 맞은 상태였다면
		if (DestMonster->IsBeingHit())
			return;

		// Attack Back Effect
		CreateAttackBackEffect();

		DestMonster->Damage(m_AttackDamage);

		DestMonster->SetBeingHit(true);

		DestMonster->SetAIState(Monster_AI::Hit);

		if (m_AttackDir.x < 0)
			DestMonster->SetObjectMoveDir(Vector3(-1.f, 0.f, 0.f));
		else
			DestMonster->SetObjectMoveDir(Vector3(1.f, 0.f, 0.f));

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (m_CollideShakeEffect)
		{
			m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect();
		}

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");

			DamageFont->SetDamage((int)m_AttackDamage);
		}
	}
}
