#include "EffectSceneChangeAlpha.h"

#include <Scene/Scene.h>


CEffectSceneChangeAlpha::CEffectSceneChangeAlpha() :
	m_SceneStart(true),
	m_MaintainOpacity(false),
	m_StartOpacityDecrease(false),
	m_ApplyDecreaseDestroy(false)
{}

CEffectSceneChangeAlpha::~CEffectSceneChangeAlpha()
{}

void CEffectSceneChangeAlpha::SetBlackTexture()
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "SceneChangeBlackAlpha",
		TEXT("Project/Item/BlackBack.jpg"));
}

void CEffectSceneChangeAlpha::SetUltimateAttackTexture(Ability_State State)
{
	switch(State)
	{
	case Ability_State::Beam :
		{
		m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "UltimateBlueAlpha",
			TEXT("Project/Item/UltimateBlue.jpg"));
		}
		break;
	case Ability_State::Bomb:
		{
		m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "UltimatePurpleAlpha",
			TEXT("Project/Item/UltimatePurple.jpg"));
		}
		break;
	case Ability_State::Fire:
		{
		m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "UltimateOrangeAlpha",
			TEXT("Project/Item/UltimateOrange.jpg"));
		}
		break;
	case Ability_State::Fight:
		{
		m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "UltimateRedAlpha",
			TEXT("Project/Item/UltimateRed.jpg"));
		}
		break;
	case Ability_State::Sword:
		{
		m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "UltimateYellowAlpha",
			TEXT("Project/Item/UltimateYellow.jpg"));
		}
		break;
	}
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

void CEffectSceneChangeAlpha::UpdateOpacityDestroy(float DeltaTime)
{
	if (!m_ApplyDecreaseDestroy)
		return;

	// Start Destroy
	if (!m_StartOpacityDecrease)
	{
		m_StartDestroyCurTime += DeltaTime;

		if (m_StartDestroyCurTime >= m_StartDestroyTime)
		{
			m_StartOpacityDecrease = true;
		}
	}
	else 
	{
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
			Destroy();
		}
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

	UpdateOpacityDestroy(DeltaTime);
}
