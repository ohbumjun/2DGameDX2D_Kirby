#pragma once
#include "UI\UIWindow.h"
#include "UI/UIText.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIProgressBar.h"

class CBossHUD :
    public CUIWindow
{
    friend class CViewPort;
protected:
    CBossHUD();
    virtual ~CBossHUD();
private:
    CSharedPtr<CUIImage> m_IconImage;
    CSharedPtr<CUIImage> m_VSIconImage;
    CSharedPtr<CUIProgressBar> m_HPProgressBar;
    CSharedPtr<CUIProgressBar> m_HPProgressBarBack;
private :
    bool m_EnableStart;
    bool m_InitHPReachedEnd;
    float m_CurrentOpacity;
    bool m_StartDestroy;
    bool m_InitHPIncreaseStart;
public:
    CUIProgressBar* GetHPProgressBar() const
    {
        return m_HPProgressBar;
    }
    CUIImage* GetIconImage() const
    {
        return m_IconImage;
    }
    CUIImage* GetVSIconImage() const
    {
        return m_VSIconImage;
    }
public:
    void EnableBossHUD();
    void SetText(const TCHAR* Text);
    void SetCallback();
    void StartDestroy();
    void UpdateDestroyOpacity(float DeltaTime);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
};

