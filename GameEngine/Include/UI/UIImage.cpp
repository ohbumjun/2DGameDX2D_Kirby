#include "UIImage.h"
#include "../Animation/AnimationSequence2DInstance.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"
#include "../Scene/ViewPort.h"

CUIImage::CUIImage() :
m_Info{}
{}

CUIImage::CUIImage(const CUIImage& Widget)
{
	m_ClickCallback = nullptr;
}

CUIImage::~CUIImage()
{
}

void CUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (m_Owner->GetViewPort())
	{
		CSceneResource* Resource = m_Owner->GetViewPort()->GetScene()->GetResource();
		Resource->LoadTexture(Name, FileName, PathName);

		m_Info.m_Texture = Resource->FindTexture(Name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);
		m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	if (m_Info.m_Texture)
	{
		SetSize((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		SetUseTexture(true);
	}

}

void CUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{

	if (m_Owner->GetViewPort())
	{
		CSceneResource* Resource = m_Owner->GetViewPort()->GetScene()->GetResource();
		Resource->LoadTextureFullPath(Name, FullPath);
		m_Info.m_Texture = Resource->FindTexture(Name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);
		m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	}


	if (m_Info.m_Texture)
	{
		SetSize((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		SetUseTexture(true);
	}
}

bool CUIImage::SetTexture(const std::string& Name, 
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
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
	
	return true;
}

void CUIImage::AddAnimationFrameData(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData Data = {};
	Data.Start = StartPos;
	Data.Size = Size;

	m_Info.m_vecFrameData.push_back(Data);

	SetUseAnimation(true);
}

void CUIImage::AddAnimationFrameData(int Count)
{
	size_t Size = m_Info.m_Texture->GetImageCount();
	for (size_t i = 0; i < Size; i++)
	{
		AnimationFrameData Data = {};
		Data.Start = Vector2(0.f, 0.f);
		Data.Size = Vector2((float)m_Info.m_Texture->GetHeight(i), (float)m_Info.m_Texture->GetWidth(i));
		m_Info.m_vecFrameData.push_back(Data);
	}
}

void CUIImage::SetTextureTint(const Vector4& Color)
{
	m_Info.m_Tint = Color;
}

void CUIImage::SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_Info.m_Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

bool CUIImage::Init()
{
	if (!CUIWidget::Init())
		return false;


	return true;
}

void CUIImage::Start()
{
	CUIWidget::Start();
}

void CUIImage::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);
}

void CUIImage::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
	// Frame 정보 Update
	if (!m_Info.m_vecFrameData.empty())
	{
		float FrameTime = m_Info.m_PlayTime / m_Info.m_vecFrameData.size();

		m_Info.m_AnimTime += DeltaTime * m_Info.m_PlayTime;

		if (m_Info.m_AnimTime >= FrameTime)
		{
			m_Info.m_FrameIndex = (m_Info.m_FrameIndex + 1) % (int)m_Info.m_vecFrameData.size();
			m_Info.m_AnimTime -= FrameTime;
		}
	}
}

void CUIImage::Render()
{
	int Frame = 0;

	// Animation 이 존재한다면, StartUV, EndUV도 세팅하기
	if (!m_Info.m_vecFrameData.empty())
	{
		CTexture* AnimTexture = m_Info.m_Texture;

		m_CBuffer->SetUseAnimation(true);
		m_CBuffer->SetAnimType(AnimTexture->GetImageType());

		switch (AnimTexture->GetImageType())
		{
		case Image_Type::Atlas:
		{
			Vector2 AnimStartPos = m_Info.m_vecFrameData[m_Info.m_FrameIndex].Start;
			Vector2 AnimEndPos = AnimStartPos + m_Info.m_vecFrameData[m_Info.m_FrameIndex].Size;

			Vector2 StartUV = AnimStartPos / Vector2((float)AnimTexture->GetWidth(), (float)AnimTexture->GetHeight());
			Vector2 EndUV = AnimEndPos / Vector2((float)AnimTexture->GetWidth(), (float)AnimTexture->GetHeight());

			m_CBuffer->SetStartUV(StartUV);
			m_CBuffer->SetEndUV(EndUV);
		}
		break;
		case Image_Type::Frame:
		{
			// 즉, Atlas 일 때는 FrameIndex가 곧, AnimationFrameData 를 의미하지만
			// Frame일 때는 Texture를 구성하는 TextureInfo 구조체 하나하나를 말하는 것이다 ( 여러 Image가 모여서
			// 하나의 Animation 구성 --> 하나의 Image 자체가 하나의 AnimationFrame 개념 )
			Frame = m_Info.m_FrameIndex;
			m_CBuffer->SetStartUV(Vector2(0.f, 0.f));
			m_CBuffer->SetEndUV(Vector2(1.f, 1.f));
		}
		break;
		case Image_Type::Array:
			break;
		}
	}
	else
		m_CBuffer->SetUseAnimation(false);

	if (m_Info.m_Texture)
		m_Info.m_Texture->SetShader(0, (int)ConstantBuffer_Shader_Type::Pixel, Frame);
	
	m_Tint = m_Info.m_Tint;

	CUIWidget::Render();
}

CUIImage* CUIImage::Clone()
{
	return new CUIImage(*this);
}
