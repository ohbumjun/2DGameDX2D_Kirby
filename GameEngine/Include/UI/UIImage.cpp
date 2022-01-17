#include "UIImage.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"
#include "../Scene/ViewPort.h"

CUIImage::CUIImage()
{}

CUIImage::CUIImage(const CUIImage& Widget)
{
	m_ClickCallback = nullptr;
}

CUIImage::~CUIImage()
{}

void CUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CSceneResource* Resource = m_Owner->GetViewPort()->GetScene()->GetResource();
	Resource->LoadTexture(Name, FileName, PathName);

	m_Info.m_Texture = Resource->FindTexture(Name);

	if (m_Info.m_Texture)
		SetUseTexture(true);

}

void CUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CSceneResource* Resource = m_Owner->GetViewPort()->GetScene()->GetResource();
	Resource->LoadTextureFullPath(Name, FullPath);

	m_Info.m_Texture = Resource->FindTexture(Name);

	if (m_Info.m_Texture)
		SetUseTexture(true);
}

void CUIImage::AddAnimationFrameData(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData Data = {};
	Data.Start = StartPos;
	Data.Size = Size;

	m_Info.m_vecFrameData.push_back(Data);
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
}

void CUIImage::Render()
{
	if (m_Info.m_Texture)
		m_Info.m_Texture->SetShader(0, (int)ConstantBuffer_Shader_Type::Pixel, 0);

	m_Tint = m_Info.m_Tint;

	CUIWidget::Render();
}

CUIImage* CUIImage::Clone()
{
	return new CUIImage(*this);
}
