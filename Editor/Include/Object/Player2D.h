#pragma once
#include "GameObject\GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/SceneComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/NavAgent.h"

class CPlayer2D :
    public CGameObject
{
	friend class CScene;

public :
	CPlayer2D();
	CPlayer2D(const CPlayer2D& Obj);
	virtual ~CPlayer2D() override;

private:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CSpriteComponent> m_ChildLeftSprite;
	CSharedPtr<CSpriteComponent> m_ChildRightSprite;
private :
	CSharedPtr<CSceneComponent>  m_ChildLeftMuzzle;
	CSharedPtr<CSceneComponent>  m_ChildRightMuzzle;
	CSharedPtr<CSceneComponent>  m_ChildRoot;
	CSharedPtr<CSceneComponent>  m_Muzzle;
private :
	CSharedPtr<CSpriteComponent> m_Child1Sprite;
	CSharedPtr<CSpriteComponent> m_Child2Sprite;
	CSharedPtr<CSpriteComponent> m_Child3Sprite;
	CSharedPtr<CSpriteComponent> m_Child4Sprite;
private :
	CSharedPtr<class CColliderBox2D> m_Body;
	CSharedPtr<class CCameraComponent> m_Camera;
private :
	CSharedPtr<CWidgetComponent> m_SimpleHUDWidget;
private :
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

