#pragma once

#include "../GameInfo.h"

class CCollisionSection
{
	friend class CSceneCollision;
private :
	CCollisionSection();
	~CCollisionSection();
private :
	// 모든 충돌체들은 실질적으로 이 영역에 속한 충돌체들을 들고 있어야 한다.
	std::vector<class CColliderComponent*> m_vecCollider;
	Vector3 m_SectionSize;
	Vector3 m_SectionTotatlSize; // 자기가 속한 SceneCollision의 전체 크기에 대한 정보를 지니고 있게 한다
	Vector3 m_Min;
	Vector3 m_Max;
	int m_IndexX; // SceneCollision 상의 Idx
	int m_IndexY;
	int m_IndexZ;
	int m_Index;
public :
	void Init(int IndexX, int IndexY, int IndexZ, int Index,
		const Vector3& Min, const Vector3& Max,
		const Vector3& SectionSize,
		const Vector3& SectionTotalSize);
	void Clear();
	void AddCollider(class CColliderComponent* Collider);
	void Collision(float DeltaTime);
	CColliderComponent* CollisionMouse(bool Is2D, float DeltaTime);
public :
	static int SortY(const void* Src, const void* Dest);
};



