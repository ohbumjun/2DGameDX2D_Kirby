#include "UIButton.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/ViewPort.h"

CUIButton::CUIButton() :
m_ClickCallback(nullptr),
m_State(Button_State::Normal)
{}

CUIButton::~CUIButton()
{}

void CUIButton::Enable(bool enable)
{
	CUIWidget::Enable(enable);

	m_State = enable ? Button_State::Normal : Button_State::Disable;
}

void CUIButton::SetTintColor(Button_State State, float r, float g, float b, float a)
{
	m_ButtonStateInfo[(int)State].m_Tint = Vector4(r, g, b, a);
}

void CUIButton::AddFrameData(Button_State State, const Vector2& Start, const Vector2& Size)
{
	AnimationFrameData Frame = {};
	Frame.Start = Start;
	Frame.Size  = Size;
	m_ButtonStateInfo[(int)State].m_FrameData.push_back(Frame);
}

void CUIButton::SetTexture(Button_State State, CTexture* Texture)
{
	m_ButtonStateInfo[(int)State].m_Texture = Texture;
}

void CUIButton::SetTexture(Button_State State, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	CSceneResource* Resource = m_Owner->GetViewPort()->GetScene()->GetResource();
	Resource->LoadTexture(TextureName, FileName, PathName);
	CTexture* Texture = Resource->FindTexture(TextureName);
	if (Texture)
	{
		m_ButtonStateInfo[(int)State].m_Texture = Texture;
	}
}

void CUIButton::SetTextureMultibyte(Button_State State, const std::string& TextureName, const char* FileName, const std::string& PathName)
{
	TCHAR ConvertFileName[MAX_PATH] = {};
	int ConvertLength = MultiByteToWideChar(CP_ACP, -1, FileName, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, FileName, -1, ConvertFileName, ConvertLength);

	return SetTexture(State, TextureName, ConvertFileName, PathName);
}

void CUIButton::SetTextureMultibyteFullPath(Button_State State, const std::string& TextureName, const char* FullPath)
{
	TCHAR ConvertFullPath[MAX_PATH] = {};
	int ConvertLength = MultiByteToWideChar(CP_ACP, -1, FullPath, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, FullPath, -1, ConvertFullPath, ConvertLength);

	return SetTextureFullPath(State, TextureName, ConvertFullPath);
}

void CUIButton::SetTextureFullPath(Button_State State, const std::string& TextureName, const TCHAR* FullPath)
{
	CSceneResource* Resource = m_Owner->GetViewPort()->GetScene()->GetResource();
	Resource->LoadTextureFullPath(TextureName, FullPath);

	CTexture* Texture = Resource->FindTexture(TextureName);
	if (Texture)
	{
		m_ButtonStateInfo[(int)State].m_Texture = Texture;
	}
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

void CUIButton::CallClickCallback()
{
	if (m_ClickCallback)
		m_ClickCallback();
}
