#include "EffectSceneChangeAlpha.h"

#include <Scene/Scene.h>


CEffectSceneChangeAlpha::CEffectSceneChangeAlpha() :
	m_SceneStart(true),
	m_MaintainOpacity(false)
{}

CEffectSceneChangeAlpha::~CEffectSceneChangeAlpha()
{}

void CEffectSceneChangeAlpha::SetBlackTexture()
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "SceneChangeBlackAlpha",
		TEXT("Project/Item/BlackBack.jpg"));

}

void CEffectSceneChangeAlpha::UpdateSceneChangeTime(float DeltaTime)
{
	if (m_MaintainOpacity)
		return;

	// 서서히 Opacity를 증가시킨다.
	float NewOpacity = -1.f;
	bool End = false;

	// 서서히 Opacity를 감소시킨다.
	if (m_SceneStart)
	{
		float CurrentOpacity = m_Sprite->GetMaterial()->GetOpacity();

		NewOpacity = m_Sprite->GetMaterial()->GetOpacity() - DeltaTime * 0.4f;

		if (NewOpacity <= 0.1f)
			End = true;
	}
	else
	{
		NewOpacity = m_Sprite->GetMaterial()->GetOpacity() + DeltaTime * 1.4f;

		if (NewOpacity >= 0.99f)
			End = true;
	}

	m_Sprite->SetOpacity(NewOpacity);

	if (End)
	{
		if (m_SceneChangeCallback)
			m_SceneChangeCallback();

		Destroy();
	}
}

void CEffectSceneChangeAlpha::Start()
{
	CGameObject::Start();

	if (m_SceneStart)
		m_Sprite->SetOpacity(1.0f);
	else
		m_Sprite->SetOpacity(0.0f);
}

bool CEffectSceneChangeAlpha::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("SceneChangeAlpha");

	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "SceneChangeAlpha",
		TEXT("Project/Item/WhiteBack.png"));

	m_Sprite->SetLayerName("SceneChange");

	m_Sprite->SetTransparency(true);

	SetWorldScale(m_Scene->GetWorldResolution().x, m_Scene->GetWorldResolution().y, 1.f);

	m_Sprite->SetOpacity(1.0f);

	return true;
}

void CEffectSceneChangeAlpha::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	UpdateSceneChangeTime(DeltaTime);
}
