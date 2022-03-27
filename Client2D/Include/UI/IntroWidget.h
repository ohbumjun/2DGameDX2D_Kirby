#pragma once
#include "UI\UIWindow.h"
#include "UI/UIImage.h"
#include "UI/UIButton.h"

class CIntroWidget :
    public CUIWindow
{
    friend class CViewPort;
protected:
    CIntroWidget();
    virtual ~CIntroWidget();
protected :
    CSharedPtr<CUIImage> m_NormalKirbyImage;
    CSharedPtr<CUIImage> m_RadioKirbyImage;
    CSharedPtr<CUIImage> m_MovieKirbyImage;
    CSharedPtr<CUIImage> m_KirbyWithPenguinImage;
protected:
    CSharedPtr<CUIImage> m_KirbyBackGroundImage;
    CSharedPtr<CUIButton> m_StartButton;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
private :
    void StartGameButtonClick();
};

