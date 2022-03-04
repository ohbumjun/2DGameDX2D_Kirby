#pragma once

#include "Component/SpriteComponent.h"
#include "GameObject/LifeObject.h"
#include "Component/WidgetComponent.h"
#include "Component/NavAgent.h"

class CPlayer2D :
	public CLifeObject
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

	CSharedPtr<CNavAgent> m_NavAgent;

	bool  m_SolW;
	float m_WDistance;
	float m_Opacity;
private :
	// 전체 움직임 속도 ( 레버 + 대쉬 )
	float m_MoveVelocity;
	float m_MoveVelocityMax;

	bool m_IsLeverMoving;
	float m_LeverMoveAccel;
	float m_LeverVelocity;
	float m_LeverMaxMoveVelocity;

	bool m_IsDashMoving;
	float m_DashMoveAccel;
	float m_DashVelocity;
	float m_DashMaxMoveVelocity;

	bool m_RightMove;
	float m_ToLeftWhenRightMove;
	bool m_RightMovePush;

	bool m_LeftMove;
	bool m_ToRightWhenLeftMove;
	bool m_LeftMovePush;

	bool m_TriangleJump;

public:
	virtual bool       Init() override;
	virtual void		Start() override;
	virtual void       Update(float DeltaTime) override;
	virtual void       PostUpdate(float DeltaTime) override;
	virtual CPlayer2D* Clone() override;
private : // 화면 밖으로 안나가게 하기
	void CheckOutsideWorldResolution();
private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
private :
	void MoveLeftW(float DeltaTime);
	void MoveDashLeft(float DeltaTime);
	void MoveRightW(float DeltaTime);
	void MoveDashRight(float DeltaTime);
	void LeftLeverMoveEnd(float DeltaTime);
	void RightLeverMoveEnd(float DeltaTime);
	void LeftDashMoveEnd(float DeltaTime);
	void RightDashMoveEnd(float DeltaTime);
private :
	float CalculateLeverMoveSpeed(float DeltaTime);
	float CalculateDashMoveSpeed(float DeltaTime);
	float CalculateTotalMoveSpeed(float DeltaTime);
	void PlayerMoveUpdate(float DeltaTime);
	void ResetMoveInfo();
private :
	void RotationZInv(float DeltaTime);
	void RotationZ(float DeltaTime);
private :
	void Jump(float DeltaTime);
private :
	void Attack(float DeltaTime);
	void Attack1(float DeltaTime);
	void Skill1(float DeltaTime);
	void MovePointDown(float DeltaTime);
};
