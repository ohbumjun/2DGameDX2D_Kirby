#pragma once

#include "../Ref.h"
#include "UIWidget.h"

class CUIWindow : public CRef
{
	friend class CViewPort;
	friend class CWidgetComponent;
protected :
	CUIWindow();
	CUIWindow(const CUIWindow& Window);
	virtual ~CUIWindow();
protected :
	class CWidgetComponent* m_OwnerComponent;
	class CScene* m_Scene;
	class CViewPort* m_ViewPort;
	int m_ZOrder;
	bool m_CollisionMouseEnable;
	Vector2 m_Pos;
	Vector2 m_Size;
	bool m_Start;
	float m_Angle;
	bool m_MoueHovered;
	std::list<CSharedPtr<CUIWidget>> m_WidgetList;
public :
	CWidgetComponent* GetWidgetComponent ()
	{
		return m_OwnerComponent;
	}
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
	void SetCollisionMouseEnable(bool Enable)
{
		m_CollisionMouseEnable = Enable;
}
public :
	bool CollisionMouse(const Vector2& MousePos);
public :
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CUIWindow* Clone();
public :
	static bool SortWidget(CSharedPtr<CUIWidget>& A, CSharedPtr<CUIWidget>& B)
{
		return A->m_ZOrder > B->m_ZOrder;
}
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
		Widget->SetOwner(this);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}


		m_WidgetList.push_back(Widget);

		return (T*)Widget;
}
};

