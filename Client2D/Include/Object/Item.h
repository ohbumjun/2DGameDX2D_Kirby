#pragma once
#include "GameObject\GameObject.h"

class CItem :
    public CGameObject
{
    friend class CScene;
protected:
	CItem();
	CItem(const CItem& Monster);
	virtual ~CItem() = 0;
protected:
	Item_Type m_ItemType;
	CSharedPtr<class CColliderCircle> m_ColliderBody;
public:
	virtual void   Start() override;
	virtual bool   Init() override;
	virtual void   Update(float DeltaTime);
	virtual void   PostUpdate(float DeltaTime);
public:
	virtual void Load(FILE* pFile) override;
private :
	void CollisionPlayerCallback(const CollisionResult& Result);
};

