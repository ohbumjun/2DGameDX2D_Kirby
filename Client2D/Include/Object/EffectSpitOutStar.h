#pragma once
#include "GameObject\GameObject.h"

class CEffectSpitOutStar :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectSpitOutStar();
    CEffectSpitOutStar(const CEffectSpitOutStar& Dash);
    virtual ~CEffectSpitOutStar();
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderCircle> m_ColliderBody;
    float m_SpitOutDir;
    float m_AliveTime;
public :
    void SetSpitOutDir(float Dir)
{
        m_SpitOutDir = Dir;
}
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectSpitOutStar* Clone() override;
private :
    void StarCollision(const CollisionResult& Result);
};

