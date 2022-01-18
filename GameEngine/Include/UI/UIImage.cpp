#include "UIImage.h"
#include "../Animation/AnimationSequence2DInstance.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"
#include "../Scene/ViewPort.h"

CUIImage::CUIImage() :
	m_PlayTime(1.f),
m_AnimTime(0.f),
m_FrameIndex(0)
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
		SetUseTexture(true);

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
		SetUseTexture(true);
}

void CUIImage::AddAnimationFrameData(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData Data = {};
	Data.Start = StartPos;
	Data.Size = Size;

	m_Info.m_vecFrameData.push_back(Data);

	SetUseAnimation(true);
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

	// Frame 정보 Update
	if (!m_Info.m_vecFrameData.empty())
	{
		float FrameTime = m_PlayTime / m_Info.m_vecFrameData.size();

		m_AnimTime += DeltaTime;
		if (m_AnimTime >= FrameTime)
		{
			m_FrameIndex = (m_FrameIndex + 1) % (int)m_Info.m_vecFrameData.size();
			m_AnimTime -= FrameTime;
		}
	}
}

void CUIImage::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

void CUIImage::Render()
{
	// Animation이 있을 때랑 없을 때를 구분해서 세팅한다.

	if (m_Info.m_Texture)
		m_Info.m_Texture->SetShader(0, (int)ConstantBuffer_Shader_Type::Pixel, 0);

	// Animation 이 존재한다면, StartUV, EndUV도 세팅하기
	if (!m_Info.m_vecFrameData.empty() && m_Info.m_Texture)
	{
		Vector2 m_AnimStartPos = m_Info.m_vecFrameData[m_FrameIndex].Start;
		Vector2 m_AnimEndPos = m_AnimStartPos + m_Info.m_vecFrameData[m_FrameIndex].Size;

		Vector2 StartUV = m_AnimStartPos / Vector2((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		Vector2 EndUV = m_AnimEndPos / Vector2((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());

		m_CBuffer->SetStartUV(StartUV);
		m_CBuffer->SetEndUV(EndUV);
	}
	
	m_Tint = m_Info.m_Tint;

	CUIWidget::Render();
}

CUIImage* CUIImage::Clone()
{
	return new CUIImage(*this);
}
