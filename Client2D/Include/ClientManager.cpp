#include "ClientManager.h"
#include "Engine.h"
#include "Input.h"
#include "resource.h"
// Scene
#include "Scene/MainScene.h"
#include "Scene/IntroScene.h"
#include "Scene/EndingScene.h"
#include "Scene/BeginningScene.h"
#include "Scene/Green3Scene.h"
#include "Scene/Green4Scene.h"
#include "Scene/Green5Scene.h"
#include "Scene/StartScene.h"
#include "Scene/DynaBlade1.h"
#include "Scene/DynaBlade2.h"
#include "Scene/Float1Scene.h"
#include "Scene/Float4Scene.h"
#include "Scene/Float3Scene.h"
#include "Scene/Float5Scene.h"
#include "Scene/Float6Scene.h"
#include "Scene/Float7Scene.h"
#include "Scene/Float2_1Scene.h"
#include "Scene/Float2_2Scene.h"
#include "Scene/SceneManager.h"
// Excel
#include "Excel/Excel.h"
// UI
#include "UI/MouseNormal.h"
#include "UI/MouseAttack.h"
// Object 
#include "Object/Player2D.h"
#include "Object/TileMap.h"
#include "Object/YellowBird.h"
#include "Object/NormalBear.h"
#include "Object/MushRoom.h"
#include "Object/Kabu.h"
#include "Object/Fish.h"
#include "Object/Pig.h"
#include "Object/Frog.h"
#include "Object/Scary.h"
#include "Object/Chicken.h"
#include "Object/PurpleBeatle.h"
#include "Object/TileMapEmpty.h"
#include "Object/BackGround.h"
#include "Object/BeamMonster.h"
#include "Object/FireMonster.h"
#include "Object/BombMonster.h"
#include "Object/FightMonster.h"
#include "Object/SwordMonster.h"
#include "Object/LineContainer.h"
#include "Object/Block.h"
#include "Object/Awl.h"
#include "Object/Ladder.h"
#include "GameObject/Line.h"
#include "Object/HPYellowItem.h"
#include "Object/HPGreenItem.h"
#include "Object/HPRedItem.h"
#include "Object/MiddleBossHammer.h"
#include "Object/MiddleBossScissorBug.h"
#include "Object/BossTree.h"
#include "Object/BossPenguin.h"
#include "Object/BossDyna.h"
#include "Object/WaterFloat1.h"
#include "Object/WaterFloat2_2.h"
#include "Object/Squid.h"
#include "Object/EffectSceneChangeStar.h"
#include "Object/EffectChangeToGreen2.h"
#include "Object/EffectChangeToGreen1.h"
#include "Object/EffectChangeToGreen4.h"
#include "Object/EffectChangeToGreen5.h"
#include "Object/EffectChangeToFloat3.h"
#include "Object/EffectChangeToFloat4.h"
#include "Object/EffectChangeToFloat2_1.h"
#include "Object/EffectChangeToFloat2_2.h"
#include "Object/EffectChangeToFloat4.h"
#include "Object/EffectChangeToFloat6.h"
#include "Object/EffectChangeToFloat7.h"
#include "Object/EffectChangeToDyna2.h"
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

CClientManager::CClientManager() : m_RandomEngine((std::random_device())())
{
}

CClientManager::~CClientManager()
{
	CEngine::DestroyInst();

	// Excel에 현재 정보 저장
	CExcel::GetInst()->SaveExcel();
	CExcel::GetInst()->DestroyInst();
}

float CClientManager::GenerateRandomNumberFrom0To1()
{
	std::uniform_real_distribution<float> Dis(0.f, 1.f);

	return Dis(m_RandomEngine);
}

bool CClientManager::Init(HINSTANCE hInst)
{
	// Excel 파일 읽어오기
	CExcel::GetInst()->Init();
	CExcel::GetInst()->LoadExcel();

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

	CInput::GetInst()->CreateKey("SpitOut", 'Q');

	CInput::GetInst()->CreateKey("SpecialChange", '1');
	// CInput::GetInst()->SetShiftKey("SpecialChange", true);

	CInput::GetInst()->CreateKey("Attack", 'F');
	CInput::GetInst()->CreateKey("SlideAttack", 'C');
	CInput::GetInst()->CreateKey("SpecialAttack", 'T');
	CInput::GetInst()->CreateKey("UltimateAttack", 'G');

	CInput::GetInst()->CreateKey("Jump", VK_SPACE);
	CInput::GetInst()->CreateKey("JumpWhileDash", VK_SPACE);
	CInput::GetInst()->SetCtrlKey("JumpWhileDash", true);

	CInput::GetInst()->CreateKey("JumpDown", 'S');
	CInput::GetInst()->SetCtrlKey("JumpDown", true);

	// CInput::GetInst()->CreateKey("Skill1", '1');
	CInput::GetInst()->CreateKey("Attack1", VK_LBUTTON);
	CInput::GetInst()->CreateKey("MovePoint", VK_RBUTTON);

	// Sound
	CResourceManager::GetInst()->CreateChannelGroup("UI");
	CResourceManager::GetInst()->CreateChannelGroup("PlayerEffect");
	CResourceManager::GetInst()->CreateChannelGroup("Monster");
	CResourceManager::GetInst()->CreateChannelGroup("Scene");
	CResourceManager::GetInst()->CreateChannelGroup("BGM");

	// Mouse 제작
	CEngine::GetInst()->CreateMouseUIWindow<CMouseNormal>(Mouse_State::Normal, "MouseNormal");
	CEngine::GetInst()->CreateMouseUIWindow<CMouseAttack>(Mouse_State::State1, "MouseNormal");

	// Random Number 생성을 위한 세팅

	return true;
}

void CClientManager::CreateDefaultSceneMode()
{
	// CSceneManager::GetInst()->CreateSceneMode<CIntroScene>();
	// CSceneManager::GetInst()->CreateSceneMode<CEndingScene>();
	// ----------------------------------------------------------------------------

	// CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat1Scene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat2_1Scene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat2_2Scene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CMainScene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat4Scene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat7Scene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat6Scene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat5Scene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat3Scene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CGreen3Scene>();
	CSceneManager::GetInst()->CreateSceneModeEmpty<CGreen4Scene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CGreen5Scene>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CDynaBlade1>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CDynaBlade2>();
	// CSceneManager::GetInst()->CreateSceneModeEmpty<CBeginningScene>();

	CSceneManager::GetInst()->GetScene()->PrepareResources();

	// CSceneManager::GetInst()->GetScene()->Load("Dyna1.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Dyna2_FinalBoss.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Float2_2.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Float1.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Float2_1.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Green1_FirstSpecial.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Green1_OnlyBear.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Float4_MiddleBoss.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Float7_SecondBoss.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Float5_FourthSpecial.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Float6.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Float3.scn", SCENE_PATH); 
	// CSceneManager::GetInst()->GetScene()->Load("Green3_MiddleBoss_SecondSpecial.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Green5.scn", SCENE_PATH);
	CSceneManager::GetInst()->GetScene()->Load("Green4.scn", SCENE_PATH);
	// CSceneManager::GetInst()->GetScene()->Load("Beginning.scn", SCENE_PATH);
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
	else if (GameObjectTypeID == typeid(CPig).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CPig>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CFish).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CFish>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CFrog).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CFrog>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CChicken).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CChicken>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CSquid).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CSquid>();
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
	else if (GameObjectTypeID == typeid(CMiddleBossHammer).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CMiddleBossHammer>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CMiddleBossScissorBug).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CMiddleBossScissorBug>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CBossTree).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CBossTree>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CBossPenguin).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CBossPenguin>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CBossDyna).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CBossDyna>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CHPYellowItem).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CHPYellowItem>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CHPGreenItem).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CHPGreenItem>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CHPRedItem).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CHPRedItem>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CFightMonster).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CFightMonster>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CBombMonster).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CBombMonster>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CSwordMonster).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CSwordMonster>();
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
	if (GameObjectTypeID == typeid(CBlock).hash_code())
	{
		CBlock* Obj = Scene->LoadGameObject<CBlock>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CAwl).hash_code())
	{
		CAwl* Obj = Scene->LoadGameObject<CAwl>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CKabu).hash_code())
	{
		CKabu* Obj = Scene->LoadGameObject<CKabu>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CScary).hash_code())
	{
		CScary* Obj = Scene->LoadGameObject<CScary>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CLadder).hash_code())
	{
		CLadder* Obj = Scene->LoadGameObject<CLadder>();
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
	if (GameObjectTypeID == typeid(CEffectChangeToGreen4).hash_code())
	{
		CEffectChangeToGreen4* Obj = Scene->LoadGameObject<CEffectChangeToGreen4>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectChangeToGreen5).hash_code())
	{
		CEffectChangeToGreen5* Obj = Scene->LoadGameObject<CEffectChangeToGreen5>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectChangeToFloat2_1).hash_code())
	{
		CEffectChangeToFloat2_1* Obj = Scene->LoadGameObject<CEffectChangeToFloat2_1>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectChangeToFloat2_2).hash_code())
	{
		CEffectChangeToFloat2_2* Obj = Scene->LoadGameObject<CEffectChangeToFloat2_2>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectChangeToFloat3).hash_code())
	{
		CEffectChangeToFloat3* Obj = Scene->LoadGameObject<CEffectChangeToFloat3>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectChangeToFloat4).hash_code())
	{
		CEffectChangeToFloat4* Obj = Scene->LoadGameObject<CEffectChangeToFloat4>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectChangeToFloat6).hash_code())
	{
		CEffectChangeToFloat6* Obj = Scene->LoadGameObject<CEffectChangeToFloat6>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectChangeToFloat7).hash_code())
	{
		CEffectChangeToFloat7* Obj = Scene->LoadGameObject<CEffectChangeToFloat7>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CWaterFloat1).hash_code())
	{
		CWaterFloat1* Obj = Scene->LoadGameObject<CWaterFloat1>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CWaterFloat2_2).hash_code())
	{
		CWaterFloat2_2* Obj = Scene->LoadGameObject<CWaterFloat2_2>();
		return Obj;
	}
	if (GameObjectTypeID == typeid(CEffectChangeToDyna2).hash_code())
	{
		CEffectChangeToDyna2* Obj = Scene->LoadGameObject<CEffectChangeToDyna2>();
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

