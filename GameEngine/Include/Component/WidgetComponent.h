#pragma once
#include "SceneComponent.h"
#include "../Scene/Scene.h"
#include "../UI/UIWindow.h"

class CWidgetComponent : public CSceneComponent {
	friend class CGameObject;
protected :
	CWidgetComponent();
	CWidgetComponent(const CWidgetComponent& Component);
	virtual ~CWidgetComponent();
private :
	CSharedPtr<CUIWindow> m_WidgetWindow;
	WidgetComponent_Space m_Space;
public :
	CUIWindow* GetWidgetWindow() const
{
		return m_WidgetWindow;
}
	WidgetComponent_Space GetWidgetComponentSpace() const
{
		return m_Space;
}
public :
	void SetWidgetSpace(WidgetComponent_Space Space)
	{
		m_Space = Space;
		if (m_Space == WidgetComponent_Space::Screen)
		{
			SetInheritRotX(false);
			SetInheritRotY(false);
			SetInheritRotZ(false);
			SetLayerName("ScreenWidgetComponent");
		}
		else
		{
			SetInheritRotX(true);
			SetInheritRotY(true);
			SetInheritRotZ(true);
			SetLayerName("Default");
		}
	}

	void SetWidgetWindow(CUIWindow* Window)
	{
		if (m_WidgetWindow)
		{
			m_WidgetWindow->m_OwnerComponent = nullptr;
		}
		m_WidgetWindow = Window;
		m_WidgetWindow->m_OwnerComponent = this;
	}
public :
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void CheckCollision();
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CWidgetComponent* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
public :
	template<typename T>
	T* CreateUIWindow(const std::string& Name)
{
		T* Window = new T;
		Window->SetName(Name);
		Window->m_ViewPort = m_Scene->GetViewPort();
	if (!Window->Init())
	{
		SAFE_DELETE(Window);
		return nullptr;
	}
	if (m_WidgetWindow)
	{
		m_WidgetWindow->m_OwnerComponent = nullptr;
	}
	Window->m_OwnerComponent = this;
	m_WidgetWindow = Window;
	return m_WidgetWindow;
}
};