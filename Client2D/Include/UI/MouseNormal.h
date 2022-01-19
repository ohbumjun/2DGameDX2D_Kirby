#pragma once

#include "UI\UIWindow.h"
#include "UI/UIImage.h"

class CMouseNormal :
    public CUIWindow
{
    friend class CViewPort;
    friend class CWidgetComponent;
    friend class CEngine;
protected :
    CMouseNormal();
    CMouseNormal(const CMouseNormal& window);
    virtual ~CMouseNormal() override;
private :
    CSharedPtr<CUIImage> m_Image;
public :
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CMouseNormal* Clone();
};

