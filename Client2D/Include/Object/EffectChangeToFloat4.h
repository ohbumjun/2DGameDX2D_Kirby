#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToFloat4 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToFloat4();
    virtual ~CEffectChangeToFloat4();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
private:
    void MakeSceneChangeEffect(const CollisionResult& Result);
    void ChangeSceneToFloat4Scene();
    void SetSceneChangeCallbackToPlayer(const CollisionResult& Result);
    void ResetSceneChangeCallbackToPlayer(const CollisionResult& Result);
public:
    virtual void Load(FILE* pFile) override;
};
