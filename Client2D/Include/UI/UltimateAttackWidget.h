#pragma once
#include "UI\UIWindow.h"
#include "UI/UIImage.h"

class CUltimateAttackWidget :
    public CUIWindow
{
    friend class CViewPort;
protected:
    CUltimateAttackWidget();
    ~CUltimateAttackWidget();
private:
    CSharedPtr<CUIImage> m_FrontImage;
    float m_UIProceedTime;
    float m_UIProceedTimeMax;
    float m_UIProceedSpeed;
    Resolution m_RS;
public :
    void SetUITexture(const char* Name, const TCHAR* FileName);
	void SetUIProceedTime(float Time)
	{
        m_UIProceedTimeMax = Time;
	}
public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
};

