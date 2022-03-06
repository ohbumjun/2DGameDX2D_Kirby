#pragma once
#include "SceneComponent.h"

class CColliderComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CCollision;
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
    // 충돌체끼리 떨어지는 것을 체크하기 위해서는
    // 기존까지 충돌되어 있었음을 판단할 수 있어야 한다
    // 즉, 이전까지 충돌이 되어 있었음을 알고 있어야 한다
    // 따라서, 충돌이 되면, 이 변수에 충돌체들을 넣고
    // 떨어지게 되면, 이때 충돌 목록에서 빼는 원리이다
    // 쉽게 말해서, 떨어지는 것을 판단하기 위해 세팅한 변수이다.
    std::list<CColliderComponent*> m_PrevCollisionList;

    // 현재 프레임에 이전 영역에서 충돌한 충돌체 목록 : 2개의 충돌체가 충돌영역이 겹쳐서
    // 충돌 확인 과정에서 1타 4피를 방지하기 위해 단 하나의 충돌영역 에서만 충돌 처리를 한다.
    std::list<CColliderComponent*> m_CurrentCollisionList;

    Collision_Profile* m_Profile;

    // 현재 섹션에서 충돌이 되었는지 여부를 판단하기 위한 변수
    // 쉽게 말해서 같은 충돌체가 중복되어 SceneCollision의 m_ColliderList에 들어있을 수도 있다
    // (물론 코드상으로는 그렇지 않으나 혹시나 하여)
    // 따라서 중복처리르 방지하기 위해 세팅하는 변수이다. 
    bool m_CurrentSectionCheck;

    // 충돌시 호출할 함수 목록 --> 여러개 넣어줄 수 있게 세팅
    // 즉, 3단계로 생각 --> 해당 함수 포인터 --> 그것들의 list --> 그런 list 들의 배열 
    std::list<std::function<void(const CollisionResult&)>> m_CollisionListCallback[(int)Collision_State::Max];

    // 마우스 충돌시 호출할 함수 목록
    std::list<std::function<void(const CollisionResult&)>> m_CollisionMouseCallback[(int)Collision_State::Max];

    // Mouse 와 충돌 중인지, 다른 Object와 충돌중인지를 구별해주는 bool 변수 
    bool m_MouseCollision;

    // 충돌 결과 저장하는 변수
    CollisionResult m_Result;
    CollisionResult m_MouseResult;

    // Mesh
    class CMesh* m_Mesh;
    // Shader
    class CShader* m_Shader;
    // CBuffer
    class CColliderConstantBuffer* m_CBuffer;

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
    // 매 프레임마다 속한 충돌 영역의 목록을 추가하는 함수
    void AddSectionIndex(int SectionIndex)
	{
		m_vecSectionIndex.push_back(SectionIndex);
	}
    CollisionResult GetCurrentResult() const
	{
        return m_Result;
	}
    void CheckCurrentSection()
	{
        m_CurrentSectionCheck = true;
	}
    bool GetCurrentSectionCheck() const
	{
        return m_CurrentSectionCheck;
	}
    Collision_Profile* GetCurrentProfile() const
	{
        return m_Profile;
	}
public :
    // Profile 정보 세팅하기 
    void SetCollisionProfile(const std::string& Name);
    // 이전에 충돌했던 충돌체들과 현재도 충돌중인지를 판단하는 함수
	void CheckPrevColliderSection();
    // 이전 충돌 목록에 추가하기 
    void AddPrevCollision(CColliderComponent* Collider);
    // 이전 충돌 목록에서 제거하기 
    void DeletePrevCollision(CColliderComponent* Collider);
    // 이전 충돌 목록이 비었는지 판단하기 
    bool EmptyPrevCollision();
    // 이전 충돌 목록에 존재하는지 판단하기 
    bool CheckPrevCollision(CColliderComponent* Collider);
    // 현재 프레임 충돌 목록에 존재하는가 
    bool CheckCurrentFrameCollision(CColliderComponent* Collider);
    // 현재 프레임 충돌 목록에 추가하기 
    void AddCurrentFrameCollision(CColliderComponent* Collider);
    // 충돌 콜백 호출 
    void CallCollisionCallback(Collision_State State);
    // 마우스 충돌 콜백 호출 
    void CallCollisionMouseCallback(Collision_State State);
    // 초기화 
    void ClearFrame();
public :
    bool CheckIsInSameCollisionSection(CColliderComponent* Collider);
public :
    void SetColliderColor(float r, float g, float b, float a);
    virtual void Destroy() override;
public :
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CColliderComponent* Clone();
    virtual void CheckCollision();
public :
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
public :
    virtual bool Collision(CColliderComponent* Dest) = 0;
    virtual bool CollisionMouse(const Vector2& MousePos) = 0;
public :
    template<typename T>
    void AddCollisionCallback(Collision_State State, T* Obj, void (T::*Func)(const CollisionResult&))
	{
        m_CollisionListCallback[(int)State].push_back(std::bind(Func, Obj, std::placeholders::_1));
	}
    template<typename T>
    void AddCollisionMouseCallback(Collision_State State, T* Obj, void (T::* Func)(const CollisionResult&))
    {
        m_CollisionMouseCallback[(int)State].push_back(std::bind(Func, Obj, std::placeholders::_1));
    }
};


