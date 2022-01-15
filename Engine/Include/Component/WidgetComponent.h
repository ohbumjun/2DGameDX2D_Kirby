#pragma once
#include "SceneComponent.h"
#include "../UI/UIWindow.h"

class CWidgetComponent :
    public CSceneComponent
{
    friend class CGameObject;
protected:
    CWidgetComponent();
    CWidgetComponent(const CWidgetComponent& Component);
    virtual ~CWidgetComponent();
protected:
    CSharedPtr<CUIWindow> m_WidgetWindow;
    WidgetComponent_Space m_Space;
public :
    WidgetComponent_Space GetWidgetSpace() const
{
        return m_Space;
}
public :
    void SetWidgetWindow(CUIWindow* Window)
{
        if (m_WidgetWindow)
            m_WidgetWindow->m_OwnerComponent = nullptr;
        m_WidgetWindow = Window;
        m_WidgetWindow->m_OwnerComponent = this;
}
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
public:
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
    // 기본적으로 Viewport를 통해서 UIWindow를 만들지만,
    // 여기서는 Viewport를 통해 만드는 것이 아니라 자체 제작
    // 즉, 해당 UIWindow를 관리하려면, 별도의 프로세스가 필요 ( viewport 로 관리하는 것 외에 )
    template<typename T>
    T* CreateUIWindow(const std::string& Name)
{
        T* Window = new T;
        Window->SetName(Name);
        Window->m_OwnerComponent = this;
    if (!Window->Init())
    {
        SAFE_RELEASE(Window);
        return nullptr;
    }

    m_WidgetWindow = Window;

    return Window;
}
};

