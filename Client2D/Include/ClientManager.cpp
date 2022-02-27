#include "ClientManager.h"
#include "Engine.h"
#include "Input.h"
#include "resource.h"
#include "Scene/MainScene.h"
#include "Scene/StartScene.h"
#include "Scene/SceneManager.h"
#include "UI/MouseNormal.h"
#include "UI/MouseAttack.h"
#include "Object/Player2D.h"
#include "Object/TileMap.h"
// SceneComponent
#include "Component/StaticMeshComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/TileMapComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/ParticleComponent.h"
// Object Component
#include "Component/PaperBurnComponent.h"
#include "Component/NavAgent.h"
#include "Component/ObjectComponent.h"


DEFINITION_SINGLE(CClientManager)

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
	CEngine::DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
	                              1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

	CSceneManager::GetInst()->SetCreateSceneModeCallback<CClientManager>(this, &CClientManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectCallback<CClientManager>(this, &CClientManager::CreateObject);
	CSceneManager::GetInst()->SetCreateComponentCallback<CClientManager>(this, &CClientManager::CreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceCallback<CClientManager>(this, &CClientManager::CreateAnimationInstance);


	CInput::GetInst()->CreateKey("MoveUp", 'W');
	CInput::GetInst()->CreateKey("MoveDown", 'S');
	CInput::GetInst()->CreateKey("RotationZInv", 'A');
	CInput::GetInst()->CreateKey("RotationZ", 'D');
	CInput::GetInst()->CreateKey("Attack", VK_SPACE);
	CInput::GetInst()->CreateKey("Attack1", VK_LBUTTON);
	CInput::GetInst()->CreateKey("Skill1", '1');
	CInput::GetInst()->CreateKey("MovePoint", VK_RBUTTON);

	// Sound
	CResourceManager::GetInst()->CreateChannelGroup("UI");

	// Mouse 제작
	CEngine::GetInst()->CreateMouseUIWindow<CMouseNormal>(Mouse_State::Normal, "MouseNormal");
	CEngine::GetInst()->CreateMouseUIWindow<CMouseAttack>(Mouse_State::State1, "MouseNormal");

	return true;
}

void CClientManager::CreateDefaultSceneMode()
{
	// CSceneManager::GetInst()->CreateSceneMode<CStartScene>();
	CSceneManager::GetInst()->CreateSceneMode<CMainScene>();
}

int CClientManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CClientManager::CreateSceneMode(CScene* Scene, size_t Type)
{}

CGameObject* CClientManager::CreateObject(CScene* Scene, size_t Type)
{
	if (Type == typeid(CGameObject).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CGameObject>();

		return Obj;
	}
	if (Type == typeid(CPlayer2D).hash_code())
	{
		CPlayer2D* Obj = Scene->LoadGameObject<CPlayer2D>();

		return Obj;
	}
	if (Type == typeid(CTileMap).hash_code())
	{
		CTileMap* Obj = Scene->LoadGameObject<CTileMap>();

		return Obj;
	}

	return nullptr;
}

CComponent* CClientManager::CreateComponent(CGameObject* Obj, size_t Type)
{
	size_t ComponentTypeId1 = typeid(CSceneComponent).hash_code();
	size_t ComponentTypeId2 = typeid(CStaticMeshComponent).hash_code();
	size_t ComponentTypeId3 = typeid(CSpriteComponent).hash_code();
	size_t ComponentTypeId4 = typeid(CColliderBox2D).hash_code();
	size_t ComponentTypeId5 = typeid(CWidgetComponent).hash_code();
	size_t ComponentTypeId6 = typeid(CCameraComponent).hash_code();
	size_t ComponentTypeId7 = typeid(CNavAgent).hash_code();
	size_t ComponentTypeId8 = typeid(CTileMapComponent).hash_code();

	if (Type == typeid(CSceneComponent).hash_code())
	{
		CSceneComponent* Component = Obj->LoadComponent<CSceneComponent>();

		return Component;
	}
	if (Type == typeid(CStaticMeshComponent).hash_code())
	{
		CStaticMeshComponent* Component = Obj->LoadComponent<CStaticMeshComponent>();

		return Component;
	}
	if (Type == typeid(CSpriteComponent).hash_code())
	{
		CSpriteComponent* Component = Obj->LoadComponent<CSpriteComponent>();

		return Component;
	}
	if (Type == typeid(CColliderBox2D).hash_code())
	{
		CColliderBox2D* Component = Obj->LoadComponent<CColliderBox2D>();

		return Component;
	}
	if (Type == typeid(CColliderCircle).hash_code())
	{
		CColliderCircle* Component = Obj->LoadComponent<CColliderCircle>();

		return Component;
	}
	if (Type == typeid(CColliderPixel).hash_code())
	{
		CColliderPixel* Component = Obj->LoadComponent<CColliderPixel>();

		return Component;
	}
	if (Type == typeid(CWidgetComponent).hash_code())
	{
		CWidgetComponent* Component = Obj->LoadComponent<CWidgetComponent>();

		return Component;
	}
	if (Type == typeid(CCameraComponent).hash_code())
	{
		CCameraComponent* Component = Obj->LoadComponent<CCameraComponent>();

		return Component;
	}
	if (Type == typeid(CNavAgent).hash_code())
	{
		CNavAgent* Component = Obj->LoadComponent<CNavAgent>();

		return Component;
	}
	if (Type == typeid(CTileMapComponent).hash_code())
	{
		CTileMapComponent* Component = Obj->LoadComponent<CTileMapComponent>();

		Component->EnableEditMode(true);

		return Component;
	}
	if (Type == typeid(CParticleComponent).hash_code())
	{
		CParticleComponent* Component = Obj->LoadComponent<CParticleComponent>();

		return Component;
	}
	if (Type == typeid(CPaperBurnComponent).hash_code())
	{
		CPaperBurnComponent* Component = Obj->LoadComponent<CPaperBurnComponent>();

		return Component;
	}
	if (Type == typeid(CNavAgent).hash_code())
	{
		CNavAgent* Component = Obj->LoadComponent<CNavAgent>();

		return Component;
	}

	// 하나라도 매치가 안된다면
	// SceneComponent 로 Load 해본다.
	CSceneComponent* Component = Obj->LoadComponent<CSceneComponent>();
	return Component;
}

void CClientManager::CreateAnimationInstance(CSpriteComponent* Component, const size_t AnimationTypeID)
{
	/*
	if (AnimationTypeID == typeid(CAnimationSequence2DInstance).hash_code())
	{
		Component->LoadAnimationInstance<CAnimationSequence2DInstance>();
	}
	*/
	Component->LoadAnimationInstance<CAnimationSequence2DInstance>();
}
