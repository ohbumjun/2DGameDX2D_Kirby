#include "UIText.h"
#include "../../Device.h"
#include "../Resource/ResourceManager.h"

CUIText::CUIText() :
m_TextCount(0),
m_TextCapacity(4),
m_Font(nullptr),
m_Layout(nullptr),
m_ColorBrush(nullptr),
m_2DTarget(nullptr),
m_FontSize(20.f),
m_Alpha(true),
m_Opacity(1.f),
m_ShadowEnable(false),
m_ShadowColorBrush(nullptr),
m_ShadowAlpha(true),
m_ShadowOpacity(1.f),
m_ShadowOffset(1.f, 1.f),
m_AlignH(TEXT_ALIGN_H::Center),
m_Alignv(TEXT_ALIGN_V::Middle)
{
	m_Text = new TCHAR[m_TextCapacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	m_FontName = new TCHAR[128];

	memset(m_FontName, 0, sizeof(TCHAR) * 128);

	lstrcpy(m_Text, TEXT("Text"));

	m_TextCount = 4;

	// 알파값은 항상 1로 세팅
	m_Color.w = 1.f;
	m_ShadowColor.w = 1.f;
}

CUIText::CUIText(const CUIText& Text) : CUIWidget(Text)
{
	*this = Text;

	m_Text = new TCHAR[m_TextCapacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	if (m_TextCount > 0)
		memcpy(m_Text, Text.m_Text, sizeof(TCHAR) * m_TextCapacity);

	m_FontName = new TCHAR[128];

	memset(m_FontName, 0, sizeof(TCHAR) * 128);

	lstrcpy(m_FontName, Text.m_FontName);

	m_Layout = nullptr;

	CreateTextLayout();
}

CUIText::~CUIText()
{
	SAFE_RELEASE(m_Layout);
	SAFE_DELETE_ARRAY(m_Text);
	SAFE_DELETE_ARRAY(m_FontName);
}

void CUIText::SetFont(const std::string& Name)
{
	m_FontKey = Name;
	m_Font = CResourceManager::GetInst()->FindFont(m_FontKey);
	CreateTextLayout();
}

void CUIText::SetFontSize(float Size)
{
	m_FontSize = Size;
	CreateTextLayout();
}

void CUIText::SetAlignH(TEXT_ALIGN_H Align)
{
	m_AlignH = Align;
	CreateTextLayout();
}

void CUIText::SetAlignV(TEXT_ALIGN_V Align)
{
	m_Alignv = Align;
	CreateTextLayout();
}

void CUIText::SetColor(float r, float g, float b)
{
	m_Color.x = r;
	m_Color.y = g;
	m_Color.z = b;

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);
}

void CUIText::SetColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_Color.x = r / 255.f;
	m_Color.y = r / 255.f;
	m_Color.z = r / 255.f;

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);
}

void CUIText::SetColor(const Vector4& Color)
{
	m_Color.x = Color.x;
	m_Color.y = Color.y;
	m_Color.z = Color.z;

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);
}

void CUIText::SetColor(unsigned Color)
{
	m_Color.x = ((Color >> 16) & 0x000000ff) / 255.f;
	m_Color.y = ((Color >> 8) & 0x000000ff) / 255.f;
	m_Color.z = ((Color) & 0x000000ff) / 255.f;

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);
}

void CUIText::SetAlphaEnable(bool Enable)
{
	m_Alpha = Enable;
}

void CUIText::SetOpacity(float Opacity)
{
	m_Opacity = Opacity;
}

void CUIText::SetShadowEnable(bool ShadowEnable)
{
	m_ShadowEnable = ShadowEnable;
}

void CUIText::SetShadowOffset(const Vector2& Offset)
{
	m_ShadowOffset = Offset;
}

void CUIText::SetShadowColor(float r, float g, float b)
{
	m_ShadowColor.x = r;
	m_ShadowColor.y = g;
	m_ShadowColor.z = b;
}

void CUIText::SetShadowColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_ShadowColor.x = r / 255.f;
	m_ShadowColor.y = g / 255.f;
	m_ShadowColor.z = b / 255.f;

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
}

void CUIText::SetShadowColor(const Vector4& Color)
{
	m_ShadowColor.x = Color.x;
	m_ShadowColor.y = Color.y;
	m_ShadowColor.z = Color.z;

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
}

void CUIText::SetShadowColor(unsigned int Color)
{
	m_ShadowColor.x = ((Color >> 16) & 0x000000ff) / 255.f;
	m_ShadowColor.y = ((Color >> 8) & 0x000000ff) / 255.f;
	m_ShadowColor.z = ((Color) & 0x000000ff) / 255.f;

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
}

void CUIText::SetShadowAlphaEnable(bool Enable)
{
	m_ShadowAlpha = Enable;
}

void CUIText::SetShadowOpacity(float Opacity)
{
	m_ShadowOpacity = Opacity;
}

bool CUIText::CreateTextLayout()
{
	if (!m_Font)
		return false;

	SAFE_RELEASE(m_Layout);

	m_Layout = CResourceManager::GetInst()->CreateTextLayout(m_Text, m_Font, m_Size.x, m_Size.y);

	// Size 조절
	DWRITE_TEXT_RANGE Range = {};
	Range.startPosition = 0;
	Range.length = lstrlen(m_Text);

	switch (m_AlignH)
	{
	case TEXT_ALIGN_H::Left :
		m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case TEXT_ALIGN_H::Center:
		m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_H::Right:
		m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	}

	switch (m_Alignv)
	{
	case TEXT_ALIGN_V::Top:
		m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case TEXT_ALIGN_V::Middle:
		m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_V::Bottom:
		m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	}

	return true;
}

bool CUIText::Init()
{
	if (!CUIWidget::Init())
		return false;

	m_2DTarget = CDevice::GetInst()->Get2DRenderTarget();

	const TCHAR* FontName = CResourceManager::GetInst()->GetFontFaceName("Default");

	lstrcpy(m_FontName, FontName);

	m_FontKey = "Default";

	m_Font = CResourceManager::GetInst()->FindFont(m_FontKey);

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);

	CreateTextLayout();

	return true;
}

void CUIText::Start()
{
	CUIWidget::Start();
}

void CUIText::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);
}

void CUIText::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

void CUIText::Render()
{
	CUIWidget::Render();
}
