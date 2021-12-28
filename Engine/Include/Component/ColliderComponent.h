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
    Vector3 m_Min; // 자신이 속한 충돌영역을 구한기 위한 Min, Max 값 
    Vector3 m_Max;

    // 하나의 충돌체가 여러개의 영역에 걸쳐 있을 경우가 있으므로, 여러개의 충돌영역 idx 정보를 저장해둔다. 
    std::vector<int> m_vecSectionIndex;

    // 이전 프레임에서 해당 충돌체와 충돌한 충돌체 목록
    std::list<CColliderComponent*> m_PrevCollisionList;

    // 현재 프레임에 이전 영역에서 충돌한 충돌체 목록 : 2개의 충돌체가 충돌영역이 겹쳐서
    // 충돌 확인 과정에서 1타 4피를 방지하기 위해 단 하나의 충돌영역 에서만 충돌 처리를 한다.
    std::list<CColliderComponent*> m_CurrentCollisionList;

    Collision_Profile* Profile;

    // 현재 섹션에서 충돌이 되었는지 여부를 판단하기 위한 변수 
    bool m_CurrentSectionCheck;

    // 충돌시 호출할 함수 목록 --> 여러개 넣어줄 수 있게 세팅
    // 즉, 3단계로 생각 --> 해당 함수 포인터 --> 그것들의 list --> 그런 list 들의 배열 
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


