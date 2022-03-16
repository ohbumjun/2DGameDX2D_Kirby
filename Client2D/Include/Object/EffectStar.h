#pragma once
#include "GameObject\GameObject.h"

class CEffectStar :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectStar();
    CEffectStar(const CEffectStar& Dash);
    virtual ~CEffectStar();
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderCircle> m_ColliderBody;
    Vector2 m_SpitOutDir;
    float m_AliveTime;
    float m_StarMoveSpeed;
public :
    void SetStarMoveSpeed(float Speed)
{
        m_StarMoveSpeed = Speed;
}
    void SetSpitOutDir(const Vector2& Dir)
{
        m_SpitOutDir = Dir;
}
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectStar* Clone() override;
private :
    void StarCollision(const CollisionResult& Result);
};

