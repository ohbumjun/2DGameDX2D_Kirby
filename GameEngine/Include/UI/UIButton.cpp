#include "UIButton.h"
#include "../PathManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "UIWindow.h"
#include "../Scene/ViewPort.h"

CUIButton::CUIButton() :
	m_State(Button_State::Normal),
	m_ButtonSounds{},
	m_HoverSound(false),
	m_ClickSound(false),
	m_Info { }
{
	SetTypeID<CUIButton>();//
}

CUIButton::CUIButton(const CUIButton& Button) : CUIWidget(Button)
{
	m_State = Button_State::Normal;
	m_ClickCallback = nullptr;
	m_HoverSound = false;;
	m_ClickSound = false;;
}

CUIButton::~CUIButton()
{}

void CUIButton::SetMouseHoverSound(CSound* Sound)
{
	m_ButtonSounds[(int)Button_SoundState::BtnHover] = Sound;
}

void CUIButton::SetMouseClickSound(CSound* Sound)
{
	m_ButtonSounds[(int)Button_SoundState::BtnClick] = Sound;
}

void CUIButton::SetMouseSound(Button_SoundState State, const std::string& GroupName, const std::string& SoundName, 
	bool Loop, const TCHAR* FileName,
	const std::string& PathName)
{
	CSound* Sound = nullptr;

	// 특정 Scene에 속한 것 
	if (m_Owner->GetViewPort())
	{
		m_Owner->GetViewPort()->GetScene()->GetResource()->LoadSound(GroupName, SoundName,
			Loop, FileName, PathName);
		Sound = m_Owner->GetViewPort()->GetScene()->GetResource()->FindSound(SoundName);
	}
	else
	{
		CResourceManager::GetInst()->LoadSound(GroupName, SoundName,
			Loop, FileName, PathName);
		Sound = CResourceManager::GetInst()->FindSound(SoundName);
	}

	if (!Sound)
		return;

	m_ButtonSounds[(int)State] = Sound;
}

void CUIButton::SetMouseSound(Button_SoundState State, const std::string& SoundName)
{
	CSound* Sound = nullptr;

	if (m_Owner->GetViewPort())
	{
		Sound = m_Owner->GetViewPort()->GetScene()->GetResource()->FindSound(SoundName);
	}
	else
	{
		Sound = CResourceManager::GetInst()->FindSound(SoundName);
	}

	if (Sound)
		m_ButtonSounds[(int)State] = Sound;
}

void CUIButton::SetMouseSound(Button_SoundState State, CSound* Sound)
{
	m_ButtonSounds[(int)State] = Sound;
}

void CUIButton::SetTextureTint(Button_State State, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_Info[(int)State].m_Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

bool CUIButton::SetTexture(Button_State State, CTexture* Texture)
{
	m_Info[(int)State].m_Texture = Texture;
	SetUseTexture(true);

	return true;
}

bool CUIButton::SetTexture(Button_State State, const std::string& Name, const TCHAR* FileName,
						   const std::string& PathName)
{
	TCHAR FullPath[MAX_PATH] = {};

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);
	if (Path)
		lstrcpy(FullPath, Path->Path);
	lstrcat(FullPath, FileName);

	return SetTextureFullPath(State, Name, FullPath);
}

bool CUIButton::SetTextureFullPath(Button_State State, const std::string& Name, const TCHAR* FullPath)
{
	CTexture* Texture = nullptr;

	if (m_Owner->GetViewPort())
	{
		if (!m_Owner->GetViewPort()->GetScene()->GetResource()->LoadTextureFullPath(Name, FullPath))
			return false;
		Texture = m_Owner->GetViewPort()->GetScene()->GetResource()->FindTexture(Name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
			return false;
		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	if (Texture)
	{
		m_Info[(int)State].m_Texture = Texture;
		SetUseTexture(true);
	}

	return true;
}

void CUIButton::SetButtonEnable(bool Enable)
{
	m_State = Enable ? Button_State::Normal : Button_State::Disable;
}

void CUIButton::AddFrameData(Button_State State, const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData FrameData = {};
	FrameData.Start = StartPos;
	FrameData.Size = StartPos + Size;
	m_Info[(int)State].m_vecFrameData.push_back(FrameData);
}

bool CUIButton::Init()
{
	if (!CUIWidget::Init())
		return false;

	return true;
}

void CUIButton::Start()
{
	CUIWidget::Start();
}

void CUIButton::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);

	bool LMouseClick = CInput::GetInst()->IsLMouseClicked();

	if (m_State != Button_State::Disable)
	{
		if (m_MouseHovered)
		{
			if (!m_HoverSound)
			{
				if (m_ButtonSounds[(int)Button_SoundState::BtnHover])
					m_ButtonSounds[(int)Button_SoundState::BtnHover]->Play();
				m_HoverSound = true;
			}
			if (LMouseClick)
			{
				m_State = Button_State::Click;
				if (!m_ClickSound)
				{
					if (m_ButtonSounds[(int)Button_SoundState::BtnClick])
						m_ButtonSounds[(int)Button_SoundState::BtnClick]->Play();
					m_ClickSound = true;
				}
			}
			else if (m_State == Button_State::Click)
			{
				if (m_ClickCallback)
					m_ClickCallback();
				m_State = Button_State::MouseOn;
				m_ClickSound = false;

			}
			else
			{
				m_State = Button_State::MouseOn;
				m_ClickSound = false;
			}
		}
		else
		{
			m_State = Button_State::Normal;
			m_ClickSound = false;
			m_HoverSound = false;
		}
	}
}

void CUIButton::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

void CUIButton::Render()
{
	if (m_Info[(int)m_State].m_Texture)
		m_Info[(int)m_State].m_Texture->SetShader(0, (int)ConstantBuffer_Shader_Type::Pixel, 0);

	m_Tint = m_Info[(int)m_State].m_Tint;

	CUIWidget::Render();
}

CUIButton* CUIButton::Clone()
{
	return new CUIButton(*this);
}
