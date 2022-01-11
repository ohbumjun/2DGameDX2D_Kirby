#include "UIButton.h"
#include "../PathManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"

CUIButton::CUIButton() :
	m_State(Button_State::Normal)
{
	SetTypeID<CUIButton>();
}

CUIButton::CUIButton(const CUIButton& Button) : CUIWidget(Button)
{
	m_State = Button_State::Normal;
}

CUIButton::~CUIButton()
{}

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
	if (!CSceneManager::GetInst()->GetScene()->GetResource()->LoadTextureFullPath(Name, FullPath))
		return false;
	CTexture* Texture = CSceneManager::GetInst()->GetScene()->GetResource()->FindTexture(Name);
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
	m_Info[(int)State].vecFrameData.push_back(FrameData);
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
