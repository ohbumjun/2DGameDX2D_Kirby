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
    Vector3 m_Min; // �ڽ��� ���� �浹������ ���ѱ� ���� Min, Max �� 
    Vector3 m_Max;

    // �ϳ��� �浹ü�� �������� ������ ���� ���� ��찡 �����Ƿ�, �������� �浹���� idx ������ �����صд�. 
    std::vector<int> m_vecSectionIndex;

    // ���� �����ӿ��� �ش� �浹ü�� �浹�� �浹ü ���
    // �浹ü���� �������� ���� üũ�ϱ� ���ؼ���
    // �������� �浹�Ǿ� �־����� �Ǵ��� �� �־�� �Ѵ�
    // ��, �������� �浹�� �Ǿ� �־����� �˰� �־�� �Ѵ�
    // ����, �浹�� �Ǹ�, �� ������ �浹ü���� �ְ�
    // �������� �Ǹ�, �̶� �浹 ��Ͽ��� ���� �����̴�
    // ���� ���ؼ�, �������� ���� �Ǵ��ϱ� ���� ������ �����̴�.
    std::list<CColliderComponent*> m_PrevCollisionList;

    // ���� �����ӿ� ���� �������� �浹�� �浹ü ��� : 2���� �浹ü�� �浹������ ���ļ�
    // �浹 Ȯ�� �������� 1Ÿ 4�Ǹ� �����ϱ� ���� �� �ϳ��� �浹���� ������ �浹 ó���� �Ѵ�.
    std::list<CColliderComponent*> m_CurrentCollisionList;

    Collision_Profile* m_Profile;

    // ���� ���ǿ��� �浹�� �Ǿ����� ���θ� �Ǵ��ϱ� ���� ����
    // ���� ���ؼ� ���� �浹ü�� �ߺ��Ǿ� SceneCollision�� m_ColliderList�� ������� ���� �ִ�
    // (���� �ڵ�����δ� �׷��� ������ Ȥ�ó� �Ͽ�)
    // ���� �ߺ�ó���� �����ϱ� ���� �����ϴ� �����̴�. 
    bool m_CurrentSectionCheck;

    // �浹�� ȣ���� �Լ� ��� --> ������ �־��� �� �ְ� ����
    // ��, 3�ܰ�� ���� --> �ش� �Լ� ������ --> �װ͵��� list --> �׷� list ���� �迭 
    std::list<std::function<void(const CollisionResult&)>> m_CollisionListCallback[(int)Collision_State::Max];

    // ���콺 �浹�� ȣ���� �Լ� ���
    std::list<std::function<void(const CollisionResult&)>> m_CollisionMouseCallback[(int)Collision_State::Max];

    // Mouse �� �浹 ������, �ٸ� Object�� �浹�������� �������ִ� bool ���� 
    bool m_MouseCollision;

    // �浹 ��� �����ϴ� ����
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
    // �� �����Ӹ��� ���� �浹 ������ ����� �߰��ϴ� �Լ�
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
    // Profile ���� �����ϱ� 
    void SetCollisionProfile(const std::string& Name);
    // ������ �浹�ߴ� �浹ü��� ���絵 �浹�������� �Ǵ��ϴ� �Լ�
	void CheckPrevColliderSection();
    // ���� �浹 ��Ͽ� �߰��ϱ� 
    void AddPrevCollision(CColliderComponent* Collider);
    // ���� �浹 ��Ͽ��� �����ϱ� 
    void DeletePrevCollision(CColliderComponent* Collider);
    // ���� �浹 ����� ������� �Ǵ��ϱ� 
    bool EmptyPrevCollision();
    // ���� �浹 ��Ͽ� �����ϴ��� �Ǵ��ϱ� 
    bool CheckPrevCollision(CColliderComponent* Collider);
    // ���� ������ �浹 ��Ͽ� �����ϴ°� 
    bool CheckCurrentFrameCollision(CColliderComponent* Collider);
    // ���� ������ �浹 ��Ͽ� �߰��ϱ� 
    void AddCurrentFrameCollision(CColliderComponent* Collider);
    // �浹 �ݹ� ȣ�� 
    void CallCollisionCallback(Collision_State State);
    // ���콺 �浹 �ݹ� ȣ�� 
    void CallCollisionMouseCallback(Collision_State State);
    // �ʱ�ȭ 
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


