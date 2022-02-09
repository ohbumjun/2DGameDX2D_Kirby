#include "SpriteCameraObject.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"

CSpriteCameraObject::CSpriteCameraObject()
{}

CSpriteCameraObject::~CSpriteCameraObject()
{}

bool CSpriteCameraObject::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Camera = CreateComponent<CCameraComponent>("Camera");
	SetRootComponent(m_Camera);

	m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);


	return true;
}
