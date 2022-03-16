#include "EffectKirbyRide.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"	
#include "../Scene/Green3Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/CameraManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Engine.h"
#include "Component/TileEmptyComponent.h"

CEffectKirbyRide::CEffectKirbyRide()  :
	m_YToggleDir(1.f) ,
	m_GetOutOfOriginalWorldTime(2.f),
	m_SceneChangeLimitTime(7.f)
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

	// TileMap�� ������ �ʰ� �����Ѵ�.
	m_Scene->GetTileEmptyComponent()->Enable(false);
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

	// Clone �ؼ� ��������� �Ѵ�.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	SetWorldScale(GetWorldScale().x * 1.2f, GetWorldScale().y * 1.2f, 1.f);

	// Camera Component
	m_Camera = CreateComponent<CCameraComponent>("DefaultCamera");

	m_Camera->OnViewportCenter();

	m_Camera->SetAdjustRatio(false);

	m_Sprite->AddChild(m_Camera);

	// Resolution
	m_RS = CEngine::GetInst()->GetResolution();

	return true;
}

void CEffectKirbyRide::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_GetOutOfOriginalWorldTime > 0.f)
	{
		m_GetOutOfOriginalWorldTime -= DeltaTime;

		AddWorldPos(Vector3(1.f, 0.f, 0.f) * DeltaTime * 500.f);
	}
	else
	{
		AddWorldPos(Vector3(1.f, m_YToggleDir, 0.f) * DeltaTime * 500.f);

		if (GetWorldPos().y - (float)m_RS.Height * 0.5f <= 0.f)
		{
			m_YToggleDir = 1.f;
		}
		else if (GetWorldPos().y + (float)m_RS.Height * 0.2f >= (float)m_RS.Height)
		{
			m_YToggleDir = 1.f * -1.f;
		}
	}


	// ���� �پ��� �Ѵ�.
	SetWorldScale(GetWorldScale().x * 0.9998f, GetWorldScale().y * 0.9998f, 1.f);

	// ���� ī�޶� �ڽ� Component ���
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

	// ���� �ð��� ���� �Ŀ� Scene�� Change �Ѵ�.
	if (m_SceneChangeLimitTime > 0.f)
	{
		m_SceneChangeLimitTime -= DeltaTime;

		if (m_SceneChangeLimitTime <= 0.f)
		{
			ChangeSceneToGreen3Scene();
		}
	}
}

CEffectKirbyRide* CEffectKirbyRide::Clone()
{
	return new CEffectKirbyRide(*this);
}

void CEffectKirbyRide::ChangeSceneToGreen3Scene()
{
	CSceneManager::GetInst()->CreateNewScene(false);
	CSceneManager::GetInst()->CreateSceneModeEmpty<CGreen3Scene>(false);
	CSceneManager::GetInst()->GetNextScene()->PrepareResources();

	if (CSceneManager::GetInst()->GetNextScene()->Load("Green3.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
}
