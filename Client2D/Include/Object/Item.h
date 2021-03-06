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
	CSharedPtr<class CSpriteComponent> m_Sprite;
	CSharedPtr<class CColliderCircle> m_ColliderBody;
protected :
	float m_ToggleCurTime;
	float m_ToggleCurTimeMax;
	bool m_ToggleUp;
public:
	virtual void   Start() override;
	virtual bool   Init() override;
	virtual void   Update(float DeltaTime);
	virtual void   PostUpdate(float DeltaTime);
public:
	virtual void Load(FILE* pFile) override;
private :
	void UpdateToggleTime(float DeltaTime);
	void CollisionPlayerCallback(const CollisionResult& Result);
};

