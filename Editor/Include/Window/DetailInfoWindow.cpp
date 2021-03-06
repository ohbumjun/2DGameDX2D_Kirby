#include "DetailInfoWindow.h"
// IMGUI
#include "IMGUILabel.h"
#include "IMGUIImage.h"
#include "IMGUIComboBox.h"
#include "IMGUIButton.h"
#include "IMGUIText.h"
#include "IMGUISameLine.h"
#include "IMGUITextInput.h"
#include "../EditorManager.h"
#include "IMGUIListBox.h"
#include "ObjectHierarchy.h"
// Object
#include "GameObject/GameObject.h"
#include "GameObject/LifeObject.h"
#include "../Object/Block.h"
// Scene
#include "Scene/SceneResource.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Component/SceneComponent.h"

CDetailInfoWindow::CDetailInfoWindow()
{
}

CDetailInfoWindow::~CDetailInfoWindow()
{
}

bool CDetailInfoWindow::Init()
{
	CIMGUIWindow::Init();

	// ===============================================
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Animation", 400.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	m_CharacterSprite = AddWidget<CIMGUIImage>("SpriteOrigin", 150.f, 150.f);
	
	// ===============================================

	Label = AddWidget<CIMGUILabel>("Animation", 80.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_CharacterCurrentAnimation = AddWidget<CIMGUIText>("AnimName");
	m_CharacterCurrentAnimation->SetColor(255, 255, 255);
	m_CharacterCurrentAnimation->SetText("- - - ");
	m_CharacterCurrentAnimation->SetSize(150.f, 80.f);

	// ===============================================

	Label = AddWidget<CIMGUILabel>("Set Anim", 80.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_SetAnimationComboBox = AddWidget<CIMGUIComboBox>("Set Anim", 100.f, 30.f);
	/*
	m_SetAnimationComboBox->AddItem("Sprite");
	m_SetAnimationComboBox->AddItem("Scene");
	m_SetAnimationComboBox->AddItem("TileMap");
	m_SetAnimationComboBox->AddItem("BackGround");
	m_SetAnimationComboBox->AddItem("Line");
	m_SetAnimationComboBox->AddItem("CharacterCreate");
	m_SetAnimationComboBox->AddItem("CharacterEdit");
	*/
	m_SetAnimationComboBox->SetHideName(true);
	m_SetAnimationComboBox->SetSelectCallback<CDetailInfoWindow>(this, &CDetailInfoWindow::SetCurrentAnimationCallback);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.0f);


	// ===============================================
	Label = AddWidget<CIMGUILabel>("Detail Info", 400.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ===============================================
	Label = AddWidget<CIMGUILabel>("Name", 80.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_CharacterName = AddWidget<CIMGUIText>("Name");
	m_CharacterName->SetColor(255, 255, 255);
	m_CharacterName->SetText("- - - ");
	m_CharacterName->SetSize(150.f, 80.f);


	// ===============================================
	Label = AddWidget<CIMGUILabel>("IsPhysics", 80.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_IsPhysicsText = AddWidget<CIMGUIText>("IsPhysics");
	m_IsPhysicsText->SetColor(255, 255, 255);
	m_IsPhysicsText->SetText("- - - ");
	m_IsPhysicsText->SetSize(150.f, 80.f);

	// ===============================================
	Label = AddWidget<CIMGUILabel>("Set Physics", 80.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_IsPhysicsComboBox = AddWidget<CIMGUIComboBox>("Physics", 100.f, 30.f);
	m_IsPhysicsComboBox->AddItem("TRUE");
	m_IsPhysicsComboBox->AddItem("FALSE");
	m_IsPhysicsComboBox->SetHideName(true);
	m_IsPhysicsComboBox->SetSelectCallback<CDetailInfoWindow>(this, &CDetailInfoWindow::SetPhysicsSimulateCallback);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.0f);

	// ===============================================
	Label = AddWidget<CIMGUILabel>("Transform", 400.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Label = AddWidget<CIMGUILabel>("Pos", 40.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ImGui::SetCursorScreenPos(ImVec2(100.f, 100.f));

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosX = AddWidget<CIMGUITextInput>("PosX", 60.f, 30.f);
	m_PosX->SetHideName(true);
	m_PosX->SetCallback(this, &CDetailInfoWindow::SetPositionXCallback);
	m_PosX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosY = AddWidget<CIMGUITextInput>("PosY", 60.f, 30.f);
	m_PosY->SetHideName(true);
	m_PosY->SetCallback(this, &CDetailInfoWindow::SetPositionYCallback);
	m_PosY->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosZ = AddWidget<CIMGUITextInput>("PosZ", 60.f, 30.f);
	m_PosZ->SetHideName(true);
	m_PosZ->SetCallback(this, &CDetailInfoWindow::SetPositionZCallback);
	m_PosZ->SetTextType(ImGuiText_Type::Float);

	// ===============================================
	Label = AddWidget<CIMGUILabel>("Rot", 40.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotX = AddWidget<CIMGUITextInput>("RotX", 60.f, 30.f);
	m_RotX->SetHideName(true);
	m_RotX->SetCallback(this, &CDetailInfoWindow::SetRotationXCallback);
	m_RotX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotY = AddWidget<CIMGUITextInput>("RotY", 60.f, 30.f);
	m_RotY->SetHideName(true);
	m_RotY->SetCallback(this, &CDetailInfoWindow::SetRotationYCallback);
	m_RotY->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotZ = AddWidget<CIMGUITextInput>("RotZ", 60.f, 30.f);
	m_RotZ->SetHideName(true);
	m_RotZ->SetCallback(this, &CDetailInfoWindow::SetRotationZCallback);
	m_RotZ->SetTextType(ImGuiText_Type::Float);

	// ==============================================

	Label = AddWidget<CIMGUILabel>("Scale", 40.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ScaleX = AddWidget<CIMGUITextInput>("ScaleX", 60.f, 30.f);
	m_ScaleX->SetHideName(true);
	m_ScaleX->SetCallback(this, &CDetailInfoWindow::SetScalingXCallback);
	m_ScaleX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ScaleY = AddWidget<CIMGUITextInput>("ScaleY", 60.f, 30.f);
	m_ScaleY->SetHideName(true);
	m_ScaleY->SetCallback(this, &CDetailInfoWindow::SetScalingYCallback);
	m_ScaleY->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ScaleZ = AddWidget<CIMGUITextInput>("ScaleZ", 60.f, 30.f);
	m_ScaleZ->SetHideName(true);
	m_ScaleZ->SetCallback(this, &CDetailInfoWindow::SetScalingZCallback);
	m_ScaleZ->SetTextType(ImGuiText_Type::Float);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);

	// ===============================================
	Label = AddWidget<CIMGUILabel>("Block Item", 400.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Label = AddWidget<CIMGUILabel>("Set Block", 80.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_BlockSetButton = AddWidget<CIMGUIButton>("Make Block", 100.f, 20.f);
	m_BlockSetButton->SetClickCallback(this, &CDetailInfoWindow::SetBlockTileAroundBlockItem);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	m_NormalSetButton = AddWidget<CIMGUIButton>("Make Normal", 100.f, 20.f);
	m_NormalSetButton->SetClickCallback(this, &CDetailInfoWindow::SetNormalTileAroundBlockItem);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);

	return true;
}

void CDetailInfoWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CDetailInfoWindow::SetPosRotScaleInfo(CGameObject* Object)
{
	Vector3 Pos       = Object->GetWorldPos();
	m_PosX->SetFloat(Pos.x);
	m_PosY->SetFloat(Pos.y);
	m_PosZ->SetFloat(Pos.z);

	Vector3 Rot       = Object->GetWorldRot();
	m_RotX->SetFloat(Rot.x);
	m_RotY->SetFloat(Rot.y);
	m_RotZ->SetFloat(Rot.z);

	Vector3 Scale = Object->GetWorldScale();
	m_ScaleX->SetFloat(Scale.x);
	m_ScaleY->SetFloat(Scale.y);
	// m_ScaleZ->SetFloat(Scale.z);
}

void CDetailInfoWindow::SetClickedObjectInfo(CSceneComponent* Component)
{
	if (!Component)
		return;

	// Sprite ???? ????????
	CSpriteComponent* SelectedRootComponent = dynamic_cast<CSpriteComponent*>(Component);

	if (!SelectedRootComponent)
		return;

	if (!SelectedRootComponent->GetAnimationInstance())
		return;

	m_ClickedComponent = Component;

	AnimationFrameData SelectFrame = SelectedRootComponent->GetAnimationInstance()->GetCurrentAnimation()->GetFrameData(0);

	m_CharacterSprite->SetTexture(SelectedRootComponent->GetMaterial()->GetTexture());
	m_CharacterSprite->SetImageStart(SelectFrame.Start);
	m_CharacterSprite->SetImageEnd( SelectFrame.Start + SelectFrame.Size);

	// Object ???? 
	SetDetailInfoName(Component->GetGameObject()->GetName());

	// Animation ???? 
	if (SelectedRootComponent->GetAnimationInstance())
		SetCurrentAnimationName(SelectedRootComponent->GetAnimationInstance()->GetCurrentAnimation()->GetName());

	// Animation Combox ?????? Animation ???? ???? ????
	std::vector<std::string> AnimNames;
	SelectedRootComponent->GetAnimationInstance()->GatherAnimationNames(AnimNames);

	m_SetAnimationComboBox->Clear();

	size_t AnimSize = AnimNames.size();

	for (size_t i = 0; i < AnimSize; i++)
	{
		m_SetAnimationComboBox->AddItem(AnimNames[i]);
	}

	// Is Ground ????
	CLifeObject* OwnerObject = (CLifeObject*)(Component->GetGameObject());
	SetPhysicsInfo(OwnerObject->IsPhysicsSimulate());

	// Transform ????
	SetTransformInfo(Component);
}

void CDetailInfoWindow::SetCurrentAnimationCallback(int Index, const char* Animation)
{
	if (!m_ClickedComponent)
		return;

	// ???? Animation Change 
	CSpriteComponent* SelectedRootComponent = (CSpriteComponent*)m_ClickedComponent.Get();

	SelectedRootComponent->GetAnimationInstance()->SetCurrentAnimation(Animation);

	// Sprite ?? ????
	AnimationFrameData SelectFrame = SelectedRootComponent->GetAnimationInstance()->GetCurrentAnimation()->GetFrameData(0);
	m_CharacterSprite->SetTexture(SelectedRootComponent->GetMaterial()->GetTexture());
	m_CharacterSprite->SetImageStart(SelectFrame.Start);
	m_CharacterSprite->SetImageEnd(SelectFrame.Start + SelectFrame.Size);

	// Animation Name Text ????
	SetCurrentAnimationName(SelectedRootComponent->GetAnimationInstance()->GetCurrentAnimation()->GetName());

}

void CDetailInfoWindow::SetDetailInfoName(const std::string& Name)
{
	m_CharacterName->SetText(Name.c_str());
}

void CDetailInfoWindow::SetPhysicsInfo(bool Enable)
{
	char IsPhysicsText[MAX_PATH] = {};

	if (Enable)
		sprintf_s(IsPhysicsText, "%s", "TRUE");
	else
		sprintf_s(IsPhysicsText, "%s", "FALSE");

	m_IsPhysicsText->SetText(IsPhysicsText);
}

void CDetailInfoWindow::SetCurrentAnimationName(const std::string& Name)
{
	m_CharacterCurrentAnimation->SetText(Name.c_str());
}

void CDetailInfoWindow::SetPhysicsSimulateCallback(int Index, const char* Animation)
{
	if (!m_ClickedComponent)
		return;

	// CLifeObject* OwnerObject = (CLifeObject*)(m_ClickedComponent->GetGameObject());
	CLifeObject* OwnerObject = (CLifeObject*)(m_ClickedComponent->GetGameObject());

	if (!OwnerObject)
		return;

	if (strcmp(Animation, "TRUE") == 0)
	{
		// ???? ???? ????
		OwnerObject->SetPhysicsSimulate(true);

		// ???? Text ???? 
		SetPhysicsInfo(true);
	}
	else
	{
		OwnerObject->SetPhysicsSimulate(false);
		SetPhysicsInfo(false);
	}
}

void CDetailInfoWindow::SetBlockTileAroundBlockItem()
{
	if (!m_ClickedComponent)
		return;

	// CLifeObject* OwnerObject = (CLifeObject*)(m_ClickedComponent->GetGameObject());
	CBlock* BlockObject = dynamic_cast<CBlock*>(m_ClickedComponent->GetGameObject());

	if (BlockObject)
	{
		BlockObject = dynamic_cast<CBlock*>(m_ClickedComponent->GetGameObject());

		BlockObject->MakeTileTypeAround(true); //
	}
}

void CDetailInfoWindow::SetNormalTileAroundBlockItem()
{
	if (!m_ClickedComponent)
		return;

	// CLifeObject* OwnerObject = (CLifeObject*)(m_ClickedComponent->GetGameObject());
	CBlock* BlockObject = dynamic_cast<CBlock*>(m_ClickedComponent->GetGameObject());

	if (BlockObject)
	{
		BlockObject = dynamic_cast<CBlock*>(m_ClickedComponent->GetGameObject());

		BlockObject->MakeTileTypeAround(false);
	}
}

void CDetailInfoWindow::SetTransformInfo(CSceneComponent* Component)
{
	Vector3 WorldScale = Component->GetWorldScale();
	Vector3 WorldPos    = Component->GetWorldPos();
	Vector3 WorldRot    = Component->GetWorldRot();

	m_PosX->SetFloat(WorldPos.x);
	m_PosY->SetFloat(WorldPos.y);
	m_PosZ->SetFloat(WorldPos.z);

	m_RotX->SetFloat(WorldRot.x);
	m_RotY->SetFloat(WorldRot.y);
	m_RotZ->SetFloat(WorldRot.z);

	m_ScaleX->SetFloat(WorldScale.x);
	m_ScaleY->SetFloat(WorldScale.y);
	m_ScaleZ->SetFloat(WorldScale.z);
}

void CDetailInfoWindow::SetPositionXCallback()
{
	// ?????? GameObject + Component?? ???????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < -1)
		return;

	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;

	Vector3 ObjectPos = Object->GetWorldPos();
	float NewXPos = m_PosX->GetValueFloat();
	Object->SetWorldPos(NewXPos, ObjectPos.y, ObjectPos.z);
}

void CDetailInfoWindow::SetPositionYCallback()
{
	// ?????? GameObject + Component?? ???????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	// Object ???? X
	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < -1)
		return;

	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	// ???? ???????? ?? Object?? ?????? 
	if (!Object)
		return;

	Vector3 ObjectPos = Object->GetWorldPos();
	float NewYPos = m_PosY->GetValueFloat();
	Object->SetWorldPos(ObjectPos.x, NewYPos, ObjectPos.z);
}

void CDetailInfoWindow::SetPositionZCallback()
{
	// ?????? GameObject + Component?? ???????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;


	Vector3 ObjectPos = Object->GetWorldPos();
	float NewZPos = m_PosZ->GetValueFloat();
	Object->SetWorldPos(ObjectPos.x, ObjectPos.y, NewZPos);
}

void CDetailInfoWindow::SetRotationXCallback()
{
	// ?????? GameObject + Component?? ???????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;

	Object->SetWorldRotationX(m_RotX->GetValueFloat());
}

void CDetailInfoWindow::SetRotationYCallback()
{
	// ?????? GameObject + Component?? ???????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;
	
	Object->SetWorldRotationY(m_RotY->GetValueFloat());
}

void CDetailInfoWindow::SetRotationZCallback()
{
	// ?????? GameObject + Component?? ???????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;
	
	Object->SetWorldRotationZ(m_RotZ->GetValueFloat());
}

void CDetailInfoWindow::SetScalingXCallback()
{
	// ?????? GameObject + Component?? ???????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;
	Vector3 ObjectScale = Object->GetWorldScale();
	Object->SetWorldScale(m_ScaleX->GetValueFloat(), ObjectScale.y, ObjectScale.z);
}

void CDetailInfoWindow::SetScalingYCallback()
{
	// ?????? GameObject + Component?? ???????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;
	Vector3 ObjectScale = Object->GetWorldScale();
	Object->SetWorldScale(ObjectScale.x,m_ScaleY->GetValueFloat(), ObjectScale.z);
}

void CDetailInfoWindow::SetScalingZCallback()
{
	// ?????? GameObject + Component?? ???????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;
	Vector3 ObjectScale = Object->GetWorldScale();
	Object->SetWorldScale(ObjectScale.x, ObjectScale.y, m_ScaleZ->GetValueFloat());
}