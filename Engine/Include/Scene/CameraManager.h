#pragma once

#include "../GameInfo.h"
#include "../Component/CameraComponent.h"

class CCameraManager
{
	friend class CScene;
private :
	CCameraManager();
	~CCameraManager();

private :
	class CScene* m_Scene;
private :
	// 기본적으로 메인 카메라를 반드시 만들어주고
	// UI용 카메라도 반드시 만들어줄 것이다
	// UI용 카메라는 고정해서 사용하고, 메인 카메라는 그때그때
	// 바꿔서 사용할 것이다 . 다양한 효과를 주기 위해
	CSharedPtr<CCameraComponent> m_CurrentCamera;
	// 카메라 정보를 유지하는 변수
	CSharedPtr<CCameraComponent> m_KeepCamera;
	CSharedPtr<CCameraComponent> m_UICamera;
public :
	CCameraComponent* GetCurrentCamera() const
{
		return m_CurrentCamera;
}
	CCameraComponent* GetUICamera() const
	{
		return m_CurrentCamera;
	}
	void SetCurrentCamera(CCameraComponent* Camera)
{
		m_CurrentCamera = Camera;
}
	void KeepCamera(CCameraComponent* Camera)
{
		m_KeepCamera = Camera;
}
	void ReturnCamera()
{
		m_CurrentCamera = m_KeepCamera;
		m_KeepCamera = nullptr;
}
public :
		void Start();
		void Init();
		void Update(float DeltaTime);
		void PostUpdate(float DeltaTime);
};

