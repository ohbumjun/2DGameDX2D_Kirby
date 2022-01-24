#include "UINumberWidget.h"

#include "UIWindow.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/ViewPort.h"

CUINumberWidget::CUINumberWidget() :
	m_vecNumbers{},
	m_Number(0)
{
	m_vecNumbers.reserve(10);
}

CUINumberWidget::CUINumberWidget(const CUINumberWidget& Widget) :
CUIWidget(Widget)
{
	m_Number = Widget.m_Number;
}

CUINumberWidget::~CUINumberWidget()
{}

void CUINumberWidget::SetTexture(const std::string& Name, 
	const TCHAR* FileName, const std::string& PathName)
{
	if (m_Owner->GetViewPort())
	{
		m_Owner->GetViewPort()->GetScene()->GetResource()->LoadTexture(Name, FileName, PathName);
		m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetResource()->FindTexture(Name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);
		m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	}
	if (m_Info.m_Texture)
	{
		float Width = (float)m_Info.m_Texture->GetWidth();
		float Height = (float)m_Info.m_Texture->GetHeight();

		SetSize(Width, Height);
		SetUseTexture(true);
	}
}

void CUINumberWidget::SetTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
								 const std::string& PathName)
{
	if (m_Owner->GetViewPort())
	{
		m_Owner->GetViewPort()->GetScene()->GetResource()->LoadTexture(Name, vecFileName);
		m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetResource()->FindTexture(Name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, vecFileName);
		m_Info.m_Texture = CResourceManager::GetInst() ->FindTexture(Name);
	}
	if (m_Info.m_Texture)
	{
		float Width = (float)m_Info.m_Texture->GetWidth();
		float Height = (float)m_Info.m_Texture->GetHeight();

		SetSize(Width, Height);
		SetUseTexture(true);
	}
}

void CUINumberWidget::AddAnimationFrameData(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData Frame = {};
	Frame.Start = StartPos;
	Frame.Size = Size;
	m_Info.m_vecFrameData.push_back(Frame);
}

void CUINumberWidget::AddAnimationFrameData(int Count)
{
	for (int i = 0; i < Count; i++)
	{
		AnimationFrameData Frame = {};
		Frame.Start = Vector2(0.f, 0.f);
		Frame.Size = Vector2((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());

		m_Info.m_vecFrameData.push_back(Frame);
	}
}

void CUINumberWidget::SetTextureTint(const Vector4& Color)
{
	m_Info.m_Tint = Color;
}

void CUINumberWidget::SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_Info.m_Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

bool CUINumberWidget::Init()
{
	if (!CUIWidget::Init())
		return false;

	if (m_Owner->GetViewPort())
		m_Shader = m_Owner->GetViewPort()->GetScene()->GetResource()->FindShader("NumberShader");
	else
		m_Shader = CResourceManager::GetInst()->FindShader("NumberShader");

	return true;
}

void CUINumberWidget::Start()
{
	CUIWidget::Start();
}

void CUINumberWidget::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);
}

void CUINumberWidget::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

void CUINumberWidget::Render()
{
	// 숫자 계산하기
	std::stack<int> stack;

	int TempNum = m_Number;

	m_vecNumbers.clear();

	if (m_Number == 0)
		m_vecNumbers.push_back(0);
	else
	{
		while (TempNum > 0)
		{
			stack.push(TempNum % 10);
			TempNum /= 10;
		}
		while (!stack.empty())
		{
			m_vecNumbers.push_back(stack.top());
			stack.pop();
		}
	}

	int Size = (int)m_vecNumbers.size();

	int Frame = 0;
	Vector2 StartUV = { 0.f, 0.f }, EndUV = { 1.f, 1.f };

	for (int i = 0; i < Size; i++)
	{
		Frame = 0;

		if (m_Info.m_Texture)
		{
			float TextureWidth = (float)m_Info.m_Texture->GetWidth();
			float TextureHeight = (float)m_Info.m_Texture->GetHeight();

			switch (m_Info.m_Texture->GetImageType())
			{
			case Image_Type::Atlas :
				{
					float TextureWidth  = (float)m_Info.m_Texture->GetWidth();
					float TextureHeight = (float)m_Info.m_Texture->GetHeight();

					AnimationFrameData FrameData = m_Info.m_vecFrameData[m_vecNumbers[i]];
					StartUV = FrameData.Start / Vector2(TextureWidth, TextureHeight);
					EndUV   = (FrameData.Start + FrameData.Size) / Vector2(TextureWidth, TextureHeight);

					m_CBuffer->SetStartUV(StartUV);
					m_CBuffer->SetEndUV(EndUV);

				}
				break;
			case Image_Type::Frame :
				{
					AnimationFrameData FrameData = m_Info.m_vecFrameData[m_vecNumbers[i]];
					StartUV = FrameData.Start / Vector2(TextureWidth, TextureHeight);
					EndUV = (FrameData.Start + FrameData.Size) / Vector2(TextureWidth, TextureHeight);

					m_CBuffer->SetStartUV(StartUV);
					m_CBuffer->SetEndUV(EndUV);

					Frame = m_vecNumbers[i];
				}
				break;
			}

			m_Info.m_Texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, Frame);

			m_CBuffer->SetAnimType(m_Info.m_Texture->GetImageType());
		}

		m_Tint = m_Info.m_Tint;

		// Render Pos 세팅하기
		m_RenderPos.x += (m_Size.x * i);

		CUIWidget::Render();
	}
}

CUINumberWidget* CUINumberWidget::Clone()
{
	return new CUINumberWidget(*this);
}
