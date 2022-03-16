#include "ClientManager.h"
#include "Engine.h"
#include "Input.h"
#include "resource.h"
// Scene
#include "Scene/MainScene.h"
#include "Scene/StartScene.h"
#include "Scene/SceneManager.h"
// UI
#include "UI/MouseNormal.h"
#include "UI/MouseAttack.h"
// Object 
#include "Object/Player2D.h"
#include "Object/TileMap.h"
#include "Object/YellowBird.h"
#include "Object/NormalBear.h"
#include "Object/MushRoom.h"
#include "Object/PurpleBeatle.h"
#include "Object/TileMapEmpty.h"
#include "Object/BackGround.h"
#include "Object/BeamMonster.h"
#include "Object/FireMonster.h"
#include "Object/FightMonster.h"
#include "Object/LineContainer.h"
#include "GameObject/Line.h"
#include "Object/EffectSceneChangeStar.h"
#include "Object/EffectChangeToGreen2.h"
#include "Object/EffectChangeToGreen1.h"

// SceneComponent
#include "Component/StaticMeshComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/TileMapComponent.h"
#include "Component/BackGroundComponent.h"
#include "Component/TileEmptyComponent.h"
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
	// CInput::GetInst()->CreateKey("MoveDown", 'S');
	// CInput::GetInst()->CreateKey("RotationZInv", 'A');
	// CInput::GetInst()->CreateKey("RotationZ", 'D');
	CInput::GetInst()->CreateKey("MoveRight", 'D');
	CInput::GetInst()->CreateKey("MoveDashRight", 'D');
	CInput::GetInst()->SetCtrlKey("MoveDashRight", true);

	CInput::GetInst()->CreateKey("MoveLeft", 'A');
	CInput::GetInst()->CreateKey("MoveDashLeft", 'A');
	CInput::GetInst()->SetCtrlKey("MoveDashLeft", true);

	CInput::GetInst()->CreateKey("PullRight", 'D');
	CInput::GetInst()->SetShiftKey("PullRight", true);
	CInput::GetInst()->CreateKey("PullLeft", 'A');
	CInput::GetInst()->SetShiftKey("PullLeft", true);

	CInput::GetInst()->CreateKey("SpitOut", 'F');

	CInput::GetInst()->CreateKey("SpecialChange", 'S');
	CInput::GetInst()->SetShiftKey("SpecialChange", true);

	// CInput::GetInst()->CreateKey("Attack", VK_SPACE);
	CInput::GetInst()->CreateKey("Jump", VK_SPACE);
	CInput::GetInst()->CreateKey("JumpWhileDash", VK_SPACE);
	CInput::GetInst()->SetCtrlKey("JumpWhileDash", true);

	CInput::GetInst()->CreateKey("JumpDown", 'S');
	CInput::GetInst()->SetCtrlKey("JumpDown", true);


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
	// CSceneManager::GetInst()->CreateSceneMode<CMainScene>();

	CSceneManager::GetInst()->CreateSceneModeEmpty<CMainScene>();
	CSceneManager::GetInst()->GetScene()->PrepareResources();

	/*
	// CSceneManager::GetInst()->GetScene()->Load("TestTileMapSceneWithPlayerWithBackTestCamera.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("TestTileMapSceneWithPlayerManyMonster.scn", SCENE_PATH);
	CSceneManager::GetInst()->GetScene()->Load("TestTileMapSceneWithPlayerManyMonsterMushRoom.scn", SCENE_PATH);
	*/

	// CSceneManager::GetInst()->GetScene()->Load("Green1_OnlyBear.scn", SCENE_PATH);
	CSceneManager::GetInst()->GetScene()->Load("Green1.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Green2_SpecialScene.scn", SCENE_PATH);
}

int CClientManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CClientManager::CreateSceneMode(CScene* Scene, size_t Type)
{}

CGameObject* CClientManager::CreateObject(CScene* Scene, size_t GameObjectTypeID)
{
	if (GameObjectTypeID == typeid(CGameObject).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CGameObject>();

		return Obj;
	}
	else if (GameObjectTypeID == typeid(CPlayer2D).hash_code())
	{
		// 

		CGameObject* Obj = Scene->LoadGameObject<CPlayer2D>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CYellowBird).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CYellowBird>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CPurpleBeatle).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CPurpleBeatle>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CNormalBear).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CNormalBear>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CMushRoom).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CMushRoom>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CBeamMonster).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CBeamMonster>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CFireMonster).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CFireMonster>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CFightMonster).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CFightMonster>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CTileMap).hash_code())
	{
		CTileMap* Obj = Scene->LoadGameObject<CTileMap>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CTileMapEmpty).hash_code())
	{
		CTileMapEmpty* Obj = Scene->LoadGameObject<CTileMapEmpty>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CBackGround).hash_code())
	{
		CBackGround* Obj = Scene->LoadGameObject<CBackGround>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CLine).hash_code())
	{
		CLine* Obj = Scene->LoadGameObject<CLine>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CLineContainer).hash_code())
	{
		CLineContainer* Obj = Scene->LoadGameObject<CLineContainer>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectSceneChangeStar).hash_code())
	{
		CEffectSceneChangeStar* Obj = Scene->LoadGameObject<CEffectSceneChangeStar>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectChangeToGreen2).hash_code())
	{
		CEffectChangeToGreen2* Obj = Scene->LoadGameObject<CEffectChangeToGreen2>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectChangeToGreen1).hash_code())
	{
		CEffectChangeToGreen1* Obj = Scene->LoadGameObject<CEffectChangeToGreen1>();
		return Obj;
	}

	return nullptr;
}

CComponent* CClientManager::CreateComponent(CGameObject* Obj, size_t Type)
{

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
	if (Type == typeid(CTileEmptyComponent).hash_code())
	{
		CTileEmptyComponent* Component = Obj->LoadComponent<CTileEmptyComponent>();

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
	if (Type == typeid(CBackGroundComponent).hash_code())
	{
		CBackGroundComponent* Component = Obj->LoadComponent<CBackGroundComponent>();

		return Component;
	}

	// 하나라도 매치가 안된다면
	// SceneComponent 로 Load 해본다.
	// CSceneComponent* Component = Obj->LoadComponent<CSceneComponent>();
	return nullptr;
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
