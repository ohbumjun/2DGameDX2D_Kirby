#pragma once
#include "UI\UIWindow.h"
#include "UI/UIButton.h"
#include "UI/UIText.h"
#include "UI/UIProgressbar.h"

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
    CSharedPtr<CUIProgressBar> m_ProgressBar;
public :
    CUIText* GetNameText() const
{
        return m_NameText;
}
    CUIProgressBar* GetProgressBar() const
{
        return m_ProgressBar;
}
public :
    void SetText(const TCHAR* Text);
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
    virtual CSimpleHUD* Clone() override;
};

