#pragma once
#include "SceneComponent.h"

class CColliderComponent :
    public CSceneComponent
{
    friend class CGameObject;
protected :
    CColliderComponent();
    CColliderComponent(const CColliderComponent& com);
    virtual ~CColliderComponent();
protected :
    Collider_Type m_ColliderType;
    Vector3 m_Offset;
    Vector3 m_Min;
    Vector3 m_Max;
public :
    Collider_Type GetColliderType() const
	{
        return m_ColliderType;
	}
    Vector3 GetMin() const
    {
            return m_Min;
    }
    Vector3 GetMax() const
	{
		return m_Max;
	}
    void SetOffset(const Vector3& Offset)
	{
		m_Offset = Offset;
	}
public :
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render(HDC hDC);
    virtual void PostRender();
    virtual CColliderComponent* Clone();
    virtual void CheckCollision();
public :
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};


