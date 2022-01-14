#pragma once

#include "UIWidget.h"

enum class TEXT_ALIGN_H {
	Left,
	Center,
	Right
};

enum class TEXT_ALIGN_V {
	Top,
	Middle,
	Bottom
};


class CUIText :
    public CUIWidget
{
	friend class CUIWindow;
protected :
	CUIText();
	CUIText(const CUIText& Text);
	virtual ~CUIText() override;
protected :
	TCHAR* m_Text;
	int m_TextCount;
	int m_TextCapacity;
protected :
	IDWriteTextFormat* m_Font;
	IDWriteTextLayout* m_Layout;
	ID2D1SolidColorBrush* m_ColorBrush;
	ID2D1RenderTarget* m_2DTarget;
	std::string m_FontKey;
	TCHAR* m_FontName;
	float m_FontSize;
	bool m_Alpha;
	float m_Opacity;
	Vector4 m_Color;

	// Shadow
	bool m_ShadowEnable;
	ID2D1SolidColorBrush* m_ShadowColorBrush;
	bool m_ShadowAlpha;
	float m_ShadowOpacity;
	Vector4 m_ShadowColor;
	Vector2 m_ShadowOffset;

	TEXT_ALIGN_H m_AlignH;
	TEXT_ALIGN_V m_Alignv;
public :
	const TCHAR* GetText() const
{
		return m_Text;
}
	int GetTextCount() const
{
		return m_TextCount;
}
public :
	void SetText(const TCHAR* Text)
{
		int Length = lstrlen(Text);
	if (Length + 1 > m_TextCapacity)
	{
		m_TextCapacity = Length + 1;
		SAFE_DELETE_ARRAY(m_Text);
		m_Text = new TCHAR[m_TextCapacity];
		memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
	}
	lstrcpy(m_Text, Text);
	m_TextCount = Length;

	CreateTextLayout();
}
	void AddText(const TCHAR* Text)
{
		int AddLength = lstrlen(Text);
	if (m_TextCount + AddLength + 1 > m_TextCapacity)
	{
		m_TextCapacity = 2 * (m_TextCount + AddLength + 1);
		TCHAR* Text = new TCHAR[m_TextCapacity];
		memset(Text, 0, sizeof(TCHAR) * m_TextCapacity);
		memcpy(Text, m_Text, sizeof(TCHAR) * m_TextCapacity);
		SAFE_DELETE_ARRAY(m_Text);
		m_Text = Text;
	}
	lstrcat(m_Text, Text);
	m_TextCount += AddLength;
	CreateTextLayout();
}
	void pop_back()
{
	if (m_TextCount > 0)
	{
		m_TextCount -= 1;
		m_Text[m_TextCount] = 0;
		CreateTextLayout();
	}
}

	void Clear()
{
	if (m_TextCount > 0)
	{
		m_TextCount = 0;
		memset(m_Text, 0, sizeof(TCHAR) * MAX_PATH);
		CreateTextLayout();
	}
}
public :
	void SetFont(const std::string& Name);
	void SetFontSize(float Size);
	void SetAlignH(TEXT_ALIGN_H Align);
	void SetAlignV(TEXT_ALIGN_V Align);
	void SetColor(float r, float g, float b);
	void SetColor(unsigned char r, unsigned char g, unsigned char b);
	void SetColor(const Vector4& Color);
	void SetColor(unsigned int Color);
	void SetAlphaEnable(bool Enable);
	void SetOpacity(float Opacity);

	void SetShadowEnable(bool ShadowEnable);
	void SetShadowOffset(const Vector2& Offset);
	void SetShadowColor(float r, float g, float b);
	void SetShadowColor(unsigned char r, unsigned char g, unsigned char b);
	void SetShadowColor(const Vector4& Color);
	void SetShadowColor(unsigned int Color);
	void SetShadowAlphaEnable(bool Enable);
	void SetShadowOpacity(float Opacity);

	bool CreateTextLayout();
public :
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void Render() override;
};

