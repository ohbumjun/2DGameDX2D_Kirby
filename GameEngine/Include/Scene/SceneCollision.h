#pragma once

#include "CollisionSection.h"

struct CollisionSectionInfo
{
	// 하나의 Scene 에서 여러개의 Collision Section을 들고 있게 세팅할 것이다. 
	std::vector<class CCollisionSection*> vecSection;
	Vector3 SectionSize; // 영역 1칸의 크기
	Vector3 Center; // 전체 영역에서의 중심
	Vector3 SectionTotalSize; // 전체 영역 크기
	Vector3 Min; // 전체 영역에서의 최소값
	Vector3 Max; // 전체 영역에서의 최대값
	int CountX;
	int CountY;
	int CountZ;

	CollisionSectionInfo() :
		CountX(1),
		CountY(1),
		CountZ(1),
		SectionSize{ 1000.f, 1000.f, 1.f },
		SectionTotalSize{ 1000.f, 1000.f, 1.f },
		Min{ 0.f, 0.f, -0.5f },
		Max{ 500.f, 500.f, 0.5f }
	{
		
	}
};

class CSceneCollision
{
	friend class CScene;
private :
	CSceneCollision();
	~CSceneCollision();
private:
	class CScene* m_Scene;
private :
	CollisionSectionInfo* m_Section;
	// CSceneCollision 내의 전체 Section의 모 ~~ 든 충돌체 목록을 지니고 있게 한다.
	std::vector<class CColliderComponent*> m_vecCollider;
	class CColliderComponent* m_MouseCollision;
public :
	void Start();
	bool Init();
	void Collision(float DeltaTime);
	void CollisionMouse(float DelatTime);
public :
	void SetSectionSize(const Vector3& Size);
	void SetSectionSize(float x, float y, float z = 1.f);
	void SetSectionCenter(const Vector3& Size);
	void SetSectionCenter(float x, float y, float z = 1.f);
	void SetSectionMin(const Vector3& Size);
	void SetSectionMin(float x, float y, float z = 1.f);
	void SetSectionMax(const Vector3& Size);
	void SetSectionMax(float x, float y, float z = 1.f);
	void SetSectionCount(int CountX, int CountY, int CountZ = 1);
	void CreateSection();
	void Clear();
	void AddCollider(class CColliderComponent* Collider);
private :
	void CheckColliderSection();
};


