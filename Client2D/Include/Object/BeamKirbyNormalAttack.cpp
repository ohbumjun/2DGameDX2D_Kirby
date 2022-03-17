#include "BeamKirbyNormalAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "FireMonster.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CBeamKirbyNormalAttack::CBeamKirbyNormalAttack() :
	m_AttackDir(1.f),
	m_AttackDistLimit(0.f),
	m_AttackDistLimitMax(1000.f)
{}

CBeamKirbyNormalAttack::CBeamKirbyNormalAttack(const CBeamKirbyNormalAttack& Attack) : CGameObject(Attack)
{}

CBeamKirbyNormalAttack::~CBeamKirbyNormalAttack()
{}

void CBeamKirbyNormalAttack::SetRightAttackDir(float YDir)
{
	m_AttackDir = 1.f;
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CBeamKirbyNormalAttack::SetLeftAttackDir(float YDir)
{
	m_AttackDir = -1.f;
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}

void CBeamKirbyNormalAttack::Start()
{
	CGameObject::Start();
}

bool CBeamKirbyNormalAttack::Init()
{
	if (!CGameObject::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BeamAttackEffect", TEXT("Kirby_Beam_Effect_NormalAttack.anim"));

	float AnimDelayTime = AnimationInstance->GetCurrentAnimation()->GetPlayTime()
		/ AnimationInstance->GetCurrentAnimation()->GetFrameCount();

	m_Sprite = CreateComponent<CSpriteComponent>("MainSprite");
	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->SetWorldScale(120.f, 120.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	CColliderCircle* ColliderCirle = CreateComponent<CColliderCircle>("FirstCollider");
	m_Sprite->AddChild(ColliderCirle);
	ColliderCirle->SetCollisionProfile("PlayerAttack");
	ColliderCirle->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.4f);
	ColliderCirle->AddCollisionCallback(Collision_State::Begin, this, &CBeamKirbyNormalAttack::CollisionCallback);

	return true;
}

void CBeamKirbyNormalAttack::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	float MoveDist = m_AttackDir * DeltaTime * 500.f;

	AddWorldPos(Vector3(m_AttackDir, 0.f, 0.f) * DeltaTime * 500.f);

	if (m_AttackDistLimit < m_AttackDistLimitMax)
	{
		m_AttackDistLimit += MoveDist;

		if (m_AttackDistLimit >= m_AttackDistLimitMax)
		{
			Destroy();
		}
	}
}

void CBeamKirbyNormalAttack::CollisionCallback(const CollisionResult& Result)
{
	Destroy();

	CColliderComponent* CollisionDest = Result.Dest;
	CGameObject* Owner = CollisionDest->GetGameObject();
	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner)
	{
		// HP Bar �ް� �ϱ�
		if (Owner->GetTypeID() == typeid(CMonster).hash_code())
		{
			CMonster* DestMonster = (CMonster*)Owner;
			// DestMonster->Damage(2.f);
		}

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();
		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");
		}
	}
}
