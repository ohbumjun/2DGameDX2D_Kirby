#include "FireUltimate.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Scene/SceneResource.h"
#include "Player2D.h"

CFireUltimate::CFireUltimate()  :
	m_MoveStart(false)
{}

CFireUltimate::~CFireUltimate()
{}

void CFireUltimate::Start()
{
	CKirbyAttackEffect::Start();
}

bool CFireUltimate::Init()
{
	if (!CKirbyAttackEffect::Init())
		return false;

	m_MainSprite->SetWorldScale(100.f, 100.f, 1.f);

	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

	m_AttackDistLimitMax = 2200.f;

	m_AttackObjectSpeed = 1300.f;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"KirbyFireFallAttackEffect", TEXT("Kirby_Fire_Effect_ComeDownFireEffect.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);

	m_MainSprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayTime(0.2f);

	m_SideCollisionApplied = false;

	m_InitHideTime = ((float)rand() / (float)RAND_MAX) * 0.05f;

	m_MainSprite->GetMaterial()->SetOpacity(0.f);

	return true;
}

void CFireUltimate::Update(float DeltaTime)
{
	m_MoveReadyTime += DeltaTime;

	if (m_InitHideTime > 0.f)
	{
		m_InitHideTime -= DeltaTime;

		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

		Player->SetAttackEnable(true);

		return;
	}

	if (m_InitHideTime <= 0.f)
	{
		m_MainSprite->GetMaterial()->SetOpacity(1.f);
	}

	if (m_MoveReadyTime < m_MoveStartTime || m_InitHideTime > 0.f ) 
		return;

	if (!m_MoveStart)
	{
		m_MoveStart = true;

		m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect(true);

		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

		Player->SetAttackEnable(false);
	}

	CKirbyAttackEffect::Update(DeltaTime);
}

void CFireUltimate::PostUpdate(float DeltaTime)
{
	CKirbyAttackEffect::PostUpdate(DeltaTime);
}