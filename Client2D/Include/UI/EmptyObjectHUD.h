#pragma once
#include "UI\UIWindow.h"

class CEmptyObjectHUD :
    public CUIWindow
{
    friend class CViewport;
    friend class CWidgetComponent;
protected:
    CEmptyObjectHUD();
    CEmptyObjectHUD(const CEmptyObjectHUD& window);
    virtual ~CEmptyObjectHUD() override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
    virtual CEmptyObjectHUD* Clone() override;
};

