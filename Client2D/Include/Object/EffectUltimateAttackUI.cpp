#include "EffectUltimateAttackUI.h"
#include <Scene/Scene.h>
#include "Scene/CameraManager.h"
#include "Engine.h"
#include "Component/SpriteComponent.h"

CEffectUltimateAttackUI::CEffectUltimateAttackUI() 
{}

CEffectUltimateAttackUI::~CEffectUltimateAttackUI()
{}

void CEffectUltimateAttackUI::SetUITexture(const TCHAR* FileName)
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, 
		"UltimateNewUI",
		FileName);

	m_Sprite->SetTransparency(true);
}

void CEffectUltimateAttackUI::UpdateSceneChangeTime(float DeltaTime)
{
	m_UIProceedTime += DeltaTime;

	float SpeedPerSec = (float)m_RS.Width / m_UIProceedTimeMax;

	if (m_UIProceedTime <= m_UIProceedTimeMax * 0.1f || 
		m_UIProceedTime > m_UIProceedTimeMax * 0.9f)
	{
		// 0.1 초 동안, 30% 정도를 가야 한다.

		AddWorldPos(Vector3(-1.f, 0.f, 0.f) * DeltaTime * SpeedPerSec * 3.f);
	}
	else if (m_UIProceedTimeMax * 0.1f < m_UIProceedTime && 
		m_UIProceedTime <= m_UIProceedTimeMax * 0.9f)
	{
		// 반대로 0.8초 동안, 30% 정도를 가야 한다.
		// 즉, 0.1초에 3.8xx% 정도를 가야 한다.

		AddWorldPos(Vector3(-1.f, 0.f, 0.f) * DeltaTime * SpeedPerSec * 0.38f);
	}
}

void CEffectUltimateAttackUI::Start()
{
	CGameObject::Start();
	
}

bool CEffectUltimateAttackUI::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("UltimateAttackUI");

	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "UltimateAttackUI",
		TEXT("Project/Item/WhiteBack.png"));

	m_Sprite->SetLayerName("UltimateAttackUI");

	m_Sprite->SetTransparency(true);

	// SetWorldScale(m_Scene->GetWorldResolution().x * 0.3f, m_Scene->GetWorldResolution().y * 0.3f, 1.f);
	SetWorldScale(800.f, 720.f, 1.f);

	Resolution RS = CEngine::GetInst()->GetResolution();

	m_RS = RS;

	SetWorldPos((float)RS.Width,
		m_Scene->GetCameraManager()->GetCurrentCamera()->GetWorldPos().y, 0.f);

	return true;
}

void CEffectUltimateAttackUI::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	UpdateSceneChangeTime(DeltaTime);
}
