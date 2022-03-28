#pragma once
#include "UIWindow.h"
#include "UIText.h"

class CMonsterEditorHUD :
    public CUIWindow
{
    friend class CViewport;
    friend class CWidgetComponent;
protected:
    CMonsterEditorHUD();
    virtual ~CMonsterEditorHUD() override;
private :
    CSharedPtr<CUIText> m_NameText;
public :
    void SetText(const TCHAR* Text);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
};

