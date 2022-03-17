#include "BeamSparkEffect.h"
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

CBeamSparkEffect::CBeamSparkEffect()
{
	SetTypeID<CBeamSparkEffect>();
}

CBeamSparkEffect::CBeamSparkEffect(const CBeamSparkEffect& Beatle) : CAttackEffect(Beatle)
{}

CBeamSparkEffect::~CBeamSparkEffect()
{}

void CBeamSparkEffect::SetRightAttackDir()
{
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CBeamSparkEffect::SetLeftAttackDir()
{
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}

void CBeamSparkEffect::Start()
{
	CAttackEffect::Start();

	m_Sprite = (CSpriteComponent*)FindComponent("EffectSprite");
}

bool CBeamSparkEffect::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("EffectSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRenderState("AlphaBlend");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"SparkEffect", TEXT("Kirby_Beam_Effect_UpAttackEffect.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");

	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CBeamSparkEffect::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);
}

void CBeamSparkEffect::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

CBeamSparkEffect* CBeamSparkEffect::Clone()
{
	return new CBeamSparkEffect(*this);
}
