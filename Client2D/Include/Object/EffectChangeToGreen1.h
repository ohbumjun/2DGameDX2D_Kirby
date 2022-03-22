#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToGreen1 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToGreen1();
    CEffectChangeToGreen1(const CEffectChangeToGreen1& Dash);
    virtual ~CEffectChangeToGreen1();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectChangeToGreen1* Clone() override;
private:
    void MakeSceneChangeEffect(const CollisionResult& Result);
    void ChangeSceneToGreen1Scene();
    void SetSceneChangeCallbackToPlayer(const CollisionResult& Result);
    void ResetSceneChangeCallbackToPlayer(const CollisionResult& Result);
public:
    virtual void Load(FILE* pFile) override;
};

