#pragma once

#include "GameObject/GameObject.h"

class CBulletCamera : public CGameObject {
	friend class CScene;
public :
	CBulletCamera();
	CBulletCamera(const CBulletCamera& Camera);
	virtual ~CBulletCamera() override;
private :
	float m_Distance;
	CSharedPtr<class CSpriteComponent> m_Sprite;
	CSharedPtr<class CCameraComponent> m_Camera;
	CSharedPtr<class CColliderBox2D> m_Body;
public :
	virtual void Start() override;
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
public:
	virtual void CollisionCallback(const CollisionResult& Result);
};