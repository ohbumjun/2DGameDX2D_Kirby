#pragma once
#include "UI\UIWindow.h"
#include "UI/UIText.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIProgressBar.h"

class CPlayerHUD :
    public CUIWindow
{
    friend class CViewPort;
protected:
    CPlayerHUD();
    virtual ~CPlayerHUD();
private:
    CSharedPtr<CUIText> m_NameText;
    CSharedPtr<CUIImage> m_IconImage;
    CSharedPtr<CUIProgressBar> m_HPProgressBar;
    CSharedPtr<CUIImage> m_HPProgressBarBack;
    CSharedPtr<CUIProgressBar> m_MPProgressBar;
    CSharedPtr<CUIImage> m_MPProgressBarBack;
    CSharedPtr<CUIImage> m_MPProgressBarWhiteBack;
private :
    float m_MPBarToggleMaxTime;
    float m_MPBarToggleTime;
    bool m_IsOpacityToggle;
public :
    CUIProgressBar* GetHPProgressBar() const
	{
        return m_HPProgressBar;
	}
    CUIProgressBar* GetMPProgressBar() const
    {
        return m_MPProgressBar;
    }
    CUIImage* GetIconImage() const
{
        return m_IconImage;
}
    CUIText* GetNameText() const
{
        return m_NameText;
}
public :
    void SetText(const TCHAR* Text);
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
};

