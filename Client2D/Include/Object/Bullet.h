#pragma once

#include "GameObject/GameObject.h"

class CBullet :
	public CGameObject
{
	friend class CScene;

protected:
	CBullet();
	CBullet(const CBullet& obj);
	virtual ~CBullet() override;

private:
	CSharedPtr<class CSpriteComponent> m_Sprite;
	CSharedPtr<class CColliderCircle> m_Body;
	float                              m_Distance;

public:
	virtual bool     Init() override;
	virtual void     Update(float DeltaTime) override;
	virtual void     PostUpdate(float DeltaTime) override;
	virtual CBullet* Clone() override;
public :
	void BulletCollision(const CollisionResult& Result);
	void SetCollisionProfile(const std::string& Name);
};
