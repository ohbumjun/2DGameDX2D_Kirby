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
	m_AttackDistLimitMax(550.f),
	m_ExplodeByCollision(true)
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
		"BombAttack", TEXT("Ability_Bomb_ThrowBomb.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(80.f, 80.f, 1.f);
	m_MainSprite->GetAnimationInstance()->Play();
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");

	m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("Explode")->SetLoop(true);
	m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectLeft")->SetPlayTime(0.4f);
	m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectRight")->SetPlayTime(0.4f);


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

		m_ExplodeByCollision = false;

		CollisionResult Result;

		ExplodeEffect(Result);
	}
}

void CBombMonsterAttack::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

void CBombMonsterAttack::SetExplodeAnimation()
{
	m_MainSprite->GetAnimationInstance()->ChangeAnimation("Explode");
}

void CBombMonsterAttack::ApplyJumpEffect()
{
	// Jump
	m_PhysicsSimulate = true;
	m_Jump = true;
	m_JumpVelocity = 40.f;
	m_FallStartY = GetWorldPos().y;
	m_FallTime = 0.f;
}

void CBombMonsterAttack::ExplodeEffect(const CollisionResult& Result)
{
	if (m_ExplodeByCollision)
	{
		CColliderComponent* CollisionDest = Result.Dest;

		if (!CollisionDest)
			return;

		CGameObject* Owner = CollisionDest->GetGameObject();

		if (Owner && Owner == m_Scene->GetPlayerObject())
		{
			CPlayer2D* Player = (CPlayer2D*)Owner;

			// Scene Change 가 일어나고 있다면
			if (Player->IsSceneChanging())
				return;

			// Player 의 본체일 때만 Damage --> Pull Right, Left Collider 일 때는 X
			if (Player->GetBodyCollider() != (CColliderBox2D*)Result.Dest)
				return;
		}
	}

	CBombMonsterAttack* ExplodeEffect = m_Scene->CreateGameObject<CBombMonsterAttack>("Explode");
	ExplodeEffect->SetRightAttackDir();
	ExplodeEffect->SetAttackDirX(0.f);
	ExplodeEffect->SetWorldPos(GetWorldPos());
	ExplodeEffect->SetWorldScale(180.f, 180.f, 1.f);
	ExplodeEffect->SetLifeTime(0.5f);
	ExplodeEffect->SetExplodeAnimation();
	ExplodeEffect->m_Collider->Enable(false);
}
