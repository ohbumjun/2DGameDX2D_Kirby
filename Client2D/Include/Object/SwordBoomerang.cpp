#include "SwordBoomerang.h"
#include "Player2D.h"
#include "GameObject/LifeObject.h"
#include "Component/SpriteComponent.h"
#include "Scene/Scene.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"
#include "Scene/SceneResource.h"
#include "Scene/CameraManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "FireMonster.h"
#include "Block.h"
#include "FireAttackBackEffect.h"

CSwordBoomerang::CSwordBoomerang() :
	m_ThrowDirDistMax(1000.f),
	m_Return(false),
	m_SpeedAccel(0.8f),
	m_InitSpeed(1200.f),
	m_AttackDir(1.f)
{
}

CSwordBoomerang::~CSwordBoomerang()
{}

void CSwordBoomerang::SetAttackDamage(float Damage)
{
	m_AttackDamage = Damage;
}

void CSwordBoomerang::SetAttackDir(float DirX)
{
	m_AttackDir = DirX;
}

void CSwordBoomerang::UpdateBoomerangMovement(float DeltaTime)
{}

void CSwordBoomerang::Start()
{
	CGameObject::Start();
}

bool CSwordBoomerang::Init()
{
	if (!CGameObject::Init())
		return false;

	m_MainSprite = CreateComponent<CSpriteComponent>("MainSprite");

	m_MainSprite->SetRenderState("AlphaBlend");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"KirbySwordBoomerang", TEXT("Kirby_Cutter_Boomerang.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);

	m_MainSprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayTime(0.1f);

	m_MainSprite->SetWorldScale(250.f, 250.f, 1.f);

	m_MainSprite->SetPivot(0.5f, 0.5f, 0.f);

	m_Collider = CreateComponent<CColliderCircle>("Collider");

	m_Collider->SetCollisionProfile("PlayerAttack");

	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.5f);

	m_Collider->AddCollisionCallback(Collision_State::Begin, this, &CSwordBoomerang::CollisionCallback);

	m_MainSprite->AddChild(m_Collider);

	m_CurrentSpeed = m_InitSpeed;

	return true;
}

void CSwordBoomerang::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (!m_Return)
	{
		m_CurrentSpeed -= m_SpeedAccel;

		float MoveDist = (Vector3(m_AttackDir, 0.f, 0.f) * DeltaTime * m_CurrentSpeed).Length();

		AddWorldPos(Vector3(m_AttackDir, 0.f, 0.f) * DeltaTime * m_CurrentSpeed);

		m_ThrowDirDist += MoveDist;

		if (m_ThrowDirDist >= m_ThrowDirDistMax || m_CurrentSpeed <= 0.f)
		{
			m_ThrowDirDist = 0.f;

			m_Return = true;
		}
	}
	else
	{
		m_CurrentSpeed += m_SpeedAccel;

		float MoveDist = (Vector3(m_AttackDir, 0.f, 0.f) * DeltaTime * m_CurrentSpeed).Length();

		AddWorldPos(Vector3(m_AttackDir * -1.f, 0.f, 0.f) * DeltaTime * m_CurrentSpeed);

		m_ThrowDirDist += MoveDist;

		if (m_ThrowDirDist >= m_ThrowDirDistMax)
		{
			Destroy();
		}
	}
}


void CSwordBoomerang::CollisionCallback(const CollisionResult& Result)
{
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

		if (DestMonster->IsBeingHit())
			return;

		// HP Bar 달게 하기
		DestMonster->Damage(m_AttackDamage);

		DestMonster->SetBeingHit(true);

		DestMonster->SetAIState(Monster_AI::Hit);

		if (m_AttackDir < 0)
			DestMonster->SetObjectMoveDir(Vector3(-1.f, 0.f, 0.f));
		else
			DestMonster->SetObjectMoveDir(Vector3(1.f, 0.f, 0.f));

		// DestMonster->Damage(2.f);

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");

			DamageFont->SetDamage((int)m_AttackDamage);
		}
	}
}
