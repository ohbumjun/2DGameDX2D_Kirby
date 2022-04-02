#include "BeamUltimate.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

 CBeamUltimate::CBeamUltimate() :
	 m_ShowEnable(true)
{}

 CBeamUltimate::~CBeamUltimate()
{}

 void CBeamUltimate::Start()
{
	CKirbyAttackEffect::Start();
}

 bool CBeamUltimate::Init()
{
	 if (!CKirbyAttackEffect::Init())
		 return false;

	 m_MainSprite->SetWorldScale(100.f, 100.f, 1.f);
	 m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

	 m_AttackDistLimitMax = 4100.f;
	 m_AttackObjectSpeed = 1000.f;

	 CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		 "KirbyBeamFallAttackEffect", TEXT("Kirby_Beam_Effect_UpAttackEffect.anim"));

	 m_MainSprite->SetAnimationInstance(AnimationInstance);

	 m_MainSprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayScale(3.f);

	 m_SideCollisionApplied = false;

	 m_InitStopTime = ((float)rand() / (float)RAND_MAX) * 0.05f;

	 m_EnableToggleTime = ((float)rand() / (float)RAND_MAX) * 0.05f;

	 return true;
}

 void CBeamUltimate::Update(float DeltaTime)
{
	 if (m_InitStopTime > 0.f)
	 {
		 Enable(true);

		 m_InitStopTime -= DeltaTime;

		 return;
	 }

	CKirbyAttackEffect::Update(DeltaTime);

	m_EnableCurTime += DeltaTime;

	if (m_EnableCurTime >= m_EnableToggleTime)
	{
		if (m_ShowEnable)
		{
			m_ShowEnable = false;

			m_MainSprite->GetMaterial()->SetOpacity(0.f);
		}
		else
		{
			m_ShowEnable = true;

			m_MainSprite->GetMaterial()->SetOpacity(1.f);
		}

		m_EnableCurTime = 0.f;
	}
}

 void CBeamUltimate::PostUpdate(float DeltaTime)
{
	CKirbyAttackEffect::PostUpdate(DeltaTime);
}