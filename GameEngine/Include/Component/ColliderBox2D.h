#pragma once
#include "ColliderComponent.h"

class CColliderBox2D :
    public CColliderComponent
{
    friend class CGameObject;
protected :
    CColliderBox2D();
    CColliderBox2D(const CColliderBox2D& Collider);
    virtual ~CColliderBox2D() override;
protected :
    Box2DInfo m_Info;
public :
    Box2DInfo GetInfo () const
	{
        return m_Info;
	}
    void SetExtend(float Width, float Height)
	{
	    // 사실상 반지름 길이
        m_Info.Length.x = Width;
        m_Info.Length.y = Height;

		 // 세팅한 길이의 * 2 를 해줘야 전체 크기가 된다
        SetWorldScale(m_Info.Length.x * 2, m_Info.Length.y * 2, 1.f);
	}
public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CColliderComponent* Clone();
    virtual void CheckCollision();
public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
public:
    virtual bool Collision(CColliderComponent* Dest);
    virtual bool CollisionMouse(const Vector2& MousePos);
};


