#include "EffectKirbyRide.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/CameraManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Engine.h"

CEffectKirbyRide::CEffectKirbyRide()  :
	m_YToggleDir(1.f)
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

	m_Scene->GetCameraManager()->KeepCamera();
	m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
	// m_Scene->GetCameraManager()->ReturnCamera();
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

	// Camera Component
	m_Camera = CreateComponent<CCameraComponent>("DefaultCamera");

	m_Camera->OnViewportCenter();

	m_Sprite->AddChild(m_Camera);

	// Resolution
	m_RS = CEngine::GetInst()->GetResolution();

	return true;
}

void CEffectKirbyRide::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	AddWorldPos(Vector3(1.f, m_YToggleDir, 0.f) * DeltaTime * 400.f);

	if (GetWorldPos().y - (float)m_RS.Height * 0.4f < 0.f)
	{
		m_YToggleDir = 1.f;
	}
	else if (GetWorldPos().y + (float)m_RS.Height * 0.4f >= (float)m_RS.Height)
	{
		m_YToggleDir = 1.f * -1.f;
	}


	// 점점 줄어들게 한다.
	SetWorldScale(GetWorldScale().x * 0.9995f, GetWorldScale().y * 0.9995f, 1.f);

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
