#pragma once
#include "UI\UIWindow.h"
#include "UI/UIButton.h"
#include "UI/UIText.h"

class CSimpleHUD :
    public CUIWindow
{
    friend class CViewport;
    friend class CWidgetComponent;
protected:
    CSimpleHUD();
    CSimpleHUD(const CSimpleHUD& window);
    virtual ~CSimpleHUD() override;
private :
    CSharedPtr<CUIButton> m_Button;
    CSharedPtr<CUIText> m_NameText;
public :
    void SetText(const TCHAR* Text);
public :
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CSimpleHUD* Clone();
};

