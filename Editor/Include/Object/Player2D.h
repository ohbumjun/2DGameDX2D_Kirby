#pragma once
#include "GameObject\LifeObject.h"
#include "Component/SpriteComponent.h"
#include "Component/SceneComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/NavAgent.h"
#include "Component/KirbyState.h"

class CPlayer2D :
    public CLifeObject
{
	friend class CScene;

public :
	CPlayer2D();
	CPlayer2D(const CPlayer2D& Obj);
	virtual ~CPlayer2D() override;

private:
	CSharedPtr<CKirbyState> m_KirbyState;

	CSharedPtr<class CColliderBox2D> m_Body;
	CSharedPtr<class CColliderBox2D> m_PullRightCollider;
	CSharedPtr<class CColliderBox2D> m_PullLeftCollider;

	CSharedPtr<class CCameraComponent> m_Camera;

	CSharedPtr<CWidgetComponent> m_SimpleHUDWidget;

	CSharedPtr<CNavAgent> m_NavAgent;

	bool  m_SolW;
	float m_WDistance;
	float m_Opacity;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPlayer2D* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

