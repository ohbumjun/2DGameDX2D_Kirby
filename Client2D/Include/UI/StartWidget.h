#pragma once
#include "UI\UIWindow.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"

class CStartWidget :
    public CUIWindow
{
    friend class CViewPort;

protected:
    CStartWidget();
    CStartWidget(const CStartWidget& Widget);
    virtual ~CStartWidget() override;
private :
    CSharedPtr<CUIButton> m_StartButton;
    CSharedPtr<CUIButton> m_ExitButton;
    CSharedPtr<CUIImage> m_Back;
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
    virtual CStartWidget* Clone() override;
public :
    void StartButtonClick();
    void ExitButtonClick();
};

