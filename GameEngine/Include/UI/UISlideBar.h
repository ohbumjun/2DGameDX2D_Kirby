#pragma once
#include "UIWidget.h"

class CUISlideBar :
    public CUIWidget
{
    friend class CUIWindow;
protected :
    CUISlideBar();
    CUISlideBar(const CUISlideBar& SlideBar);
    virtual ~CUISlideBar() override;
private :
    bool m_MousePush;
    bool m_MoueOn;
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
    virtual CUISlideBar* Clone() override;
};

