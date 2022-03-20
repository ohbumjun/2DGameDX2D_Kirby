#include "BeamMonsterAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "BeamMonster.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CBeamMonsterAttack::CBeamMonsterAttack() :
	m_AttackImageSize(65.f),
	m_RotationSpeed(120.f),
	m_AttackDir(1.f)
{
}

CBeamMonsterAttack::CBeamMonsterAttack(const CBeamMonsterAttack& Attack) : CAttackEffect(Attack)
{}

CBeamMonsterAttack::~CBeamMonsterAttack()
{}

void CBeamMonsterAttack::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->SetWorldRotationZ(70.f);
	m_SecondSprite->SetRelativePos(m_AttackImageSize, 0.f, 0.f);
	m_ThirdSprite->SetRelativePos(m_AttackImageSize * 2.f, 0.f, 0.f);
	m_FourthSprite->SetRelativePos(m_AttackImageSize * 3.f, 0.f, 0.f);
}

void CBeamMonsterAttack::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->SetWorldRotationZ(-70.f);
	m_SecondSprite->SetRelativePos(m_AttackImageSize * -1.f, 0.f, 0.f);
	m_ThirdSprite->SetRelativePos(m_AttackImageSize * 2.f * -1.f, 0.f, 0.f);
	m_FourthSprite->SetRelativePos(m_AttackImageSize * 3.f * -1.f, 0.f, 0.f);
}

void CBeamMonsterAttack::Start()
{
	CAttackEffect::Start();
}

bool CBeamMonsterAttack::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BeamAttackEffect", TEXT("Ability_Beam_AttackEffect.anim"));

	AnimationInstance->Play();

	float AnimDelayTime = AnimationInstance->GetCurrentAnimation()->GetPlayTime()
	/	AnimationInstance->GetCurrentAnimation()->GetFrameCount();

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetInfo(Vector2(0.f ,0.f), m_MainSprite->GetWorldScale().x * 0.3f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, this, &CBeamMonsterAttack::CollisionCallback);

	m_SecondSprite = CreateComponent<CSpriteComponent>("SecondSprite");
	m_SecondSprite->SetAnimationInstance(AnimationInstance);
	m_SecondSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);
	m_SecondSprite->SetRelativePos(m_AttackImageSize, 0.f, 0.f);
	m_SecondSprite->GetAnimationInstance()->GetCurrentAnimation()->SetInitPauseTime(AnimDelayTime * 0.5f);
	m_SecondSprite->SetInheritRotZ(true);
	m_SecondSprite->SetPivot(0.5f, 0.5f, 0.f);

	CColliderCircle* ColliderCirle = CreateComponent<CColliderCircle>("FirstCollider");
	m_SecondSprite->AddChild(ColliderCirle);
	ColliderCirle->SetCollisionProfile("MonsterAttack");
	ColliderCirle->SetInfo(Vector2(0.f, 0.f), m_SecondSprite->GetWorldScale().x * 0.3f);
	ColliderCirle->AddCollisionCallback(Collision_State::Begin, this, &CBeamMonsterAttack::CollisionCallback);

	m_ThirdSprite = CreateComponent<CSpriteComponent>("ThirdSprite");
	m_ThirdSprite->SetAnimationInstance(AnimationInstance);
	m_ThirdSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);
	m_ThirdSprite->SetRelativePos(m_AttackImageSize * 2.f, 0.f, 0.f);
	m_ThirdSprite->SetInheritRotZ(true);
	m_ThirdSprite->SetPivot(0.5f, 0.5f, 0.f);
	ColliderCirle = CreateComponent<CColliderCircle>("FirstCollider");
	m_ThirdSprite->AddChild(ColliderCirle);
	ColliderCirle->SetCollisionProfile("MonsterAttack");
	ColliderCirle->SetInfo(Vector2(0.f, 0.f), m_ThirdSprite->GetWorldScale().x * 0.3f);
	ColliderCirle->AddCollisionCallback(Collision_State::Begin, this, &CBeamMonsterAttack::CollisionCallback);


	m_FourthSprite = CreateComponent<CSpriteComponent>("FourthSprite");
	m_FourthSprite->SetAnimationInstance(AnimationInstance);
	m_FourthSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);
	m_FourthSprite->SetRelativePos(m_AttackImageSize * 3.f, 0.f, 0.f);
	m_FourthSprite->GetAnimationInstance()->GetCurrentAnimation()->SetInitPauseTime(AnimDelayTime * 1.5f);
	m_FourthSprite->SetInheritRotZ(true);
	m_FourthSprite->SetPivot(0.5f, 0.5f, 0.f);
	ColliderCirle = CreateComponent<CColliderCircle>("FirstCollider");
	m_FourthSprite->AddChild(ColliderCirle);
	ColliderCirle->SetCollisionProfile("MonsterAttack");
	ColliderCirle->SetInfo(Vector2(0.f, 0.f), m_FourthSprite->GetWorldScale().x * 0.3f);
	ColliderCirle->AddCollisionCallback(Collision_State::Begin, this, &CBeamMonsterAttack::CollisionCallback);


	m_MainSprite->AddChild(m_SecondSprite);
	m_MainSprite->AddChild(m_ThirdSprite);
	m_MainSprite->AddChild(m_FourthSprite);

	m_MainSprite->SetWorldRotationZ(70.f);

	m_RotateLimit = 70.f * 2;

	return true;
}

void CBeamMonsterAttack::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	// 오른쪽 공격
	if (m_AttackDir > 0.f)
	{
		m_MainSprite->AddRelativeRotationZ(m_RotationSpeed * DeltaTime * -1.f);

		m_RotateLimit += (m_RotationSpeed * DeltaTime * -1.f);
	}
	// 왼쪽 공격
	else
	{
		m_MainSprite->AddRelativeRotationZ(m_RotationSpeed * DeltaTime);

		m_RotateLimit -= m_RotationSpeed * DeltaTime;
	}
	

	if (m_RotateLimit < 0.f)
	{
		Destroy();

		if (m_BeamOwner)
		{
			m_BeamOwner->SetAttackEnd();
		}
	}
}

void CBeamMonsterAttack::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

void CBeamMonsterAttack::CollisionCallback(const CollisionResult& Result)
{
	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner == m_Scene->GetPlayerObject())
	{
		CPlayer2D* Player = (CPlayer2D*)Owner;

		// HP Bar 달게 하기
		Player->SetIsBeingHit();

		if (m_AttackDir > 0)
			Player->SetBeingHitDirection(m_AttackDir);
		else
			Player->SetBeingHitDirection(m_AttackDir);

		// DestMonster->Damage(2.f);

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");
		}
	}
}
