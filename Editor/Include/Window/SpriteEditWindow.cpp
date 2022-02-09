#include "SpriteEditWindow.h"
#include "Device.h"
#include "Engine.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "PathManager.h"
#include "../EditorManager.h"
#include "../Object/DragObject.h"
#include "../Object/SpriteEditObject.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Resource/Texture/Texture.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"

CSpriteEditWindow::CSpriteEditWindow() :
	m_SpriteObject(nullptr),
	m_Reverse(false),
	m_CandyCrushVersion(false)
{
}

CSpriteEditWindow::~CSpriteEditWindow()
{
	SAFE_DELETE(m_Animation);
}

bool CSpriteEditWindow::Init()
{
	CIMGUIWindow::Init();
	// ==============================
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Reverse", 80.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_IsReverseText = AddWidget<CIMGUIText>("IsReverse");
	m_IsReverseText->SetColor(255, 0, 0);
	m_IsReverseText->SetText("FALSE");

	// ==============================
	Label = AddWidget<CIMGUILabel>("CandyCrush", 80.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_IsCandyVersionText = AddWidget<CIMGUIText>("IsCandyCrush");
	m_IsCandyVersionText->SetColor(255, 0, 0);
	m_IsCandyVersionText->SetText("FALSE");

	// ==============================
	Label = AddWidget<CIMGUILabel>("CameraPos", 80.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_CameraPosX = AddWidget<CIMGUIText>("CameraPosX");
	m_CameraPosX->SetColor(255, 255, 255);
	m_CameraPosX->SetText("0.0");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(180.f);

	m_CameraPosY = AddWidget<CIMGUIText>("CameraPosY");
	m_CameraPosY->SetColor(255, 255, 255);
	m_CameraPosY->SetText("0.0");

	// ==============================

	CIMGUIButton* Button = AddWidget<CIMGUIButton>("LoadTexture", 80.f, 80.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::LoadTextureButton);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("StartEdit", 80.f, 80.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SpriteEditButton);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("SetReverse", 80.f, 80.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetReverseMode);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("SetNormal", 80.f, 80.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetNormalMode);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("SetCandy", 80.f, 80.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetCandyCrushMode);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("UndoCandy", 80.f, 80.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::UndoCandyCrushMode);

	// ==============================

	Button = AddWidget<CIMGUIButton>("SaveSeq", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SaveSequence);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("LoadSeq", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::LoadSequence);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("SaveAnim", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SaveAnimation);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("LoadAnim", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::LoadAnimation);

	// ==============================

	Button = AddWidget<CIMGUIButton>("MvBlockRight", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::MoveOneBlockRight);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("MvBlockLeft", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::MoveOneBlockLeft);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("MvBlockUp", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::MoveOneBlockUp);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("MvBlockDown", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::MoveOneBlockDown);

	// ==============================

	Button = AddWidget<CIMGUIButton>("MvPixRight", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::MoveOnePixelRight);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("MvPixLeft", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::MoveOnePixelLeft);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("MvPixUp", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::MoveOnePixelUp);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("MvPixDown", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::MoveOnePixelDown);

	// ==============================
	Button = AddWidget<CIMGUIButton>("BigPixRight", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::EnlargeOnePixelRight);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("RdPixLeft", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::ReduceOnePixelLeft);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("RdPixUp", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::ReduceOnePixelUp);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("BigPixDown", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::EnlargeOnePixelDown);

	// ==============================

	m_Sprite = AddWidget<CIMGUIImage>("SpriteOrigin", 200.f, 200.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(215.f);

	m_SpriteSampled = AddWidget<CIMGUIImage>("SpriteSampled", 200.f, 200.f);
	m_SpriteSampled->SetBorderColor(255, 0, 0);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(420.f);

	m_SpriteCurrentFrame = AddWidget<CIMGUIImage>("SpriteCurrentFrame", 200.f, 200.f);
	m_SpriteCurrentFrame->SetTexture("DefaultUI");
	m_SpriteCurrentFrame->SetBorderColor(255, 0, 0);

	// ==============================

	Label = AddWidget<CIMGUILabel>("AnimationListName", 200.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("AnimFrameName", 200.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================
	m_AnimationList = AddWidget<CIMGUIListBox>("AnimationList", 200.f, 300.f);
	m_AnimationList->SetHideName(true);
	m_AnimationList->SetPageItemCount(6);
	m_AnimationList->SetSelectCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SelectAnimationSequence);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_AnimationFrameList = AddWidget<CIMGUIListBox>("AnimationFrameList", 200.f, 300.f);
	m_AnimationFrameList->SetHideName(true);
	m_AnimationFrameList->SetPageItemCount(6);
	m_AnimationFrameList->SetSelectCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SelectAnimationFrame);

	// ==============================
	Label = AddWidget<CIMGUILabel>("AnimName", 80.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

	m_AnimInputName = AddWidget<CIMGUITextInput>("AnimNameInput");
	m_AnimInputName->SetHideName(true);
	m_AnimInputName->SetSize(80.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(175.f);

	Button = AddWidget<CIMGUIButton>("AddAnim", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::AddAnimationButton);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(335.f);

	Button = AddWidget<CIMGUIButton>("AddFrame", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::AddAnimationFrameButton);

	// ==============================
	Label = AddWidget<CIMGUILabel>("SeqName", 80.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

	m_NewSequenceName = AddWidget<CIMGUITextInput>("EditSeqName");
	m_NewSequenceName->SetSize(80.f, 40.f);
	m_NewSequenceName->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(175.f);

	Button = AddWidget<CIMGUIButton>("EditName", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::EditSequenceName);


	// ==============================
	Label = AddWidget<CIMGUILabel>("RevCpyName", 80.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

	m_RevCopyTargetInputName = AddWidget<CIMGUITextInput>("RevCopySeqName");
	m_RevCopyTargetInputName->SetSize(80.f, 40.f);
	m_RevCopyTargetInputName->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(175.f);

	Button = AddWidget<CIMGUIButton>("RevCopy", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::RevCopySequenceButton);


	// ==============================
	Label = AddWidget<CIMGUILabel>("NewSeq", 80.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

	m_NewSeqAnimationLoop = AddWidget<CIMGUIComboBox>("NSeqLp", 80.f, 30.f);
	m_NewSeqAnimationLoop->SetHideName(true);
	m_NewSeqAnimationLoop->AddItem("True");
	m_NewSeqAnimationLoop->AddItem("False");
	m_NewSeqAnimationLoop->SetSelectCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetNewAnimSequenceLoop);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(180.f);

	m_NewSeqAnimationReverse = AddWidget<CIMGUIComboBox>("NSeqRev", 80.f, 30.f);
	m_NewSeqAnimationReverse->SetHideName(true);
	m_NewSeqAnimationReverse->AddItem("True");
	m_NewSeqAnimationReverse->AddItem("False");
	m_NewSeqAnimationReverse->SetSelectCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetNewAnimSequenceReverse);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(335.f);

	Button = AddWidget<CIMGUIButton>("Play", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::PlayAnimationButton);

	// ==============================
	Label = AddWidget<CIMGUILabel>("CurSeq", 80.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

	m_CurSeqAnimationLoop = AddWidget<CIMGUIComboBox>("CSeqLp", 80.f, 30.f);
	m_CurSeqAnimationLoop->SetHideName(true);
	m_CurSeqAnimationLoop->AddItem("True");
	m_CurSeqAnimationLoop->AddItem("False");
	m_CurSeqAnimationLoop->SetSelectCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetCurAnimSequenceLoop);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(180.f);

	m_CurSeqAnimationReverse = AddWidget<CIMGUIComboBox>("CSeqRev", 80.f, 30.f);
	m_CurSeqAnimationReverse->SetHideName(true);
	m_CurSeqAnimationReverse->AddItem("True");
	m_CurSeqAnimationReverse->AddItem("False");
	m_CurSeqAnimationReverse->SetSelectCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetCurAnimSequenceReverse);
	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(335.f);

	Button = AddWidget<CIMGUIButton>("Stop", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::StopAnimationButton);


	// ==============================

	Button = AddWidget<CIMGUIButton>("EditFrame", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::EditFrameButton);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

	Button = AddWidget<CIMGUIButton>("DelSeq", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::DeleteAnimationSequence);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(175.f);

	Button = AddWidget<CIMGUIButton>("ClearSeq", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::ClearAnimationSequence);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(260.f);

	Button = AddWidget<CIMGUIButton>("DelFrame", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::DeleteFrameButton);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(345.f);

	Button = AddWidget<CIMGUIButton>("ClearFrame", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::ClearFrameButton);

	// =============================

	m_StartFramePosXInput = AddWidget<CIMGUITextInput>("StartX");
	m_StartFramePosXInput->SetSize(80.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	m_StartFramePosYInput = AddWidget<CIMGUITextInput>("StartY");
	m_StartFramePosYInput->SetSize(80.f, 30.f);

	m_EndFramePosXInput = AddWidget<CIMGUITextInput>("EndX");
	m_EndFramePosXInput->SetSize(80.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	m_EndFramePosYInput = AddWidget<CIMGUITextInput>("EndY");
	m_EndFramePosYInput->SetSize(80.f, 30.f);

	// =============================
	Button = AddWidget<CIMGUIButton>("RightEnd", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetDragObjectToRightEnd);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

	Button = AddWidget<CIMGUIButton>("LeftEnd", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetDragObjectToLeftEnd);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(175.f);

	Button = AddWidget<CIMGUIButton>("ToTop", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetDragObjectToTop);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(260.f);

	Button = AddWidget<CIMGUIButton>("ToBottom", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetDragObjectToBottom);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(345.f);

	Button = AddWidget<CIMGUIButton>("ToTexture", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SetDragObjectTexture);

	// =============================
	Label = AddWidget<CIMGUILabel>("DivNum", 80.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

	m_AddDivideNumberInput = AddWidget<CIMGUITextInput>("AddDivideNumberInput");
	m_AddDivideNumberInput->SetSize(80.f, 30.f);
	m_AddDivideNumberInput->SetTextType(ImGuiText_Type::Int);
	m_AddDivideNumberInput->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(175.f);

	Button = AddWidget<CIMGUIButton>("AddDivWidth", 100.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::DivideFrameWidthAndAdd);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(280.f);

	Button = AddWidget<CIMGUIButton>("AddDivHeight", 100.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::DivideFrameHeightAndAdd);

	// =============================

	Label = AddWidget<CIMGUILabel>("Num", 80.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

	m_DivMultiNumberInput = AddWidget<CIMGUITextInput>("DivMultiNumberInput");
	m_DivMultiNumberInput->SetSize(80.f, 30.f);
	m_DivMultiNumberInput->SetTextType(ImGuiText_Type::Int);
	m_DivMultiNumberInput->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(175.f);

	Button = AddWidget<CIMGUIButton>("DivWidth", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::DivideFrameWidth);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(260.f);

	Button = AddWidget<CIMGUIButton>("DivHeight", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::DivideFrameHeight);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(345.f);

	Button = AddWidget<CIMGUIButton>("MultiWidth", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::MultiplyFrameWidth);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(430.f);

	Button = AddWidget<CIMGUIButton>("MultiHeight", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::MultiplyFrameHeight);

	// =============================

	m_Animation = new CAnimationSequence2DInstance;
	m_Animation->Init();
	m_Animation->Stop();

	return true;
}

void CSpriteEditWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	if (m_Animation)
	{
		m_Animation->Update(DeltaTime);

		if (!m_Animation->IsPlay())
			return;

		if (!m_Animation->GetCurrentAnimation())
			return;

		if (m_Animation->GetCurrentAnimation()->GetFrameCount() <= 0)
			return;

		// Update �� ����, ���� Frame�� Animation Frame Data�� �����ش�
		int                CurrentFrame = m_Animation->GetCurrentAnimation()->GetCurrentFrame();
		AnimationFrameData FrameData    = m_Animation->GetCurrentAnimation()->GetFrameData(CurrentFrame);
		m_SpriteSampled->SetImageStart(FrameData.Start);
		m_SpriteSampled->SetImageEnd(FrameData.Start + FrameData.Size);
	}
}

void CSpriteEditWindow::SetCameraPosText(float PosX, float PosY)
{
	char PosTest[128] = {};
	sprintf_s(PosTest, "%.2f", PosX);
	m_CameraPosX->SetText(PosTest);
	
	sprintf_s(PosTest, "%.2f", PosY);
	m_CameraPosY->SetText(PosTest);
}

void CSpriteEditWindow::SetReverseMode()
{
	if (!m_SpriteObject)
		return;
	m_Reverse = true;
	m_SpriteObject->SetReverse(m_Reverse);

	// Test ����
	m_IsReverseText->SetText("TRUE");
	m_IsReverseText->SetColor(0, 0, 255);
}

void CSpriteEditWindow::SetNormalMode()
{
	if (!m_SpriteObject)
		return;
	m_Reverse = false;
	m_SpriteObject->SetReverse(m_Reverse);

	// Test ����
	m_IsReverseText->SetText("FALSE");
	m_IsReverseText->SetColor(255, 0,0);
}

void CSpriteEditWindow::SetCandyCrushMode()
{
	m_CandyCrushVersion = true;

	m_IsCandyVersionText->SetText("TRUE");
	m_IsCandyVersionText->SetColor(0, 0, 255);
}

void CSpriteEditWindow::UndoCandyCrushMode()
{
	m_CandyCrushVersion = false;

	m_IsCandyVersionText->SetText("FALSE");
	m_IsCandyVersionText->SetColor(255, 0, 0);
}

void CSpriteEditWindow::EditSequenceName()
{
	int SelectIndex = m_AnimationList->GetSelectIndex();
	if (SelectIndex < 0)
		return;

	const std::string PrevSeqName = m_AnimationList->GetSelectItem();
	const std::string NewSeqName = m_NewSequenceName->GetTextMultibyte();

	// Scene Resource ���� �����ϱ�
	CSceneManager::GetInst()->GetScene()->GetResource()->EditSequence2DName(PrevSeqName, NewSeqName);

	// Animation ��ü ���� �����ϱ�
	if (m_Animation)
	{
		m_Animation->EditSequence2DName(PrevSeqName, NewSeqName);
	}

	// Animation List �� �ٲ��ֱ�
	m_AnimationList->SetItem(SelectIndex, NewSeqName);

	// m_NewSequenceName ���� ����ֱ�
	m_NewSequenceName->Empty();
}

void CSpriteEditWindow::LoadTextureButton()
{
	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Sprite)
		return;

	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner   = CEngine::GetInst()->GetWindowHandle(); // handle to window that owns the dialog box
	OpenFile.lpstrFilter =
		TEXT("�������\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
	OpenFile.lpstrFile = FilePath;
	// buffer ! filename used to initialize the file name edit control -> ���������� save,open�ҽ� ���⿡ filePath ������ ���´�.
	OpenFile.nMaxFile        = MAX_PATH; // size of buffer, pointed to by lpstrFile
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path; // Initial Directory

	if (GetOpenFileName(&OpenFile) != 0) // NonZero --> specifies file name, clicks ok button
	{
		SpriteEditButton();

		int   TextureIndex       = 0;
		TCHAR FileName[MAX_PATH] = {};
		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, MAX_PATH, nullptr, 0);

		char ConvertFileName[MAX_PATH] = {};
		int  Length                    = WideCharToMultiByte(CP_ACP, 0, FileName, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, FileName, -1, ConvertFileName, Length, nullptr, nullptr);

		m_Sprite->SetTextureFullPath(ConvertFileName, FilePath);
		m_SpriteCurrentFrame->SetTextureFullPath(ConvertFileName, FilePath);
		m_SpriteObject->GetSpriteComponent()->SetTextureFullPath(TextureIndex, 0,
		                                                         static_cast<int>(Buffer_Shader_Type::Pixel),
		                                                         ConvertFileName, FilePath);

		CTexture* Texture = m_SpriteObject->GetSpriteComponent()->GetTexture(TextureIndex);
		m_SpriteObject->SetWorldScale(static_cast<float>(Texture->GetWidth()), static_cast<float>(Texture->GetHeight()),
		                              1.f);
		m_SpriteObject->SetAnimationNewTexture(Texture);
	}
}

void CSpriteEditWindow::SpriteEditButton()
{
	CEditorManager::GetInst()->SetEditMode(EditMode::Sprite);

	if (!m_SpriteObject)
	{
		m_SpriteObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
		m_SpriteObject->SetEditWindow(this);

		// m_Sprite UIImage ���� ����
		CTexture* Texture = m_SpriteObject->GetSpriteComponent()->GetTexture();
		m_Sprite->SetTexture(Texture);
		m_SpriteCurrentFrame->SetTexture(Texture);

		// Editor Manager �󿡼� CameraObject �����Լ� ȣ�� ( ��, CameraObject �����ϱ� )
		CEditorManager::GetInst()->CreateCameraObject();
	}
}

void CSpriteEditWindow::RevCopySequenceButton()
{
	if (!m_Animation)
		return;

	// Not Selected
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	// Copy Input Name �� �ش��ϴ� Sequence�� �ҷ��´�.
	if (m_RevCopyTargetInputName->Empty())
		return;

	CSceneResource* SceneResource = CSceneManager::GetInst()->GetScene()->GetResource();

	std::string SrcSequenceName = m_RevCopyTargetInputName->GetTextMultibyte();

	if (!m_Animation->FindAnimationSequence2DData(SrcSequenceName))
		return;

	CAnimationSequence2D* SrcSequence = m_Animation->FindAnimationSequence2DData(SrcSequenceName)->GetAnimationSequence();

	// �ش� Sequece�� �������� ������ X
	if (!SrcSequence)
		return;

	// �ش� Sequence �� Frame�� ���鼭 �� �ݴ븦 Frame���� �߰����ش�.
	std::string DestSequenceName = m_AnimationList->GetSelectItem();

	// ���� ����̸� ���縦 �������� �ʴ´�.
	if (DestSequenceName == SrcSequenceName)
		return;

	if (!m_Animation->FindAnimationSequence2DData(DestSequenceName))
		return;

	CAnimationSequence2D* DestSequence = m_Animation->FindAnimationSequence2DData(DestSequenceName)->GetAnimationSequence();

	if (!DestSequence)
		return;

	size_t SrcFrameCount = SrcSequence->GetFrameCount();

	// ������ Frame �� ���ٸ� return
	if (SrcFrameCount <= 0)
		return;

	// - �ش� Sequence2D �� Reverse ������, ���� ����� �ݴ�� �������ش�.
	// - �ش� Sequece �� Reverse����, �ƴ����� ����, SpriteEdit Object�� ���ؼ��� Mode�� Reverse�� �������ֱ�
	bool SeqFrameReverse = m_Animation->FindAnimationSequence2DData(SrcSequenceName)->IsFrameReverse();
	bool ChangedRevInfo = SeqFrameReverse ? false : true;
	DestSequence->SetFrameReverse(ChangedRevInfo);

	if (ChangedRevInfo)
		SetReverseMode();
	else
		SetNormalMode();

	for (size_t i = 0; i < SrcFrameCount; i++)
	{
		AnimationFrameData SrcFrame = SrcSequence->GetFrameData((int)i);

		Vector2 SrcEndPos = SrcFrame.Start + SrcFrame.Size;
		Vector2 RevFrameStart = {};
		Vector2 RevFrameSize = {};

		// �ݴ� FrameData�� �������ش�.
		/*
		if (SrcSequence->IsFrameReverse())
		{
			RevFrameStart = Vector2(DestSequence->GetTexture()->GetWidth() + SrcFrame.Start.x, SrcFrame.Start.y);
			RevFrameSize = Vector2(DestSequence->GetTexture()->GetWidth() + SrcFrame.Size.x * -1.f, SrcFrame.Size.y);
		}
		else
		{
			RevFrameStart = Vector2(DestSequence->GetTexture()->GetWidth() - SrcFrame.Start.x, SrcFrame.Start.y);
			RevFrameSize  = Vector2(DestSequence->GetTexture()->GetWidth() - SrcFrame.Size.x * -1.f, SrcFrame.Size.y);
		}
		*/

		// ������ Reverse ���,
		if (SrcSequence->IsFrameReverse())
		{
			// SrcStart, End ��� ���������� ���̴�.
			// Start�� ���, Texture Width ���� End�� �� ����, ������ ��������� �Ѵ�.
			float RevFrameStartX = (DestSequence->GetTexture()->GetWidth() + SrcEndPos.x);
			// float RevFrameStartX = SrcFrame.Start.x * -1.f;
			RevFrameStart = Vector2(RevFrameStartX, SrcFrame.Start.y);
			RevFrameSize = Vector2(SrcFrame.Size.x * -1.f, SrcFrame.Size.y);
		}
		else
		{
			RevFrameStart = Vector2((DestSequence->GetTexture()->GetWidth() - SrcEndPos.x) * -1.f, SrcFrame.Start.y);
			RevFrameSize = Vector2(SrcFrame.Size.x * -1.f, SrcFrame.Size.y);
		}
		// Frame �߰�
		DestSequence->AddFrame(RevFrameStart, RevFrameSize);

		// AnimationFrameList �߰�
		m_AnimationFrameList->AddItem(std::to_string(m_AnimationFrameList->GetItemCount()));
	}

	// Frame �߰��� �Ϸ�Ǹ�
	// - ù��° Frame�� ���õ� �༮���� �����ϰ�
	m_AnimationFrameList->SetSelectIndex(0);

	// - DragObject, CurrentSampled�� ��ġ �������ְ�
	// DragObject�� Image Pos ����
	// Sprite Sample�� �ش� ù��° Frame�� Start, End Pos�� �������ش�
	AnimationFrameData NewFrameData = DestSequence->GetFrameData(0);
	Vector2 NewFrameStartPos = NewFrameData.Start;
	Vector2 NewFrameEndPos = NewFrameData.Start + NewFrameData.Size;

	// WorldPos
	NewFrameStartPos.y = DestSequence->GetTexture()->GetHeight() - NewFrameStartPos.y;
	NewFrameEndPos.y = DestSequence->GetTexture()->GetHeight() - NewFrameEndPos.y;

	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (m_Reverse)
	{
		// DragObject�� Image Pos ����
		// Vector2 ReverseStartPos = Vector2(NewFrameStartPos.x * -1.f, NewFrameStartPos.y);
		// Vector2 ReverseEndPos = Vector2(NewFrameEndPos.x * -1.f, NewFrameEndPos.y);
		// DragObject->SetStartPos(ReverseStartPos);
		DragObject->SetStartPos(NewFrameStartPos);
		// DragObject->SetEndPos(ReverseEndPos);
		DragObject->SetEndPos(NewFrameEndPos);

		// m_SpriteSampled Texture ���� 
		NewFrameStartPos.y = DestSequence->GetTexture()->GetHeight() - NewFrameStartPos.y;
		NewFrameEndPos.y = DestSequence->GetTexture()->GetHeight() - NewFrameEndPos.y;

		// �ش� FramePos ������ Animation Frame ���� �־��ֱ� 
		// Animation Sequence 2D ����� --> Sprite Edit Object�� �ҷ����� Texture��
		// ��, Reverse ��� ���θ� ����ؼ�, Reverse Mode ���, StartPos�� Size�� �����Ѵ�.
		// Image, End ����
		m_SpriteSampled->SetTexture(DestSequence->GetTexture());
		m_SpriteSampled->SetImageStart(NewFrameStartPos);
		m_SpriteSampled->SetImageEnd(NewFrameEndPos);

		m_SpriteCurrentFrame->SetTexture(DestSequence->GetTexture());
		m_SpriteCurrentFrame->SetImageStart(NewFrameStartPos);
		m_SpriteCurrentFrame->SetImageEnd(NewFrameEndPos);
	}
	else
	{
		DragObject->SetStartPos(NewFrameStartPos);
		DragObject->SetEndPos(NewFrameEndPos);

		// m_SpriteSampled Texture ���� 
		NewFrameStartPos.y = DestSequence->GetTexture()->GetHeight() - NewFrameStartPos.y;
		NewFrameEndPos.y = DestSequence->GetTexture()->GetHeight() - NewFrameEndPos.y;

		m_SpriteSampled->SetTexture(DestSequence->GetTexture());
		m_SpriteSampled->SetImageStart(NewFrameStartPos);
		m_SpriteSampled->SetImageEnd(NewFrameEndPos);
	}

}

void CSpriteEditWindow::DeleteAnimationSequence()
{
	// Empty
	if (m_AnimationList->GetItemCount() <= 0)
		return;

	// Not Selected
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	int SelectedSeqIndex = m_AnimationList->GetSelectIndex();

	// Delete Actual Sequence From Animation Instance, Not Sequence
	std::string SelectedSequenceName = m_AnimationList->GetSelectItem();
	bool IsDelete = m_Animation->DeleteAnimationSequence(SelectedSequenceName);

	// ���������� �������� �ʾҴٸ� return;
	if (!IsDelete)
		return;

	// ������ �༮�� ���� ù��° Seq ��� 
	if (SelectedSeqIndex == 0)
	{
		if (m_AnimationList->GetItemCount() > 1)
		{
			// 0 �̶��, ���� 2�� �̻��� Frame�� Add�� ���¶��, ���� ���� �༮, ��, �ٽ� 0�� ����Ű�� �Ѵ� ( 1���� 0�� �� �༮ )
			// �ƹ��͵� ���ϸ� 0�� ����Ű�� �� ���̴�. 
			m_AnimationList->SetSelectIndex(m_AnimationList->GetSelectIndex());

			// Current Animation ������ �ٲ��ش�. (�̰���, ���� Seq �� ������ �ٲ��ش� )
			int NextIdx = m_AnimationList->GetSelectIndex() + 1;
			SelectAnimationSequence(NextIdx, m_AnimationList->GetItem(NextIdx).c_str());
		}
		else
		{
			// �װ� �ƴ϶��, �� ������ �� --> Clear ���ش�.
			ClearAnimationSequence();
			return;
		}
	}
	else
	{
		// 1 �̻��� �����, -1�� ���ش�. ��, ���� ���� �����ϵ��� �����Ѵ�.
		int PrevIdx = m_AnimationList->GetSelectIndex() - 1;
		m_AnimationList->SetSelectIndex(PrevIdx);
		SelectAnimationSequence(PrevIdx, m_AnimationList->GetItem(PrevIdx).c_str());
	}

	// ���� AnimationList���� ������ ������ �����ְ�
	m_AnimationList->DeleteItem(SelectedSeqIndex);

	// Stop Animation
	if (m_Animation)
		m_Animation->Stop();

	return;

}

void CSpriteEditWindow::ClearAnimationSequence()
{
	if (!m_Animation)
		return;

	// ��� Animation���� �����ش�.
	m_Animation->ClearAnimationSequence();

	// Animation List �� Animation FrameList�� ����鵵 ��� �����ش�..
	m_AnimationFrameList->Clear();
	m_AnimationList->Clear();

	// SpriteSampled�� Default UI�� �������ش�.
	m_SpriteSampled->SetTexture("DefaultUI");
}

void CSpriteEditWindow::AddAnimationButton()
{
	// Sprite Object�� ���� ������ �����Ѵ�.
	if (!m_SpriteObject)
		return;

	// Input���κ��� �Է¹��� Text ������ �޾ƿ´�. 
	// Text�� ����ٸ�, �Է� ���� �ʴ´�.
	if (m_AnimInputName->Empty())
		return;

	// Text �ߺ����� 
	const std::string  SequenceName  = m_AnimInputName->GetTextMultibyte();
	const std::string& AnimationName = SequenceName;

	if (m_AnimationList->CheckItem(SequenceName))
		return;

	// Loop, Reverse ���� 
	if (m_NewSeqAnimationLoop->GetSelectIndex() < 0 || m_NewSeqAnimationReverse->GetSelectIndex() < 0)
		return;

	// Animation Sequence 2D ����� --> Sprite Edit Object�� �ҷ����� Texture��
	CSceneResource* Resource      = CSceneManager::GetInst()->GetScene()->GetResource();
	CTexture*       LoadedTexture = m_SpriteObject->GetSpriteComponent()->GetTexture();

	if (!Resource->CreateAnimationSequence2D(SequenceName, LoadedTexture))
		return;

	// Text �߰��ϱ� 
	m_AnimationList->AddItem(SequenceName);

	// Animation ���� �߰� 
	bool Loop    = StringToBool(m_NewSeqAnimationLoop->GetSelectItem());
	bool Reverse = StringToBool(m_NewSeqAnimationReverse->GetSelectItem());
	m_Animation->AddAnimation(SequenceName, AnimationName, Loop, 1.f, 1.f, Reverse);

	// Loop, Reverse ������ �������ش�.
	CAnimationSequence2DData* AddedAnimation = m_Animation->FindAnimationSequence2DData(AnimationName);
	if (m_Animation->GetCurrentAnimation() == AddedAnimation)
	{
		Loop = AddedAnimation->IsLoop();
		Reverse = AddedAnimation->IsReverse();
		m_CurSeqAnimationLoop->SetSelectIndex(Loop ? 0 : 1);
		m_CurSeqAnimationReverse->SetSelectIndex(Reverse ? 0 : 1);
	}


	// Ȥ���� SceneResource�� ������ �߰��� ������ �־��ٸ� �ش� Sequence�� Frame Data ����
	// m_AnimationFrameList�� �ѷ��ش�.
	CAnimationSequence2D* AddedAnimationSequence2D = AddedAnimation->GetAnimationSequence();
	size_t FrameCount = AddedAnimationSequence2D->GetFrameCount();
	for (size_t i = 0; i < FrameCount; i++)
	{
		m_AnimationFrameList->AddItem(std::to_string(i));
	}

	// Sprite Sample�� �ش� ù��° Frame�� Start, End Pos�� �������ش�
	if (FrameCount > 0)
	{
		// Texture ����
		m_SpriteSampled->SetTexture(AddedAnimationSequence2D->GetTexture());

		// Frame ����
		AnimationFrameData FrameData = AddedAnimationSequence2D->GetFrameData(0);
		Vector2 StartPos = FrameData.Start;
		Vector2 EndPos   = FrameData.Start + FrameData.Size;
		m_SpriteSampled->SetImageStart(StartPos);
		m_SpriteSampled->SetImageEnd(EndPos);
	}

	// Reverse Mode�� �ݿ����ش�.
	AddedAnimation->SetFrameReverse(m_Reverse);

	// Animation ����
	m_Animation->Play();
}

void CSpriteEditWindow::AddAnimationFrameButton()
{
	// Animation List�� ������ ���õǾ� �־�� �Ѵ�.
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	// ������ ������ Animation Sequence 2D�� �����ϸ� �� �� ����
	CAnimationSequence2DData* Animation = m_Animation->GetCurrentAnimation();
	if (!Animation)
		return;

	// Animation --> �ߺ��ؼ� Animation�� ���� �ȴ�.
	// Animation->AddFrame(StartPos, EndPos - StartPos);


	float                 XDiff           = -1, YDiff = -1;

	CSceneResource*       Resource        = CSceneManager::GetInst()->GetScene()->GetResource();

	Vector2               FrameStartPos   = CEditorManager::GetInst()->GetDragObject()->GetStartPos();
	Vector2				  FrameEndPos = CEditorManager::GetInst()->GetDragObject()->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(FrameStartPos, FrameEndPos);
	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos   = FinalStartEndPos.second;
	std::string           SequenceName    = m_AnimationList->GetSelectItem();
	CAnimationSequence2D* Sequence        = Resource->FindAnimationSequence2D(SequenceName);
	
	Vector2 FrameSize = FinalEndPos - FinalStartPos;

	// �ش� FramePos ������ Animation Frame ���� �־��ֱ� 
	// Animation Sequence 2D ����� --> Sprite Edit Object�� �ҷ����� Texture��
	// ��, Reverse ��� ���θ� ����ؼ�, Reverse Mode ���, StartPos�� Size�� �����Ѵ�.
	if (m_Reverse)
	{
		Vector2 ReverseStartPos = Vector2(FinalStartPos.x * -1.f, FinalStartPos.y);
		Vector2 ReverseSize = Vector2(FrameSize.x * -1.f, FrameSize.y);
		Sequence->AddFrame(ReverseStartPos, ReverseSize);
	}
	else
	{
		Sequence->AddFrame(FinalStartPos, FrameSize);
	}

	// ���� SequenceData ���� Reverse ���θ� �ݿ��Ѵ�.
	Animation->SetFrameReverse(m_Reverse);

	// Frame List Box�� �־��ֱ� 
	char FrameName[1024] = {};
	int  AnimItemCount   = m_AnimationFrameList->GetItemCount();
	sprintf_s(FrameName, "%d", AnimItemCount);
	m_AnimationFrameList->AddItem(FrameName);

	// Sampled�� Image �������ֱ� 
	CSpriteComponent* SpriteObjectComponent = dynamic_cast<CSpriteComponent*>(m_SpriteObject->GetRootComponent());
	m_SpriteSampled->SetTexture(SpriteObjectComponent->GetTextureName());

	// Image, End ���� 
	m_SpriteSampled->SetImageStart(FinalStartPos);
	m_SpriteSampled->SetImageEnd(FinalEndPos);

	// AnimationFrameListBox ���� SelectIdx ������ �ٲ��ֱ� 
	m_AnimationFrameList->SetSelectIndex(AnimItemCount);
	
	// Stop Animation
	m_Animation->Play();
}

void CSpriteEditWindow::DeleteFrameButton()
{
	// Empty
	if (m_AnimationList->GetItemCount() <= 0)
		return;
	if (m_AnimationFrameList->GetItemCount() <= 0)
		return;

	// Not Selected
	if (m_AnimationList->GetSelectIndex() < 0)
		return;
	if (m_AnimationFrameList->GetSelectIndex() < 0)
		return;

	// Delete Acual Frame Data
	CSceneResource*       Resource         = CSceneManager::GetInst()->GetScene()->GetResource();
	CAnimationSequence2D* Sequence         = Resource->FindAnimationSequence2D(m_AnimationList->GetSelectItem());
	int                   SelectedFrameIdx = std::stoi(m_AnimationFrameList->GetSelectItem());
	Sequence->DeleteFrame(SelectedFrameIdx);

	// Update Select Idx Info Of m_AnimationFrameList
	int SelectedFrameListIndex = m_AnimationFrameList->GetSelectIndex();


	if (SelectedFrameIdx == 0)
	{
		if (m_AnimationFrameList->GetItemCount() > 1)
		{
			// 0 �̶��, ���� 2�� �̻��� Frame�� Add�� ���¶��, ���� ���� �༮, ��, �ٽ� 0�� ����Ű�� �Ѵ� ( 1���� 0�� �� �༮ )
			// �ƹ��͵� ���ϸ� 0�� ����Ű�� �� ���̴�. 
			m_AnimationFrameList->SetSelectIndex(m_AnimationFrameList->GetSelectIndex());
		}
		else
		{
			// �װ� �ƴ϶��, �� ������ �� --> Clear ���ش�.
			ClearFrameButton();
			return;
		}
	}
	else
	{
		// 1 �̻��� �����, -1�� ���ش�. 
		m_AnimationFrameList->SetSelectIndex(m_AnimationFrameList->GetSelectIndex() - 1);
	}

	// Set FrameData To 0
	if (m_Animation && m_Animation->GetCurrentAnimation())
	{
		m_Animation->GetCurrentAnimation()->SetFrame(0);

		int UpdatedSelectIndex = m_AnimationFrameList->GetSelectIndex();
		AnimationFrameData NFrameData = m_Animation->GetCurrentAnimation()->GetFrameData(UpdatedSelectIndex);
		Vector2 StartPos = NFrameData.Start;
		Vector2 EndPos   = NFrameData.Start + NFrameData.Size;

		// Update Start, End Pos In m_Sampled Image
		m_SpriteSampled->SetImageStart(StartPos.x, StartPos.y);
		m_SpriteSampled->SetImageEnd(EndPos.x, EndPos.y);

		// Update DragObj Start, End Info
		StartPos.y = (float)Sequence->GetTexture()->GetHeight() - StartPos.y;
		EndPos.y = (float)Sequence->GetTexture()->GetHeight() - EndPos.y;
		CEditorManager::GetInst()->GetDragObject()->SetStartPos(StartPos);
		CEditorManager::GetInst()->GetDragObject()->SetEndPos(EndPos);


	}

	// If No Frame Left, Set Default Texture
	if (Sequence->GetFrameCount() == 0)
	{
		m_SpriteSampled->SetTexture("DefaultUI");
	}

	// Delete Text
	m_AnimationFrameList->DeleteItem(SelectedFrameIdx);

	// Update Names Of FrameDatas in AnimationFrameList
	int ListNums = m_AnimationFrameList->GetItemCount();
	for (int i = 0; i < ListNums; i++)
	{
		if (i < SelectedFrameIdx)
			continue;
		m_AnimationFrameList->SetItem(i, std::to_string(i));
	}

	// Stop Animation
	if (m_Animation)
		m_Animation->Stop();

	return;
}

void CSpriteEditWindow::ClearFrameButton()
{
	// Empty
	if (m_AnimationFrameList->GetItemCount() <= 0)
		return;
	if (m_AnimationList->GetItemCount() < 0)
		return;

	// Not Selected 
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	// Clear Frame of Sequence
	CSceneResource*       Resource = CSceneManager::GetInst()->GetScene()->GetResource();
	CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetSelectItem());
	Sequence->ClearFrame();

	// Animation Sequence2D Data Set Frame To 0
	m_Animation->GetCurrentAnimation()->ResetFrame();

	// Clear Texts
	m_AnimationFrameList->Clear();

	// Set Idx 
	m_AnimationFrameList->SetSelectIndex(-1);

	// Set Default Image 
	m_SpriteSampled->SetTexture("DefaultUI");

	// Update Drag Object Pos
	CEditorManager::GetInst()->GetDragObject()->SetStartPos(Vector2(0.f, 0.f));
	CEditorManager::GetInst()->GetDragObject()->SetEndPos(Vector2(0.f, 0.f));
}

void CSpriteEditWindow::EditFrameButton()
{
	// Empty
	if (m_StartFramePosXInput->Empty() || m_StartFramePosYInput->Empty())
		return;
	if (m_EndFramePosXInput->Empty() || m_EndFramePosYInput->Empty())
		return;

	// Not Selected 
	if (m_AnimationList->GetSelectIndex() < 0 || m_AnimationList->GetSelectIndex() < 0)
		return;

	const std::string StartFrameX = m_StartFramePosXInput->GetTextUTF8();
	const std::string StartFrameY = m_StartFramePosYInput->GetTextUTF8();
	const std::string EndFrameX   = m_EndFramePosXInput->GetTextUTF8();
	const std::string EndFrameY   = m_EndFramePosYInput->GetTextUTF8();

	if (!CheckIfStringIsDigit(StartFrameX) || !CheckIfStringIsDigit(StartFrameY))
		return;
	if (!CheckIfStringIsDigit(EndFrameX) || !CheckIfStringIsDigit(EndFrameY))
		return;

	CSceneResource*       Resource     = CSceneManager::GetInst()->GetScene()->GetResource();
	CAnimationSequence2D* Sequence     = Resource->FindAnimationSequence2D(m_AnimationList->GetSelectItem());
	Vector2               SequenceSize = Vector2(static_cast<float>(Sequence->GetTexture()->GetWidth()),
	                                             static_cast<float>(Sequence->GetTexture()->GetHeight()));

	float FloatStartFrameX = std::stof(StartFrameY);
	float FloatStartFrameY = std::stof(StartFrameX);

	float FloatEndFrameX = std::stof(EndFrameX);
	float FloatEndFrameY = std::stof(EndFrameY);

	float DStartFrameX = FloatStartFrameX < FloatEndFrameX ? FloatStartFrameX : FloatEndFrameX;
	float DStartFrameY = FloatStartFrameY < FloatEndFrameY ? FloatStartFrameY : FloatEndFrameY;

	float DEndFrameX = FloatStartFrameX > FloatEndFrameX ? FloatStartFrameX : FloatEndFrameX;
	float DEndFrameY = FloatStartFrameY > FloatEndFrameY ? FloatStartFrameY : FloatEndFrameY;


	// ���� �ʰ� �˻� 
	DStartFrameX = DStartFrameX < SequenceSize.x ? DStartFrameX : SequenceSize.x;
	DStartFrameY = DStartFrameY < SequenceSize.y ? DStartFrameY : SequenceSize.y;
	DEndFrameX   = DEndFrameX < SequenceSize.x ? DEndFrameX : SequenceSize.x;
	DEndFrameY   = DEndFrameY < SequenceSize.y ? DEndFrameY : SequenceSize.y;

	// ���� Frame �ݿ�
	Sequence->SetFrame(std::stoi(m_AnimationFrameList->GetSelectItem()),
	                   DStartFrameX, DStartFrameY, DEndFrameX - DStartFrameX, DEndFrameY - DStartFrameY);

	// Sprite �̹����� �ݿ�
	m_SpriteSampled->SetImageStart(DStartFrameX, DStartFrameY);
	m_SpriteSampled->SetImageEnd(DEndFrameX, DEndFrameY);
}

void CSpriteEditWindow::PlayAnimationButton()
{
	m_Animation->Play();
}

void CSpriteEditWindow::StopAnimationButton()
{
	m_Animation->Stop();
}

void CSpriteEditWindow::SetNewAnimSequenceLoop(int Index, const char* Text)
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;
	m_NewSeqAnimationLoop->SetSelectIndex(Index);
}

void CSpriteEditWindow::SetNewAnimSequenceReverse(int Index, const char* Text)
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;
	m_NewSeqAnimationReverse->SetSelectIndex(1 - Index);
}

void CSpriteEditWindow::SetCurAnimSequenceLoop(int Index, const char* Text)
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;
	m_CurSeqAnimationLoop->SetSelectIndex(Index);
	m_Animation->GetCurrentAnimation()->SetLoop(true);
}

void CSpriteEditWindow::SetCurAnimSequenceReverse(int Index, const char* Text)
{
		if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;
	m_CurSeqAnimationReverse->SetSelectIndex(Index);
	m_Animation->GetCurrentAnimation()->SetReverse(true);
}

void CSpriteEditWindow::SaveSequence()
{
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile    = {};
	OpenFile.lStructSize     = sizeof(OPENFILENAME);
	OpenFile.hwndOwner       = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter     = TEXT("�������\0*.*\0.Sequence File\0*.sqc");
	OpenFile.lpstrFile       = FilePath;
	OpenFile.nMaxFile        = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SEQUENCE_PATH)->Path;

	if (GetSaveFileName(&OpenFile) != 0)
	{
		// ���忡 �����ߴٸ�
		char FilePathMultibyte[MAX_PATH] = {};
		int  ConvertLength               = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FilePathMultibyte, ConvertLength, nullptr, nullptr);

		m_Animation->GetCurrentAnimation()->GetAnimationSequence()->SaveFullPath(FilePathMultibyte);

		// GameEngine�� Bin, Animation Folder ���� �����Ѵ�.
		std::string ExtraFolderName;
		if (m_CandyCrushVersion)
			ExtraFolderName = CANDY_SEQUENCE_PATH;
		else
			ExtraFolderName = ENGINE_SEQUENCE_PATH;

		// Candy Crush Version�� �� �ѹ� ���õǸ�, �ٽ� ������ �ʱ�ȭ
		UndoCandyCrushMode();

		const PathInfo* EngineSequenceFolder = CPathManager::GetInst()->FindPath(ExtraFolderName);

		// ���� �̸��� �̾Ƴ���.
		char SavedFileName[MAX_PATH] = {};
		char SavedExt[_MAX_EXT] = {};
		_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, SavedFileName, MAX_PATH, SavedExt, _MAX_EXT);

		// ���� GameEngine ��θ� �����.
		char SavedGameEnginePath[MAX_PATH] = {};
		strcpy_s(SavedGameEnginePath, EngineSequenceFolder->PathMultibyte);
		strcat_s(SavedGameEnginePath, SavedFileName);
		strcat_s(SavedGameEnginePath, SavedExt);

		// ���� ����Ǵ� ��ο� �ٸ��ٸ�, GameEngine �ʿ��� �����Ѵ�.
		if (strcmp(EngineSequenceFolder->PathMultibyte, FilePathMultibyte) != 0)
		{
			m_Animation->GetCurrentAnimation()->GetAnimationSequence()->SaveFullPath(SavedGameEnginePath);
		}
	}
}

void CSpriteEditWindow::LoadSequence()
{
	TCHAR FullPath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFile = FullPath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0.Sequence File\0*.sqc");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SEQUENCE_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FullPathMultibyte[MAX_PATH] = {};

		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, 0, 0);

		std::string SequenceName;
		CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();
		Resource->LoadSequence2DFullPath(SequenceName, FullPathMultibyte);

		// ���⿡�� ��� ���׵��� UI�� �ѷ��ֱ�
		// �̸����� Sequence�� ��������
		CAnimationSequence2D* LoadedSequence = Resource->FindAnimationSequence2D(SequenceName);

		// Sequence�� Texture�� �ε� ���� �ʾҴٸ�, return;
		if (!LoadedSequence->GetTexture())
			return;

		// ���� �̸��� �̹� �����ϸ� X
		if (m_AnimationList->CheckItem(SequenceName))
			return;

		// �ش� �̸��� m_AnimationList �� �߰����ְ�
		m_AnimationList->AddItem(SequenceName);

		// �ش� �̸��� Ŭ���� ���·� ������ְ�
		m_AnimationList->SetSelectIndex(m_AnimationList->GetItemCount() - 1);

		// Animation List�� ���ٸ� �����Ѵ�
		if (!m_Animation)
		{
			m_Animation = new CAnimationSequence2DInstance;
			m_Animation->Init();
		}

		m_Animation->AddAnimation(SequenceName, SequenceName);

		// �ش� Seq�� Sprite Edit Object�� ����
		if (!m_SpriteObject)
		{
			m_SpriteObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
			m_SpriteObject->SetEditWindow(this);
		}
		m_SpriteObject->SetMaterialTexture(LoadedSequence->GetTexture());
		m_SpriteObject->SetAnimationNewTexture(LoadedSequence->GetTexture());
		m_SpriteObject->SetTextureWorldScale();

		// �ش� Seq�� m_Sprite �� �߰��ϱ�
		m_Sprite->SetTexture(LoadedSequence->GetTexture());

		// m_Sprite �� Start, EndPos ���� ?

		// ���ο� Seq�� Load �ϴ� ���̹Ƿ� m_AnimationFrameList �� ����ش�
		m_AnimationFrameList->Clear();

		// �ش� Seq�� Animation ���� m_AnimationFrameList�� �߰�
		size_t  Size = LoadedSequence->GetFrameCount();
		for (size_t i = 0; i < Size; i++)
		{
			m_AnimationFrameList->AddItem(std::to_string(i));
		}

		// Loop, Reverse ����
		// bool Loop = LoadedSequence->Get
		//	m_NewSeqAnimationLoop
		//	m_NewSeqAnimationReverse

		// Frame�� �ִٸ� 0���� ����
		if (Size > 0)
		{
			m_AnimationFrameList->SetSelectIndex(0);

			// �ش� Frame�� St, End�� �ݿ��Ͽ� SampledSprite �� �����صα�
			AnimationFrameData AnimFrame = LoadedSequence->GetFrameData(m_AnimationFrameList->GetSelectIndex());

			Vector2 StartPos = AnimFrame.Start;
			Vector2 EndPos   = StartPos + AnimFrame.Size;

			m_SpriteSampled->SetTexture(LoadedSequence->GetTexture());
			m_SpriteSampled->SetImageStart(StartPos);
			m_SpriteSampled->SetImageEnd(EndPos);

			m_SpriteCurrentFrame->SetTexture(LoadedSequence->GetTexture());
			m_SpriteCurrentFrame->SetImageStart(StartPos);
			m_SpriteCurrentFrame->SetImageEnd(EndPos);

			// �ش� Frame�� St, End�� �̿��Ͽ� DragObject �� �׸���
			StartPos.y = LoadedSequence->GetTexture()->GetHeight() - StartPos.y;
			EndPos.y = LoadedSequence->GetTexture()->GetHeight() - EndPos.y;
			if (!CEditorManager::GetInst()->GetDragObject())
			{
				// �ʱ�ȭ �ȵǾ� �ִٸ� �ʱ�ȭ 
				CEditorManager::GetInst()->SetEditMode(EditMode::Sprite);
			}
			CEditorManager::GetInst()->GetDragObject()->SetStartPos(StartPos);
			CEditorManager::GetInst()->GetDragObject()->SetEndPos(EndPos);
		}

	}
}

void CSpriteEditWindow::SaveAnimation()
{
	if (!m_Animation)
		return;

	if (!m_Animation->GetCurrentAnimation())
		return;

	TCHAR FiilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize     = sizeof(OPENFILENAME);
	OpenFile.hwndOwner       = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter     = TEXT("All Files\0*.*\0.Animation File\0*.anim");
	OpenFile.lpstrFile       = FiilePath;
	OpenFile.nMaxFile        = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int  ConvertLength               = WideCharToMultiByte(CP_ACP, 0, FiilePath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, FiilePath, -1, FilePathMultibyte, ConvertLength, nullptr, nullptr);

		m_Animation->SaveFullPath(FilePathMultibyte);

		// GameEngine�� Bin, Animation Folder ���� �����Ѵ�.
		std::string ExtraFolderName;
		if (m_CandyCrushVersion)
			ExtraFolderName = CANDY_ANIMATION_PATH;
		else
			ExtraFolderName = ENGINE_ANIMATION_PATH;

		UndoCandyCrushMode();

		// GameEngine�� Bin, Animation Folder ���� �����Ѵ�.
		const PathInfo* EngineSequenceFolder = CPathManager::GetInst()->FindPath(ExtraFolderName);

		// ���� �̸��� �̾Ƴ���.
		char SavedFileName[MAX_PATH] = {};
		char SavedExt[_MAX_EXT] = {};
		_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, SavedFileName, MAX_PATH, SavedExt, _MAX_EXT);

		// ���� GameEngine ��θ� �����.
		char SavedGameEnginePath[MAX_PATH] = {};
		strcpy_s(SavedGameEnginePath, EngineSequenceFolder->PathMultibyte);
		strcat_s(SavedGameEnginePath, SavedFileName);
		strcat_s(SavedGameEnginePath, SavedExt);

		// ���� ����Ǵ� ��ο� �ٸ��ٸ�, GameEngine �ʿ��� �����Ѵ�.
		if (strcmp(EngineSequenceFolder->PathMultibyte, FilePathMultibyte) != 0)
		{
			m_Animation->SaveFullPath(SavedGameEnginePath);
		}
	}
}

void CSpriteEditWindow::LoadAnimation()
{
	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		if (!m_Animation)
			m_Animation = new CAnimationSequence2DInstance;
		else
		{
			// ���� Animation List�� �������� , ��, ���� Animation�� Added �Ǿ��� ��� Sequence ������ �����ش�
			ClearAnimationSequence();
		}

		m_Animation->LoadFullPath(FilePathMultibyte);

		// CurrentAnimation �� ���ٸ�,
		if (!m_Animation->GetCurrentAnimation())
			return;

		CTexture* AnimTexture = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetTexture();

		// Current Animation�� �ִٸ�, �ش� Animation �� Texture�� SpriteEditObject�� Texture�� �����Ѵ�.
		// �ش� Seq�� Sprite Edit Object�� ����
		if (!m_SpriteObject)
		{
			m_SpriteObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
			m_SpriteObject->SetEditWindow(this);
		}
		m_SpriteObject->SetMaterialTexture(AnimTexture);
		m_SpriteObject->SetAnimationNewTexture(AnimTexture);
		m_SpriteObject->SetTextureWorldScale();

		// ����������, Sprite�� Texture�� �����Ѵ�.
		m_Sprite->SetTexture(AnimTexture);

		// AnimList ��θ� �����ش�
		m_AnimationList->Clear();

		// Animation Key Name ������ �Ѵ�. (Ȥ�ó� �̸��� �߸� ����Ǿ� ���� ���� �����Ƿ� )
		m_Animation->AdjustSequence2DKeyName();

		// AnimList �� ��� Seq ������ �߰����ش�
		int Size = m_Animation->GetAnimationCount();
		std::vector<std::string> vecSeqNames;

		m_Animation->GatherSequenceNames(vecSeqNames);

		for (int i = 0; i < Size; i++)
		{
			m_AnimationList->AddItem(vecSeqNames[i]);
		}

		// ���� Scene�� ��� Sequence2D ������ �߰��Ѵ�.
		m_Animation->AddSequence2DToScene();

		// ���� Scene�� ������ m_Scene���� �������ش�
		m_Animation->SetScene(CSceneManager::GetInst()->GetScene());

		// CurrentAnimation�� ���õ� Sequence�� �������ش�
		int CurAnimIdx = m_Animation->GetCurrentAnimationOrder();
		if (CurAnimIdx == -1)
			return;

		m_AnimationList->SetSelectIndex(CurAnimIdx);

		// Animation Frame List�� ����ش�.
		m_AnimationFrameList->Clear();

		int FrameCount = (int)m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount();

		if (FrameCount > 0)
		{
			// �ش� Seq�� Frame Data ����� Animation FrameList�� ä���ش�.
			for (int i = 0; i < FrameCount; i++)
			{
				m_AnimationFrameList->AddItem(std::to_string(i));
			}

			// Frame�� �����Ѵٸ�, SpriteSampled�� Texture�� ���� Current Animation�� Texture�� �������ش�.
			m_SpriteSampled->SetTexture(AnimTexture);
			m_SpriteCurrentFrame->SetTexture(AnimTexture);

			// ù��° Frame�� SpriteSampled �� Image Start, End�� �����Ѵ�.
			AnimationFrameData FirstFrame = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(0);
			Vector2 FrameStartPos = FirstFrame.Start;
			Vector2 FrameEndPos   = FirstFrame.Start + FirstFrame.Size;

			m_SpriteSampled->SetImageStart(FrameStartPos);
			m_SpriteSampled->SetImageEnd(FrameEndPos);
			m_SpriteCurrentFrame->SetImageStart(FrameStartPos);
			m_SpriteCurrentFrame->SetImageEnd(FrameEndPos);

			// Frame�� �����Ѵٸ�, ù��° Frame�� ������ ���·� �д�.
			m_AnimationFrameList->SetSelectIndex(0);

			// Drag Object ����
			// �ش� Frame�� St, End�� �̿��Ͽ� DragObject �� �׸���
			FrameStartPos.y = AnimTexture->GetHeight() - FrameStartPos.y;
			FrameEndPos.y = AnimTexture->GetHeight() - FrameEndPos.y;

			if (!CEditorManager::GetInst()->GetDragObject())
			{
				// �ʱ�ȭ �ȵǾ� �ִٸ� �ʱ�ȭ 
				CEditorManager::GetInst()->SetEditMode(EditMode::Sprite);
			}
			CEditorManager::GetInst()->GetDragObject()->SetStartPos(FrameStartPos);
			CEditorManager::GetInst()->GetDragObject()->SetEndPos(FrameEndPos);

		}

		// Loop, Reverse ������ ���ش�.
		bool Loop     = m_Animation->GetCurrentAnimation()->IsLoop();
		bool Reverse = m_Animation->GetCurrentAnimation()->IsReverse();
		m_CurSeqAnimationLoop->SetSelectIndex(Loop ? 0 : 1);
		m_CurSeqAnimationReverse->SetSelectIndex(Reverse ? 0 : 1);

		// Animation�� �����Ѵ�..
		m_Animation->Play();

	}
}

void CSpriteEditWindow::SelectAnimationSequence(int Index, const char* TextureName)
{
	// �ش� idx�� Sequence ���� ��������
	// SceneResource�� �ƴ϶�, �ش� m_Animation�� ���� �����;� �ϴ� �� �ƴѰ� ?
	// CSceneResource*       Resource               = CSceneManager::GetInst()->GetScene()->GetResource();
	if (!m_Animation) //
		return;

	std::string           ChangedSequenceName    = m_AnimationList->GetItem(Index);

	// CAnimationSequence2D* ChangedSequence        = Resource->FindAnimationSequence2DData(ChangedSequenceName);
	CAnimationSequence2D* ChangedSequence        = m_Animation->FindAnimationSequence2DData(ChangedSequenceName)->GetAnimationSequence();

	if (!ChangedSequence)
		return;

	CTexture*             ChangedSequenceTexture = ChangedSequence->GetTexture();
	Vector2               FrameStartPos          = CEditorManager::GetInst()->GetDragObject()->GetStartPos();
	int                   MaterialTextureIdx     = 0;

	// SpriteObject �� Texture �ٲ��ֱ�
	m_SpriteObject->GetSpriteComponent()->SetTexture(MaterialTextureIdx, ChangedSequenceTexture);
	m_SpriteObject->SetWorldScale((float)(ChangedSequenceTexture->GetWidth()),(float)(ChangedSequenceTexture->GetHeight()), 1.f);

	// ���� �ִϸ��̼����� ���� 
	m_Animation->SetCurrentAnimation(ChangedSequenceName);

	// Current Animation�� ���� Loop, Reverse ����
	bool Loop = m_Animation->GetCurrentAnimation()->IsLoop();
	bool Reverse = m_Animation->GetCurrentAnimation()->IsReverse();
	m_CurSeqAnimationLoop->SetSelectIndex(Loop ? 0 : 1);
	m_CurSeqAnimationReverse->SetSelectIndex(Reverse ? 0 : 1);

	// m_Sprite�� Texture �ٲ��ֱ� 
	m_Sprite->SetTexture(ChangedSequenceTexture);

	// Clear Texts
	m_AnimationFrameList->Clear();

	// �ش� Sequence�� ���ؼ� �߰��س��� Frame������ ���ٸ� -- Clear ����
	if (ChangedSequence->GetFrameCount() <= 0)
	{
		// Set Idx 
		m_AnimationFrameList->SetSelectIndex(-1);

		// Animation Sequence2D Data Set Frame To 0
		m_Animation->GetCurrentAnimation()->ResetFrame();

		// Set Default Image 
		m_SpriteSampled->SetTexture("DefaultUI");

		// Update Drag Object Pos
		CEditorManager::GetInst()->GetDragObject()->SetStartPos(Vector2(0.f, 0.f));
		CEditorManager::GetInst()->GetDragObject()->SetEndPos(Vector2(0.f, 0.f));
		return;
	}

	// ���ο� Frame ������ �߰��ϱ� 
	char IndexChar[1024];
	for (int i = 0; i < ChangedSequence->GetFrameCount(); i++)
	{
		sprintf_s(IndexChar, "%d", i);
		m_AnimationFrameList->AddItem(IndexChar);
	}

	// ù��° AnimationFrame���� ������ ������ ����
	m_AnimationFrameList->SetSelectIndex(0);

	// ù��° Seq�� �����ϱ� 
	AnimationFrameData NewFrameData = ChangedSequence->GetFrameData(0);
	Vector2 NewFrameStartPos = NewFrameData.Start;
	Vector2 NewFrameEndPos   = NewFrameData.Start + NewFrameData.Size;

	// �ش� Sequece �� Reverse����, �ƴ����� ����, SpriteEdit Object�� ���ؼ��� Mode�� Reverse�� �������ֱ�
	bool SeqFrameReverse = m_Animation->FindAnimationSequence2DData(ChangedSequenceName)->IsFrameReverse();
	if (SeqFrameReverse)
		SetReverseMode();
	else
		SetNormalMode();

	// m_SpriteSampled Texture ���� 
	m_SpriteSampled->SetTexture(ChangedSequenceTexture);
	
	// DragObject�� Image Pos ����
	NewFrameStartPos.y = ChangedSequenceTexture->GetHeight() - NewFrameStartPos.y;
	NewFrameEndPos.y = ChangedSequenceTexture->GetHeight() - NewFrameEndPos.y;

	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	Vector2 TextureSize = Vector2((float)ChangedSequence->GetTexture()->GetWidth(), (float)ChangedSequence->GetTexture()->GetHeight());

	if (m_Reverse)
	{
		// DragObject�� Image Pos ����
		Vector2 ReverseStartPos = Vector2(NewFrameStartPos.x * -1.f, NewFrameStartPos.y);
		Vector2 ReverseEndPos   = Vector2(NewFrameEndPos.x * -1.f, NewFrameEndPos.y);
		DragObject->SetStartPos(ReverseStartPos);
		DragObject->SetEndPos(ReverseEndPos);

		// m_SpriteSampled Texture ����
// StartPos.y = ImageSize.x - StartPos.y;
		NewFrameStartPos.y = TextureSize.y - NewFrameStartPos.y;
		// EndPos.y = ImageSize.y - EndPos.y;
		NewFrameEndPos.y = TextureSize.y - NewFrameEndPos.y;

		m_SpriteSampled->SetImageStart(NewFrameStartPos);
		m_SpriteSampled->SetImageEnd(NewFrameEndPos);

		m_SpriteCurrentFrame->SetImageStart(NewFrameStartPos);
		m_SpriteCurrentFrame->SetImageEnd(NewFrameEndPos);
	}
	else
	{
		DragObject->SetStartPos(NewFrameStartPos);
		DragObject->SetEndPos(NewFrameEndPos);

		NewFrameStartPos.y = TextureSize.y - NewFrameStartPos.y;
		NewFrameEndPos.y = TextureSize.y - NewFrameEndPos.y;

		m_SpriteSampled->SetImageStart(NewFrameStartPos);
		m_SpriteSampled->SetImageEnd(NewFrameEndPos);

		m_SpriteCurrentFrame->SetImageStart(NewFrameStartPos);
		m_SpriteCurrentFrame->SetImageEnd(NewFrameEndPos);
	}

}

void CSpriteEditWindow::SelectAnimationFrame(int Index, const char* Name)
{
	// SceneResource�� �ƴ϶�, �ش� m_Animation ���κ��� ã�� ���̴� 
	// CSceneResource*       Resource        = CSceneManager::GetInst()->GetScene()->GetResource();
	std::string           SequenceName    = m_AnimationList->GetSelectItem();
	// CAnimationSequence2D* Sequence        = Resource->FindAnimationSequence2DData(SequenceName);
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;
	CTexture*             SequenceTexture = Sequence->GetTexture();
	AnimationFrameData    FrameData       = Sequence->GetFrameData(Index);

	Vector2 StartPos = FrameData.Start;
	Vector2 EndPos = FrameData.Start + FrameData.Size;

	// SpriteSampled�� �ٲ��ֱ� 
	m_SpriteSampled->SetImageStart(StartPos);
	m_SpriteSampled->SetImageEnd(EndPos);

	// Drag Object Setting
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();

	Vector2 ImageSize = Vector2(static_cast<float>(SequenceTexture->GetWidth()),
	                            static_cast<float>(SequenceTexture->GetHeight()));

	StartPos.y = ImageSize.y - StartPos.y;
	EndPos.y   = ImageSize.y - EndPos.y;

	Vector2 SpriteObject2DPos = Vector2(m_SpriteObject->GetWorldPos().x, m_SpriteObject->GetWorldPos().y);
	StartPos += SpriteObject2DPos;
	EndPos += SpriteObject2DPos;

	// �ش� Sequece �� Reverse����, �ƴ����� ����, SpriteEdit Object�� ���ؼ��� Mode�� Reverse�� �������ֱ�
	bool SeqFrameReverse = m_Animation->FindAnimationSequence2DData(SequenceName)->IsFrameReverse();
	if (SeqFrameReverse)
		SetReverseMode();
	else
		SetNormalMode();

	if (m_Reverse)
	{
		// DragObject�� Image Pos ����
		Vector2 ReverseStartPos = Vector2(StartPos.x * -1.f, StartPos.y);
		Vector2 ReverseEndPos = Vector2(EndPos.x * -1.f, EndPos.y);
		DragObject->SetStartPos(ReverseStartPos);
		DragObject->SetEndPos(ReverseEndPos);

		// m_SpriteSampled Texture ����
		// StartPos.y = ImageSize.x - StartPos.y;
		StartPos.y = ImageSize.y - StartPos.y;
		// EndPos.y = ImageSize.y - EndPos.y;
		EndPos.y = ImageSize.y - EndPos.y;

		m_SpriteCurrentFrame->SetImageStart(StartPos);
		m_SpriteCurrentFrame->SetImageEnd(EndPos);
	}
	else
	{
		if (StartPos.x < 0 && EndPos.x < 0)
		{
			DragObject->SetStartPos(Vector2(StartPos.x * -1.f, StartPos.y));
			DragObject->SetEndPos(Vector2(EndPos.x * -1.f, EndPos.y));
		}
		else
		{
			DragObject->SetStartPos(StartPos);
			DragObject->SetEndPos(EndPos);
		}
	}


	// Ŭ���� Animation �����ֱ�
	m_Animation->Stop();
}

void CSpriteEditWindow::SetDragObjectToRightEnd()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos   = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);
	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	DragObject->SetStartPos(FinalStartPos);
	DragObject->SetEndPos(Vector2(TextureSize.x - 0.1f, FinalEndPos.y));

}

void CSpriteEditWindow::SetDragObjectToLeftEnd()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);

	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	DragObject->SetStartPos(Vector2(0.1f, FinalStartPos.y));
	DragObject->SetEndPos(FinalEndPos);
}

void CSpriteEditWindow::SetDragObjectToTop()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);
	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	DragObject->SetStartPos(Vector2(FinalStartPos.x, TextureSize.y - 0.1f));
	DragObject->SetEndPos(FinalEndPos);
}

void CSpriteEditWindow::SetDragObjectToBottom()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	// Texture�� �����´�.
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);

	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	DragObject->SetStartPos(FinalStartPos);
	DragObject->SetEndPos(Vector2(FinalEndPos.x, 0.1f));
}

void CSpriteEditWindow::SetDragObjectTexture()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	// Texture�� �����´�.
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	DragObject->SetStartPos(Vector2(0.1f, (float)TextureSize.y));
	DragObject->SetEndPos(Vector2((float)TextureSize.x, 0.1f));
}

void CSpriteEditWindow::DivideFrameWidthAndAdd()
{
	int DivideNumber = m_AddDivideNumberInput->GetValueInt();
	if (DivideNumber <= 0)
		return;

	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	// Sequence�� ���õ��� �ʾҴٸ� return;
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	
	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);

	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	Vector2 FrameStartPos = FinalStartPos;
	Vector2 InitStartPos = FinalStartPos;

	float PartWidth  = (FinalEndPos.x - FinalStartPos.x) / (float)DivideNumber;
	float PartHeight = (FinalEndPos.y - FinalStartPos.y);
	Vector2 PartSize = Vector2(PartWidth, PartHeight);

	for (int i = 0; i < DivideNumber; i++)
	{
		// �ش� FramePos ������ Animation Frame ���� �־��ֱ�
		FrameStartPos.x = InitStartPos.x + (i * PartWidth);

		Sequence->AddFrame(FrameStartPos, PartSize);

		// Frame List Box�� �־��ֱ� 
		char FrameName[1024] = {};
		int  AnimItemCount = m_AnimationFrameList->GetItemCount();
		sprintf_s(FrameName, "%d", AnimItemCount);
		m_AnimationFrameList->AddItem(FrameName);

		// Sampled�� Image �������ֱ� 
		CSpriteComponent* SpriteObjectComponent = dynamic_cast<CSpriteComponent*>(m_SpriteObject->GetRootComponent());
		m_SpriteSampled->SetTexture(SpriteObjectComponent->GetTextureName());

		// Image, End ���� 
		m_SpriteSampled->SetImageStart(FrameStartPos);
		m_SpriteSampled->SetImageEnd(FrameStartPos + PartSize);

		// AnimationFrameListBox ���� SelectIdx ������ �ٲ��ֱ� 
		m_AnimationFrameList->SetSelectIndex(AnimItemCount);

	}
	
	// Animation�� Play 
	CAnimationSequence2DData* Animation = m_Animation->GetCurrentAnimation();
	if (!Animation)
		return;

	m_Animation->Play();

}

void CSpriteEditWindow::DivideFrameHeightAndAdd()
{
	int DivideNumber = m_AddDivideNumberInput->GetValueInt();
	if (DivideNumber <= 0)
		return;

	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	// Sequence�� ���õ��� �ʾҴٸ� return;
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());


	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);

	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	Vector2 FrameStartPos = FinalStartPos;
	Vector2 InitStartPos = FinalStartPos;

	float PartHeight = (FinalEndPos.y - FinalStartPos.y) / (float)DivideNumber;
	float PartWidth = (FinalEndPos.x - FinalStartPos.x);
	Vector2 PartSize = Vector2(PartWidth, PartHeight);

	for (int i = 0; i < DivideNumber; i++)
	{
		// �ش� FramePos ������ Animation Frame ���� �־��ֱ�
		FrameStartPos.y = InitStartPos.y + (i * PartHeight);

		Sequence->AddFrame(FrameStartPos, PartSize);

		// Frame List Box�� �־��ֱ� 
		char FrameName[1024] = {};
		int  AnimItemCount = m_AnimationFrameList->GetItemCount();
		sprintf_s(FrameName, "%d", AnimItemCount);
		m_AnimationFrameList->AddItem(FrameName);

		// Sampled�� Image �������ֱ� 
		CSpriteComponent* SpriteObjectComponent = dynamic_cast<CSpriteComponent*>(m_SpriteObject->GetRootComponent());
		m_SpriteSampled->SetTexture(SpriteObjectComponent->GetTextureName());

		// Image, End ���� 
		m_SpriteSampled->SetImageStart(FrameStartPos);
		m_SpriteSampled->SetImageEnd(FrameStartPos + PartSize);

		// AnimationFrameListBox ���� SelectIdx ������ �ٲ��ֱ� 
		m_AnimationFrameList->SetSelectIndex(AnimItemCount);
	}

	// Animation�� Play 
	CAnimationSequence2DData* Animation = m_Animation->GetCurrentAnimation();
	if (!Animation)
		return;

	m_Animation->Play();
}

void CSpriteEditWindow::DivideFrameWidth()
{
	int DivideNumber = m_DivMultiNumberInput->GetValueInt();
	if (DivideNumber <= 0)
		return;

	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	// Sequence�� ���õ��� �ʾҴٸ� return;
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);

	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	float FrameWidth = FinalEndPos.x - FinalStartPos.x;

	FrameWidth /= (float)DivideNumber;

	DragObject->SetStartPos(FinalStartPos);
	DragObject->SetEndPos(Vector2(FinalStartPos.x + FrameWidth, FinalEndPos.y));
}

void CSpriteEditWindow::DivideFrameHeight()
{
	int DivideNumber = m_DivMultiNumberInput->GetValueInt();
	if (DivideNumber <= 0)
		return;

	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	// Sequence�� ���õ��� �ʾҴٸ� return;
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);

	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	float FrameHeight = FinalEndPos.y - FinalStartPos.y;

	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	FrameHeight /= (float)DivideNumber;

	DragObject->SetStartPos(FinalStartPos);
	DragObject->SetEndPos(Vector2(FinalEndPos.x, FinalStartPos.y - FrameHeight));
}

void CSpriteEditWindow::MultiplyFrameWidth()
{
	int MultiNumber = m_DivMultiNumberInput->GetValueInt();
	if (MultiNumber <= 0)
		return;

	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	// Sequence�� ���õ��� �ʾҴٸ� return;
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);

	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	float FrameWidth = FinalEndPos.x - FinalStartPos.x;

	FrameWidth *= (float)MultiNumber;

	DragObject->SetStartPos(FinalStartPos);

	float MultipliedWidth = FinalStartPos.x + FrameWidth;
	MultipliedWidth = TextureSize.x < MultipliedWidth ? TextureSize.x - 0.1f : MultipliedWidth;

	DragObject->SetEndPos(Vector2(MultipliedWidth, FinalEndPos.y));
}

void CSpriteEditWindow::MultiplyFrameHeight()
{
	int MultiNumber = m_DivMultiNumberInput->GetValueInt();
	if (MultiNumber <= 0)
		return;

	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	// Sequence�� ���õ��� �ʾҴٸ� return;
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);

	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	float FrameHeight = FinalEndPos.y - FinalStartPos.y;
	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	FrameHeight *= (float)MultiNumber;

	float MultipliedHeight = FinalStartPos.y - FrameHeight;

	DragObject->SetStartPos(FinalStartPos);
	DragObject->SetEndPos(Vector2(FinalEndPos.x, MultipliedHeight));
}

void CSpriteEditWindow::MoveOneBlockRight()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);
	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	float FrameWidth = FinalEndPos.x - FinalStartPos.x;
	FinalStartPos.x += FrameWidth;
	FinalEndPos.x += FrameWidth;

	if (FinalEndPos.x >= TextureSize.x - 0.1f)
	{
		FinalEndPos.x = TextureSize.x - 0.1f;
		FinalStartPos.x = FinalEndPos.x - FrameWidth;
	}

	DragObject->SetStartPos(FinalStartPos);
	DragObject->SetEndPos(FinalEndPos);
}

void CSpriteEditWindow::MoveOneBlockLeft()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);
	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	float FrameWidth = FinalEndPos.x - FinalStartPos.x;
	FinalStartPos.x -= FrameWidth;
	FinalEndPos.x -= FrameWidth;

	if (FinalStartPos.x < 0.f)
	{
		FinalStartPos.x = 0.f;
		FinalEndPos.x = FinalStartPos.x + FrameWidth;
	}

	DragObject->SetStartPos(FinalStartPos);
	DragObject->SetEndPos(FinalEndPos);
}

void CSpriteEditWindow::MoveOneBlockUp()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);

	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;

	float FrameHeight = FinalEndPos.y - FinalStartPos.y;
	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	FinalStartPos.y += FrameHeight;
	FinalEndPos.y += FrameHeight;

	if (FinalStartPos.y >= TextureSize.y - 0.1f)
	{
		FinalStartPos.y = TextureSize.y;
		FinalEndPos.y = FinalStartPos.y - FrameHeight;
	}

	DragObject->SetStartPos(FinalStartPos);
	DragObject->SetEndPos(FinalEndPos);
}

void CSpriteEditWindow::MoveOneBlockDown()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// Texture�� �����´�.
	CAnimationSequence2D* Sequence = m_Animation->GetCurrentAnimation()->GetAnimationSequence();
	if (!Sequence)
		return;

	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 TextureSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

	Vector2 DragObjectStartPos = DragObject->GetStartPos();
	Vector2 DragObjectEndPos = DragObject->GetEndPos();

	std::pair<Vector2, Vector2> FinalStartEndPos = GetFinalFrameStartEndPos(DragObjectStartPos, DragObjectEndPos);

	Vector2 FinalStartPos = FinalStartEndPos.first;
	Vector2 FinalEndPos = FinalStartEndPos.second;


	float FrameHeight = FinalEndPos.y - FinalStartPos.y;
	FinalStartPos.y = TextureSize.y - FinalStartPos.y;
	FinalEndPos.y = TextureSize.y - FinalEndPos.y;

	FinalStartPos.y -= FrameHeight;
	FinalEndPos.y  -= FrameHeight;

	if (FinalEndPos.y < 0.f)
	{
		FinalEndPos.y = 0.f;
		FinalStartPos.y = FinalEndPos.y + FrameHeight;
	}

	DragObject->SetStartPos(FinalStartPos);
	DragObject->SetEndPos(FinalEndPos);
}

void CSpriteEditWindow::MoveOnePixelRight()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;
	DragObject->AddWorldPos(Vector3(1.f, 0.0f, 0.f));
	// Start, End Pos�� �ٲ��ش�.
	Vector2 StartPos = DragObject->GetStartPos();
	StartPos = Vector2(StartPos.x + 1.f, StartPos.y);
	Vector2 EndPos = DragObject->GetEndPos();
	EndPos = Vector2(EndPos.x + 1.f, EndPos.y);
	DragObject->SetStartPos(StartPos);
	DragObject->SetEndPos(EndPos);

}

void CSpriteEditWindow::MoveOnePixelLeft()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;

	DragObject->AddWorldPos(Vector3(-1.f, 0.f, 0.f));
	// Start, End Pos�� �ٲ��ش�.
	Vector2 StartPos = DragObject->GetStartPos();
	StartPos = Vector2(StartPos.x - 1.f, StartPos.y);
	Vector2 EndPos = DragObject->GetEndPos();
	EndPos = Vector2(EndPos.x - 1.f, EndPos.y);
	DragObject->SetStartPos(StartPos);
	DragObject->SetEndPos(EndPos);
	

}

void CSpriteEditWindow::MoveOnePixelUp()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;
	DragObject->AddWorldPos(Vector3(0.f, 1.f, 0.f));

	// Start, End Pos�� �ٲ��ش�.
	Vector2 StartPos = DragObject->GetStartPos();
	StartPos = Vector2(StartPos.x, StartPos.y + 1.f);
	Vector2 EndPos = DragObject->GetEndPos();
	EndPos = Vector2(EndPos.x, EndPos.y + 1.f);
	DragObject->SetStartPos(StartPos);
	DragObject->SetEndPos(EndPos);
	

}

void CSpriteEditWindow::MoveOnePixelDown()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;
	DragObject->AddWorldPos(Vector3(0.f, -1.f, 0.f));
	// Start, End Pos�� �ٲ��ش�.
	Vector2 StartPos = DragObject->GetStartPos();
	StartPos = Vector2(StartPos.x, StartPos.y - 1.f);
	Vector2 EndPos = DragObject->GetEndPos();
	EndPos = Vector2(EndPos.x, EndPos.y - 1.f);
	DragObject->SetStartPos(StartPos);
	DragObject->SetEndPos(EndPos);
	

}

void CSpriteEditWindow::EnlargeOnePixelRight()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;
	Vector2 EndPos = DragObject->GetEndPos();
	EndPos = Vector2(EndPos.x + 1.f, EndPos.y);
	DragObject->SetEndPos(EndPos);
}

void CSpriteEditWindow::ReduceOnePixelLeft()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;
	Vector2 EndPos = DragObject->GetEndPos();
	EndPos = Vector2(EndPos.x - 1.f, EndPos.y);
	DragObject->SetEndPos(EndPos);
}

void CSpriteEditWindow::ReduceOnePixelUp()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();
	if (!DragObject)
		return;
	Vector2 EndPos = DragObject->GetEndPos();
	EndPos = Vector2(EndPos.x, EndPos.y + 1.f);
	DragObject->SetEndPos(EndPos);
}

void CSpriteEditWindow::EnlargeOnePixelDown()
{
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();

	if (!DragObject)
		return;

	Vector2 EndPos = DragObject->GetEndPos();
	EndPos = Vector2(EndPos.x, EndPos.y - 1.f);
	DragObject->SetEndPos(EndPos);
}

void CSpriteEditWindow::SetSpriteCurrentFrameImageStart(const float x, const float y)
{
	m_SpriteCurrentFrame->SetImageStart(x, y);
}

void CSpriteEditWindow::SetSpriteCurrentFrameImageEnd(const float x, const float y)
{
	m_SpriteCurrentFrame->SetImageEnd(x, y);
}

std::pair<Vector2, Vector2> CSpriteEditWindow::GetFinalFrameStartEndPos(const Vector2& FrameStart, const Vector2& FrameEnd)
{
	float                 XDiff = -1, YDiff = -1;

	std::string           SequenceName = m_AnimationList->GetSelectItem();
	CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

	Vector2               FrameStartPos  = FrameStart;
	Vector2					FrameEndPos = FrameEnd;

	CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(SequenceName);
	CTexture* SequenceTexture = Sequence->GetTexture();
	Vector2 SequenceImageSize = Vector2((float)(SequenceTexture->GetWidth()), (float)(SequenceTexture->GetHeight()));

	XDiff = SequenceImageSize.x - FrameStartPos.x;
	YDiff = SequenceImageSize.y - FrameStartPos.y;

	// ���� ����
	// �ʰ� (FrameStartPos.x �� �ʹ� ���ʿ� ��ġ ex) -20.f )
	FrameStartPos.x = XDiff > SequenceImageSize.x ? 0.f : FrameStartPos.x;
	FrameStartPos.y = YDiff > SequenceImageSize.y ? 0.f : FrameStartPos.y;

	FrameStartPos.x = XDiff >= 0 ? FrameStartPos.x : SequenceImageSize.x - 0.1f;
	FrameStartPos.y = YDiff >= 0 ? SequenceImageSize.y - FrameStartPos.y : 0;

	// End 
	XDiff = SequenceImageSize.x - FrameEndPos.x;
	YDiff = SequenceImageSize.y - FrameEndPos.y;

	FrameEndPos.x = XDiff > SequenceImageSize.x ? 0.f : FrameEndPos.x;
	FrameEndPos.y = YDiff > SequenceImageSize.y ? 0.f : FrameEndPos.y;

	FrameEndPos.x = XDiff > 0 ? FrameEndPos.x : SequenceImageSize.x - 0.1f;
	FrameEndPos.y = YDiff > 0 ? SequenceImageSize.y - FrameEndPos.y : 0;

	Vector2 StartPos;
	Vector2 EndPos;

	// �� ���� �� ���� 
	StartPos.x = FrameStartPos.x < FrameEndPos.x ? FrameStartPos.x : FrameEndPos.x;
	StartPos.y = FrameStartPos.y < FrameEndPos.y ? FrameStartPos.y : FrameEndPos.y;

	// �� ū �� ���� 
	EndPos.x = FrameStartPos.x > FrameEndPos.x ? FrameStartPos.x : FrameEndPos.x;
	EndPos.y = FrameStartPos.y > FrameEndPos.y ? FrameStartPos.y : FrameEndPos.y;

	return std::make_pair(StartPos, EndPos);
}
