#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToDyna2 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToDyna2();
    virtual ~CEffectChangeToDyna2();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
    CSharedPtr<class CWidgetComponent> m_SimpleHUDWidget;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Load(FILE* pFile) override;
};

