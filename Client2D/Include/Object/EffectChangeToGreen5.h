#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToGreen5 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToGreen5();
    CEffectChangeToGreen5(const CEffectChangeToGreen5& Dash);
    virtual ~CEffectChangeToGreen5();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectChangeToGreen5* Clone() override;
private:
    void ChangeSceneToGreen5Scene(const CollisionResult& Result);
    void SetSceneChangeCallbackToPlayer(const CollisionResult& Result);
    void ResetSceneChangeCallbackToPlayer(const CollisionResult& Result);
public:
    virtual void Load(FILE* pFile) override;
};

