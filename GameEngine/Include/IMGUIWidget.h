#pragma once

#include "GameInfo.h"

class CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIWidget();
	virtual ~CIMGUIWidget();

protected:
	std::string         m_Name;
	class CIMGUIWindow* m_Owner;
	ImVec2              m_Size;
	ImColor             m_Color;
	ImFont*             m_Font;

public:
	void SetOwner(class CIMGUIWindow* Owner)
	{
		m_Owner = Owner;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	void SetHideName(bool Hide)
	{
		if (Hide)
			m_Name = "##" + m_Name;
	}

	void SetSize(const ImVec2& Size)
	{
		m_Size = Size;
	}

	void SetSize(const Vector2& Size)
	{
		m_Size.x = Size.x;
		m_Size.y = Size.y;
	}

	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

	void SetColor(unsigned char r, unsigned char g, unsigned char b,
	              unsigned char a = 255)
	{
		m_Color.Value.x = r / 255.f;
		m_Color.Value.y = g / 255.f;
		m_Color.Value.z = b / 255.f;
		m_Color.Value.w = a / 255.f;
	}

public :
	Vector2 GetSize() const
	{
		return Vector2(m_Size.x, m_Size.y);
	}

public:
	virtual bool Init();
	virtual void Render();
};
