#pragma once

#include "Component/SpriteComponent.h"
#include "GameObject/GameObject.h"
#include "Component/WidgetComponent.h"

class CPlayer2D :
	public CGameObject
{
	friend class CScene;

protected:
	CPlayer2D();
	CPlayer2D(const CPlayer2D& obj);
	virtual ~CPlayer2D() override;

private:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CSpriteComponent> m_ChildLeftSprite;
	CSharedPtr<CSpriteComponent> m_ChildRightSprite;
	CSharedPtr<CSceneComponent>  m_ChildLeftMuzzle;
	CSharedPtr<CSceneComponent>  m_ChildRightMuzzle;
	CSharedPtr<CSceneComponent>  m_ChildRoot;
	CSharedPtr<CSceneComponent>  m_Muzzle;

	CSharedPtr<CSpriteComponent> m_Child1Sprite;
	CSharedPtr<CSpriteComponent> m_Child2Sprite;
	CSharedPtr<CSpriteComponent> m_Child3Sprite;
	CSharedPtr<CSpriteComponent> m_Child4Sprite;

	CSharedPtr<class CColliderBox2D> m_Body;
	CSharedPtr<class CCameraComponent> m_Camera;

	CSharedPtr<CWidgetComponent> m_SimpleHUDWidget;

	bool  m_SolW;
	float m_WDistance;
	float m_Opacity;

public:
	virtual bool       Init() override;
	virtual void       Update(float DeltaTime) override;
	virtual void       PostUpdate(float DeltaTime) override;
	virtual CPlayer2D* Clone() override;

private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void RotationZInv(float DeltaTime);
	void RotationZ(float DeltaTime);
	void Attack(float DeltaTime);
	void Attack1(float DeltaTime);
	void Skill1(float DeltaTime);
};
