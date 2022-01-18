#pragma once
#include "UI/UIWindow.h"
#include "UI/UIText.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UISlideBar.h"

class CMainWidget :
    public CUIWindow
{
    friend class CViewPort;
protected :
    CMainWidget();
    virtual ~CMainWidget();
private :
    CSharedPtr<CUIButton> m_Button;
    CSharedPtr<CUIButton> m_Button1;
    CSharedPtr<CUIText> m_Text;
    CSharedPtr<CUIImage> m_TestImage;
    CSharedPtr<CUISlideBar> m_TestSlider;
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
public :
    void ButtonClick();
};

