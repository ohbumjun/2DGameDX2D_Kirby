#include "UIButton.h"
#include "../Scene/Scene.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/ViewPort.h"

CUIButton::CUIButton() :
	m_State(Button_State::Normal)
{}

CUIButton::~CUIButton()
{}

void CUIButton::AddFrameData(Button_State State, const Vector2& Start, const Vector2& Size)
{
	AnimationFrameData Frame = {};
	Frame.Start = Start;
	Frame.Size = Size;

	m_ButtonStateInfo[(int)State].m_vecAnimFrameData.push_back(Frame);
}

void CUIButton::SetTintColor(Button_State State, const Vector4& Color)
{
	m_ButtonStateInfo[(int)State].m_Tint = Color;
}

void CUIButton::SetTintColor(Button_State State, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{

	m_ButtonStateInfo[(int)State].m_Tint = Vector4((float)(r % 255), (float)(g % 255), (float)(b % 255), (float)(a % 255));
}

void CUIButton::SetTexture(Button_State State, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Scene->GetResource()->LoadTexture(Name, FileName, PathName);

	CTexture* Texture = m_Scene->GetResource()->FindTexture(Name);

	m_ButtonStateInfo[(int)State].m_Texture = Texture;
}

void CUIButton::SetTextureFullPath(Button_State State, const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetResource()->LoadTexture(Name, FullPath);
	CTexture* Texture = m_Scene->GetResource()->FindTexture(Name);
	m_ButtonStateInfo[(int)State].m_Texture = Texture;
}

void CUIButton::SetTexture(Button_State State, CTexture* Texture)
{
	m_ButtonStateInfo[(int)State].m_Texture = Texture;
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
	CUIWidget::Render();
}

void CUIButton::CallClickCallbackFunction()
{
	if (m_ClickCallbackFunction)
		m_ClickCallbackFunction();
}
