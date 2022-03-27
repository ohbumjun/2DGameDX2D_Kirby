#pragma once
#include "UI\UIWindow.h"
#include "UI/UIProgressbar.h"
#include "UI/UIImage.h"

class CLoadingWidget :
    public CUIWindow
{
    friend class CViewPort;

protected:
    CLoadingWidget();
    CLoadingWidget(const CLoadingWidget& Widget);
    ~CLoadingWidget();
private :
    CSharedPtr<CUIProgressBar> m_ProgressBar;
    CSharedPtr<CUIImage> m_FrontImage;
    CSharedPtr<CUIImage> m_BackImage;
public :
    void SetFrontTexture(const char* Name, const TCHAR* FileName);
    void SetBackTexture(const char* Name, const TCHAR* FileName);
    virtual bool Init() override;
public :
    void SetLoadingPercent(float Percent)
{
        m_ProgressBar->SetPercent(Percent);
}
};

