#pragma once

#include "../Ref.h"
#include "UIWidget.h"

class CUIWindow : public CRef
{
	friend class CViewPort;
protected :
	CUIWindow();
	virtual ~CUIWindow();
private :
	class CScene* m_Scene;
	class CViewPort* m_ViewPort;
	int m_ZOrder;
	Vector2 m_Pos;
	Vector2 m_Size;
	bool m_Start;
	std::list<CSharedPtr<CUIWidget>> m_WidgetList;
public :
	int GetZOrder() const
{
		return m_ZOrder;
}
	Vector2 GetWindowPos() const
{
		return m_Pos;
}
	Vector2 GetWindowSize() const
{
		return m_Size;
}
	class CViewPort* GetViewPort() const
{
		return m_ViewPort;
}
public :
	void SetZOrder(int ZOrder)
{
		m_ZOrder = ZOrder;
}
	void SetPos(const Vector2& Pos)
{
		m_Pos = Pos;
}
	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}
	void SetSize(float x, float y)
{
		m_Size = Vector2(x, y);
}
	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}
	void SetScene(class CScene* Scene)
{
		m_Scene = Scene;
}
	void SetViewPort(class CViewPort* m_ViewPort)
{
		m_ViewPort = m_ViewPort;
}
public :
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
public :
	template<typename T>
	T* FindUIWidget(const std::string& Name)
{
		auto iter = m_WidgetList.begin();
		auto iterEnd = m_WidgetList.end();
	for(; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return (T*)(*iter);
	}
	return nullptr;
}
	template<typename T>
	T* CreateUIWidget(const std::string& Name)
{
		T* Widget = new T;
		Widget->SetName(Name);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		Widget->SetOwner(this);
		Widget->SetScene(m_Scene);

		m_WidgetList.push_back(Widget);

		return (T*)Widget;
}
};

