#include "IMGUIImage.h"
#include "Resource/ResourceManager.h"
#include "Resource/Texture/Texture.h"

CIMGUIImage::CIMGUIImage() :
	m_ImageStart{},
	m_ImageEnd{},
	m_Tint{1.f, 1.f, 1.f, 1.f},
	m_BorderColor{}
{
}

CIMGUIImage::~CIMGUIImage()
{
}

void CIMGUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (!m_Texture)
		return;

	SetImageEnd(static_cast<float>(m_Texture->GetWidth()), static_cast<float>(m_Texture->GetHeight()));
}

void CIMGUIImage::SetTexture(const std::string& Name)
{
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (!m_Texture)
		return;

	SetImageStart(0.f, 0.f);
	SetImageEnd(static_cast<float>(m_Texture->GetWidth()), static_cast<float>(m_Texture->GetHeight()));
}

void CIMGUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (!m_Texture)
		return;

	SetImageEnd(static_cast<float>(m_Texture->GetWidth()), static_cast<float>(m_Texture->GetHeight()));
}

void CIMGUIImage::SetTexture(CTexture* Texture)
{
	m_Texture = Texture;

	if (!m_Texture)
		return;

	SetImageEnd(static_cast<float>(m_Texture->GetWidth()), static_cast<float>(m_Texture->GetHeight()));
}

bool CIMGUIImage::Init()
{
	SetTexture("DefaultUI", TEXT("DefaultUI.png"));

	// SetImageStart(0.f, 0.f);
	// SetImageEnd(100.f, 100.f);

	return true;
}

void CIMGUIImage::Render()
{
	if (m_Texture)
	{
		unsigned int Width  = m_Texture->GetWidth();
		unsigned int Height = m_Texture->GetHeight();
		// if (Width != m_Size.x)
		//	Width = m_Size.x;
		//if (Height != m_Size.y)
		//	Height = m_Size.y;

		ImVec2 StartUV = ImVec2(m_ImageStart.x / Width, m_ImageStart.y / Height);
		ImVec2 EndUV   = ImVec2(m_ImageEnd.x / Width, m_ImageEnd.y / Height);

		ImGui::Image(m_Texture->GetResource(), m_Size, StartUV, EndUV, m_Tint, m_BorderColor);
		// ImGui::Image(m_Texture->GetResource(), ImVec2(100.f, 100.f), StartUV, EndUV, m_Tint, m_BorderColor);
	}
}
