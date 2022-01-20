#include "UIDamageFont.h"

#include "UIWindow.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/ViewPort.h"

CUIDamageFont::CUIDamageFont() :
	m_LifeTime(1.f),
	m_DamageNumber(0)
{}

CUIDamageFont::CUIDamageFont(const CUIDamageFont& Font) :
CUIWidget(Font)
{
	m_DamageNumber = Font.m_DamageNumber;
	m_Info = Font.m_Info;
}

CUIDamageFont::~CUIDamageFont()
{}

void CUIDamageFont::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = nullptr;

	if (m_Owner->GetViewPort())
	{
		m_Owner->GetViewPort()->GetScene()->GetResource()->LoadTexture(Name, FileName, PathName);
		Texture = m_Owner->GetViewPort()->GetScene()->GetResource()->FindTexture(Name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);
		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	if (Texture)
	{
		m_Info.m_Texture = Texture;
		SetSize((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		SetUseTexture(true);
	}
}

void CUIDamageFont::SetTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	const std::string& PathName)
{
	CTexture* Texture = nullptr;

	if (m_Owner->GetViewPort())
	{
		m_Owner->GetViewPort()->GetScene()->GetResource()->LoadTexture(Name, vecFileName, PathName);
		Texture = m_Owner->GetViewPort()->GetScene()->GetResource()->FindTexture(Name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);
		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	if (Texture)
	{
		m_Info.m_Texture = Texture;
		SetSize((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		SetUseTexture(true);
	}
}

void CUIDamageFont::SetTextureTint(const Vector4& Color)
{
	m_Info.m_Tint = Color;
}

void CUIDamageFont::SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_Info.m_Tint = Vector4(r / 255.f, g / 255.f, a / 255.f, a / 255.f);
}

void CUIDamageFont::AddFrameData(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData FrameData = {};
	FrameData.Start = StartPos;
	FrameData.Size  = Size;

	m_Info.m_vecFrameData.push_back(FrameData);
}

void CUIDamageFont::AddFrameData(int Count)
{
	for (int i = 0; i < Count; i++)
	{
		AnimationFrameData FrameData = {};
		FrameData.Start = Vector2(0.f, 0.f);
		FrameData.Size = Vector2((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		m_Info.m_vecFrameData.push_back(FrameData);
	}
}

bool CUIDamageFont::Init()
{
	if (!CUIWidget::Init())
		return false;

	if (m_Owner->GetViewPort())
		m_Shader = m_Owner->GetViewPort()->GetScene()->GetResource()->FindShader("NumberShader");
	else
		m_Shader = CResourceManager::GetInst()->FindShader("NumberShader");

	return true;
}

void CUIDamageFont::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);

	// 중력에 따른 위치 조작 

	m_LifeTime -= DeltaTime;

	if (m_LifeTime < 0.f)
	{
		Destroy();
	}
}

void CUIDamageFont::Render()
{
	int TempNum = m_DamageNumber;

	if (TempNum == 0)
	{
		m_vecNumber.push_back(0);
	}
	else
	{
		while (TempNum > 0)
		{
			m_vecNumber.push_back(TempNum % 10);
			TempNum /= 10;
		}
	}

	int CurrentNum = 0, Frame = 0;

	for (int i = 0; i < m_vecNumber.size(); i++)
	{
		Frame = 0;
		CurrentNum = m_vecNumber[i];

		if (m_Info.m_Texture)
		{
			switch (m_Info.m_Texture->GetImageType())
			{
			case Image_Type::Atlas :
				{
				AnimationFrameData FrameData = m_Info.m_vecFrameData[CurrentNum];

				Vector2 StartUV = (FrameData.Start) / Vector2((float)m_Info.m_Texture->GetWidth(),
					(float)m_Info.m_Texture->GetHeight());
				Vector2 EndUV = (FrameData.Start + FrameData.Size) / Vector2((float)m_Info.m_Texture->GetWidth(),
					(float)m_Info.m_Texture->GetHeight());

				m_CBuffer->SetStartUV(StartUV);
				m_CBuffer->SetEndUV(EndUV);

				}
				break;
			case Image_Type::Frame:
				{
				AnimationFrameData FrameData = m_Info.m_vecFrameData[CurrentNum];

				Vector2 StartUV = (FrameData.Start) / Vector2((float)m_Info.m_Texture->GetWidth(),
					(float)m_Info.m_Texture->GetHeight());
				Vector2 EndUV = (FrameData.Start + FrameData.Size) / Vector2((float)m_Info.m_Texture->GetWidth(),
					(float)m_Info.m_Texture->GetHeight());

				m_CBuffer->SetStartUV(StartUV);
				m_CBuffer->SetEndUV(EndUV);

				Frame = CurrentNum;
				}
				break;
			}
			m_Info.m_Texture->SetShader(0, (int)ConstantBuffer_Shader_Type::Pixel, Frame);
		}

		m_Tint = m_Info.m_Tint;
		m_RenderPos.x += (i * m_Size.x);
		CUIWidget::Render();

	}

}
