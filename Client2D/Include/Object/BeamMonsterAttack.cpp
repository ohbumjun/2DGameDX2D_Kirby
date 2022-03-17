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
	m_AttackImageSize(50.f),
	m_RotationSpeed(120.f),
	m_AttackDir(1.f)
{}

CBeamMonsterAttack::CBeamMonsterAttack(const CBeamMonsterAttack& Attack) : CGameObject(Attack)
{}

CBeamMonsterAttack::~CBeamMonsterAttack()
{}

void CBeamMonsterAttack::SetRightAttackDir()
{
	m_AttackDir = 1.f;

	m_FirstSprite->SetWorldRotationZ(70.f);
	m_SecondSprite->SetRelativePos(m_AttackImageSize, 0.f, 0.f);
	m_ThirdSprite->SetRelativePos(m_AttackImageSize * 2.f, 0.f, 0.f);
	m_FourthSprite->SetRelativePos(m_AttackImageSize * 3.f, 0.f, 0.f);
}

void CBeamMonsterAttack::SetLeftAttackDir()
{
	m_AttackDir = -1.f;

	m_FirstSprite->SetWorldRotationZ(-70.f);
	m_SecondSprite->SetRelativePos(m_AttackImageSize * -1.f, 0.f, 0.f);
	m_ThirdSprite->SetRelativePos(m_AttackImageSize * 2.f * -1.f, 0.f, 0.f);
	m_FourthSprite->SetRelativePos(m_AttackImageSize * 3.f * -1.f, 0.f, 0.f);
}

void CBeamMonsterAttack::Start()
{
	CGameObject::Start();
}

bool CBeamMonsterAttack::Init()
{
	if (!CGameObject::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BeamAttackEffect", TEXT("Ability_Beam_AttackEffect.anim"));

	float AnimDelayTime = AnimationInstance->GetCurrentAnimation()->GetPlayTime()
	/	AnimationInstance->GetCurrentAnimation()->GetFrameCount();

	m_FirstSprite = CreateComponent<CSpriteComponent>("MainSprite");
	m_FirstSprite->SetAnimationInstance(AnimationInstance);
	m_FirstSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);
	m_FirstSprite->SetPivot(0.5f, 0.5f, 0.f);
	CColliderCircle* ColliderCirle = CreateComponent<CColliderCircle>("FirstCollider");
	m_FirstSprite->AddChild(ColliderCirle);
	ColliderCirle->SetCollisionProfile("MonsterAttack");
	ColliderCirle->SetInfo(Vector2(0.f ,0.f), m_FirstSprite->GetWorldScale().x * 0.3f);
	ColliderCirle->AddCollisionCallback(Collision_State::Begin, this, &CBeamMonsterAttack::CollisionCallback);

	m_SecondSprite = CreateComponent<CSpriteComponent>("SecondSprite");
	m_SecondSprite->SetAnimationInstance(AnimationInstance);
	m_SecondSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);
	m_SecondSprite->SetRelativePos(m_AttackImageSize, 0.f, 0.f);
	m_SecondSprite->GetAnimationInstance()->GetCurrentAnimation()->SetInitPauseTime(AnimDelayTime * 1.5f);
	m_SecondSprite->SetInheritRotZ(true);
	m_SecondSprite->SetPivot(0.5f, 0.5f, 0.f);
	ColliderCirle = CreateComponent<CColliderCircle>("FirstCollider");
	m_SecondSprite->AddChild(ColliderCirle);
	ColliderCirle->SetCollisionProfile("MonsterAttack");
	ColliderCirle->SetInfo(Vector2(0.f, 0.f), m_SecondSprite->GetWorldScale().x * 0.3f);
	ColliderCirle->AddCollisionCallback(Collision_State::Begin, this, &CBeamMonsterAttack::CollisionCallback);

	m_ThirdSprite = CreateComponent<CSpriteComponent>("ThirdSprite");
	m_ThirdSprite->SetAnimationInstance(AnimationInstance);
	m_ThirdSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);
	m_ThirdSprite->SetRelativePos(m_AttackImageSize * 2.f, 0.f, 0.f);
	m_ThirdSprite->GetAnimationInstance()->GetCurrentAnimation()->SetInitPauseTime(AnimDelayTime * 2.5f);
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
	m_FourthSprite->GetAnimationInstance()->GetCurrentAnimation()->SetInitPauseTime(AnimDelayTime * 3.5f);
	m_FourthSprite->SetInheritRotZ(true);
	m_FourthSprite->SetPivot(0.5f, 0.5f, 0.f);
	ColliderCirle = CreateComponent<CColliderCircle>("FirstCollider");
	m_FourthSprite->AddChild(ColliderCirle);
	ColliderCirle->SetCollisionProfile("MonsterAttack");
	ColliderCirle->SetInfo(Vector2(0.f, 0.f), m_FourthSprite->GetWorldScale().x * 0.3f);
	ColliderCirle->AddCollisionCallback(Collision_State::Begin, this, &CBeamMonsterAttack::CollisionCallback);


	m_FirstSprite->AddChild(m_SecondSprite);
	m_FirstSprite->AddChild(m_ThirdSprite);
	m_FirstSprite->AddChild(m_FourthSprite);

	m_FirstSprite->SetWorldRotationZ(70.f);

	m_RotateLimit = 70.f * 2;

	return true;
}

void CBeamMonsterAttack::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// 오른쪽 공격
	if (m_AttackDir > 0.f)
	{
		m_FirstSprite->AddRelativeRotationZ(m_RotationSpeed * DeltaTime * -1.f);

		m_RotateLimit += (m_RotationSpeed * DeltaTime * -1.f);
	}
	// 왼쪽 공격
	else
	{
		m_FirstSprite->AddRelativeRotationZ(m_RotationSpeed * DeltaTime);

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
