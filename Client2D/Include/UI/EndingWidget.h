#pragma once
#include "UI\UIWindow.h"
#include "UI/UIImage.h"

class CEndingWidget :
    public CUIWindow
{
    friend class CViewPort;
protected:
    CEndingWidget();
    virtual ~CEndingWidget();
protected:
    CSharedPtr<CUIImage> m_NormalKirbyLeftImage;
    CSharedPtr<CUIImage> m_TrophyKirbyImage;
    CSharedPtr<CUIImage> m_NormalKirbyRightImage;
protected:
    CSharedPtr<CUIImage> m_FightBackGroundImage;
    CSharedPtr<CUIImage> m_FightForeGroundImage;
protected :
    CSharedPtr<CUIImage> m_BlackToLeft;
	CSharedPtr<CUIImage> m_BlackToRight;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
};


