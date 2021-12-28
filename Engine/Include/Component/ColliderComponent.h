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
    Vector3 m_Min; // �ڽ��� ���� �浹������ ���ѱ� ���� Min, Max �� 
    Vector3 m_Max;

    // �ϳ��� �浹ü�� �������� ������ ���� ���� ��찡 �����Ƿ�, �������� �浹���� idx ������ �����صд�. 
    std::vector<int> m_vecSectionIndex;

    // ���� �����ӿ��� �ش� �浹ü�� �浹�� �浹ü ���
    std::list<CColliderComponent*> m_PrevCollisionList;

    // ���� �����ӿ� ���� �������� �浹�� �浹ü ��� : 2���� �浹ü�� �浹������ ���ļ�
    // �浹 Ȯ�� �������� 1Ÿ 4�Ǹ� �����ϱ� ���� �� �ϳ��� �浹���� ������ �浹 ó���� �Ѵ�.
    std::list<CColliderComponent*> m_CurrentCollisionList;

    Collision_Profile* Profile;

    // ���� ���ǿ��� �浹�� �Ǿ����� ���θ� �Ǵ��ϱ� ���� ���� 
    bool m_CurrentSectionCheck;

    // �浹�� ȣ���� �Լ� ��� --> ������ �־��� �� �ְ� ����
    // ��, 3�ܰ�� ���� --> �ش� �Լ� ������ --> �װ͵��� list --> �׷� list ���� �迭 
    std::list<std::function<void(const CollisionResult&)>> m_CollisionListCallback[(int)Collision_State::Max];
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


