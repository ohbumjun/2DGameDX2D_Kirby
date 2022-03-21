#include "EffectStar.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/CameraManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/ColliderCircle.h"
#include "EffectKirbyRide.h"
#include "Engine.h"
#include "BulletCamera.h"
#include "Monster.h"
#include "UI/UIDamageFont.h"
#include "Component/WidgetComponent.h"

CEffectStar::CEffectStar() :
	m_IsRotate(false),
	m_IsSpecialKirbyStar(false)
{
	SetTypeID<CEffectStar>();
}

CEffectStar::CEffectStar(const CEffectStar& Beatle) : CAttackEffect(Beatle)
{}

CEffectStar::~CEffectStar()
{}

void CEffectStar::BottomCollisionSpecificAction()
{
	if (m_IsSpecialKirbyStar)
	{
		m_Jump = true;
		m_FallTime = 0.f;
		m_FallStartY = GetWorldPos().y;
	}
}

void CEffectStar::SideCollisionSpecificAction()
{
	if (m_IsSpecialKirbyStar)
	{
		m_Jump = true;
		m_FallTime = 0.f;
		m_FallStartY = GetWorldPos().y;
	}
}

void CEffectStar::Start()
{
	CAttackEffect::Start();

	m_Collider->SetCollisionProfile("PlayerAttack");
}

bool CEffectStar::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"CSceneChangeStarEffect", TEXT("Effect_SceneChangeStar.anim"));

	// Clone �ؼ� ��������� �Ѵ�.
	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");

	SetWorldScale(GetWorldScale().x * 0.8f, GetWorldScale().y * 0.8f, 1.f);
	
	m_Collider->SetCollisionProfile("PlayerAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, this, &CEffectStar::StarCollision);

	m_BottomCollisionApplied = true;

	return true;
}

void CEffectStar::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	// �Ϲ� Monster�� ������ Star ���
	if (!m_IsSpecialKirbyStar)
	{
		AddWorldPos(Vector3(m_AttackDir.x, m_AttackDir.y, 0.f) * DeltaTime * m_EffectMoveSpeed);
	}
	// �װ� �ƴ϶�, Ability Monster �� ������ Star ���
	else
	{
		AddWorldPos(Vector3(m_AttackDir.x, 0.f, 0.f) * DeltaTime * m_EffectMoveSpeed * 0.1f);
	}

	if (m_IsRotate)
	{
		AddRelativeRotationZ(360.f * DeltaTime);
	}
}

void CEffectStar::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

CEffectStar* CEffectStar::Clone()
{
	return new CEffectStar(*this);
}

void CEffectStar::StarCollision(const CollisionResult& Result)
{
	Destroy();

	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CLifeObject* AttackSourceObject = (CLifeObject*)Result.Src->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner)
	{
		CMonster* DestMonster = dynamic_cast<CMonster*>(Owner);

		if (!DestMonster)
			return;

		// HP Bar �ް� �ϱ�
		DestMonster->Damage(AttackSourceObject->GetAttackAbility());

		DestMonster->SetBeingHit(true);

		DestMonster->SetAIState(Monster_AI::Hit);

		if (m_AttackDir.x > 0)
			DestMonster->SetObjectMoveDir(Vector3(-1.f, 0.f, 0.f));
		else
			DestMonster->SetObjectMoveDir(Vector3(1.f, 0.f, 0.f));

		// DestMonster->Damage(2.f);

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");
			DamageFont->SetDamage((int)AttackSourceObject->GetAttackAbility());
		}
	}
}
