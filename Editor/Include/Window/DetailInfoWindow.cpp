#include "DetailInfoWindow.h"
// IMGUI
#include "IMGUILabel.h"
#include "IMGUIImage.h"
#include "IMGUIComboBox.h"
#include "IMGUIText.h"
#include "IMGUISameLine.h"
#include "IMGUITextInput.h"
#include "../EditorManager.h"
#include "IMGUIListBox.h"
#include "ObjectHierarchy.h"
// Object
#include "GameObject/GameObject.h"
#include "GameObject/LifeObject.h"
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
	Label = AddWidget<CIMGUILabel>("IsGround", 80.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_IsGroundText = AddWidget<CIMGUIText>("AnimName");
	m_IsGroundText->SetColor(255, 255, 255);
	m_IsGroundText->SetText("- - - ");
	m_IsGroundText->SetSize(150.f, 80.f);

	// ===============================================
	Label = AddWidget<CIMGUILabel>("Set Ground", 80.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_IsGroundComboBox = AddWidget<CIMGUIComboBox>("Is Ground", 100.f, 30.f);
	m_IsGroundComboBox->AddItem("TRUE");
	m_IsGroundComboBox->AddItem("FALSE");
	m_IsGroundComboBox->SetHideName(true);
	m_IsGroundComboBox->SetSelectCallback<CDetailInfoWindow>(this, &CDetailInfoWindow::SetIsGroundCallback);

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
	m_ScaleZ->SetFloat(Scale.z);
}

void CDetailInfoWindow::SetClickedObjectInfo(CSceneComponent* Component)
{
	m_ClickedComponent = Component;

	// Sprite 정보 세팅하기
	CSpriteComponent* SelectedRootComponent = (CSpriteComponent*)Component;

	AnimationFrameData SelectFrame = SelectedRootComponent->GetAnimationInstance()->GetCurrentAnimation()->GetFrameData(0);

	m_CharacterSprite->SetTexture(SelectedRootComponent->GetMaterial()->GetTexture());
	m_CharacterSprite->SetImageStart(SelectFrame.Start);
	m_CharacterSprite->SetImageEnd( SelectFrame.Start + SelectFrame.Size);

	// Object 이름 
	SetDetailInfoName(Component->GetGameObject()->GetName());

	// Animation 이름 
	if (SelectedRootComponent->GetAnimationInstance())
		SetCurrentAnimationName(SelectedRootComponent->GetAnimationInstance()->GetCurrentAnimation()->GetName());

	// Animation Combox 목록에 Animation 목록 나열 하기
	std::vector<std::string> AnimNames;
	SelectedRootComponent->GetAnimationInstance()->GatherAnimationNames(AnimNames);

	m_SetAnimationComboBox->Clear();

	size_t AnimSize = AnimNames.size();

	for (size_t i = 0; i < AnimSize; i++)
	{
		m_SetAnimationComboBox->AddItem(AnimNames[i]);
	}

	// Is Ground 여부
	CLifeObject* OwnerObject = (CLifeObject*)(Component->GetGameObject());
	SetIsGroundInfo(OwnerObject->IsGround());

	// Transform 정보
	SetTransformInfo(Component);
}

void CDetailInfoWindow::SetCurrentAnimationCallback(int Index, const char* Animation)
{
	if (!m_ClickedComponent)
		return;

	// 실제 Animation Change 
	CSpriteComponent* SelectedRootComponent = (CSpriteComponent*)m_ClickedComponent.Get();

	SelectedRootComponent->GetAnimationInstance()->SetCurrentAnimation(Animation);

	// Sprite 에 반영
	AnimationFrameData SelectFrame = SelectedRootComponent->GetAnimationInstance()->GetCurrentAnimation()->GetFrameData(0);
	m_CharacterSprite->SetTexture(SelectedRootComponent->GetMaterial()->GetTexture());
	m_CharacterSprite->SetImageStart(SelectFrame.Start);
	m_CharacterSprite->SetImageEnd(SelectFrame.Start + SelectFrame.Size);

	// Animation Name Text 변화
	SetCurrentAnimationName(SelectedRootComponent->GetAnimationInstance()->GetCurrentAnimation()->GetName());

}

void CDetailInfoWindow::SetDetailInfoName(const std::string& Name)
{
	m_CharacterName->SetText(Name.c_str());
}

void CDetailInfoWindow::SetIsGroundInfo(bool Enable)
{
	char IsGroundText[MAX_PATH] = {};

	if (Enable)
		sprintf_s(IsGroundText, "%s", "TRUE");
	else
		sprintf_s(IsGroundText, "%s", "FALSE");

	m_IsGroundText->SetText(IsGroundText);
}

void CDetailInfoWindow::SetCurrentAnimationName(const std::string& Name)
{
	m_CharacterCurrentAnimation->SetText(Name.c_str());
}

void CDetailInfoWindow::SetIsGroundCallback(int Index, const char* Animation)
{
	if (!m_ClickedComponent)
		return;

	CLifeObject* OwnerObject = (CLifeObject*)(m_ClickedComponent->GetGameObject());

	if (!OwnerObject)
		return;

	if (strcmp(Animation, "TRUE") == 0)
	{
		// 실제 정보 세팅
		OwnerObject->SetIsGround(true);

		// 실제 Text 세팅 
		SetIsGroundInfo(true);
	}
	else
	{
		OwnerObject->SetIsGround(false);
		SetIsGroundInfo(false);
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
	// 선택한 GameObject + Component가 존재해야 한다.
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
	// 선택한 GameObject + Component가 존재해야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	// Object 선택 X
	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < -1)
		return;

	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	// 해당 이름으로 된 Object가 없다면 
	if (!Object)
		return;

	Vector3 ObjectPos = Object->GetWorldPos();
	float NewYPos = m_PosY->GetValueFloat();
	Object->SetWorldPos(ObjectPos.x, NewYPos, ObjectPos.z);
}

void CDetailInfoWindow::SetPositionZCallback()
{
	// 선택한 GameObject + Component가 존재해야 한다.
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
	// 선택한 GameObject + Component가 존재해야 한다.
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
	// 선택한 GameObject + Component가 존재해야 한다.
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
	// 선택한 GameObject + Component가 존재해야 한다.
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
	// 선택한 GameObject + Component가 존재해야 한다.
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
	// 선택한 GameObject + Component가 존재해야 한다.
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
	// 선택한 GameObject + Component가 존재해야 한다.
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