#include "EffectKirbyRide.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/CameraManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Engine.h"

CEffectKirbyRide::CEffectKirbyRide() 
{
	SetTypeID<CEffectKirbyRide>();
}

CEffectKirbyRide::CEffectKirbyRide(const CEffectKirbyRide& Beatle)
{}

CEffectKirbyRide::~CEffectKirbyRide()
{}

void CEffectKirbyRide::Start()
{
	CGameObject::Start();
}

bool CEffectKirbyRide::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("EffectSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRenderState("AlphaBlend");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"KirbyRideEffect", TEXT("Effect_KirbyRide.anim"));

	if (!AnimationInstance)
	{
		assert(false);
		return false;
	}

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	SetWorldScale(GetWorldScale().x * 1.2f, GetWorldScale().y * 1.2f, 1.f);

	return true;
}

void CEffectKirbyRide::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	AddWorldPos(Vector3(-1.f, 0.f, 0.f) * DeltaTime * 50.f);

	SetWorldScale(GetWorldScale().x * 0.9999f, GetWorldScale().y * 0.9999f, 1.f);

	// 현재 카메라가 자식 Component 라면
	if (m_Sprite->FindChild(m_Scene->GetCameraManager()->GetCurrentCamera()))
	{
		Resolution RS = CEngine::GetInst()->GetResolution();

		m_Scene->GetCameraManager()->GetCurrentCamera()->SetWorldPos(Vector3(
			GetWorldPos().x - (float)RS.Width * 0.5f,
			GetWorldPos().y - (float)RS.Height * 0.5f,
				GetWorldPos().z));
	}
}

void CEffectKirbyRide::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectKirbyRide* CEffectKirbyRide::Clone()
{
	return new CEffectKirbyRide(*this);
}
