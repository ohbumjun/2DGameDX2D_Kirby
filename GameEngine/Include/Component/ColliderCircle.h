#pragma once
#include "ColliderComponent.h"

class CColliderCircle :
    public CColliderComponent
{
public :
    CColliderCircle();
    CColliderCircle(const CColliderCircle& Circle);
    virtual ~CColliderCircle() override;
protected :
    CircleInfo m_Info;
public :
    CircleInfo GetInfo() const
{
        return m_Info;
}
    void SetInfo(const Vector2& Center, float Radius)
{
        m_Info.Center = Center;
        m_Info.Radius = Radius;
        SetWorldScale(Radius * 2.f, Radius * 2.f, 1.f);
}
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CColliderCircle* Clone() override;
public :
    virtual void Save(FILE* pFile) override;
    virtual void Load(FILE* pFile) override;
public :
    virtual bool Collision(CColliderComponent* Dest) override;
    virtual bool CollisionMouse(const Vector2& MousePos) override;
};

