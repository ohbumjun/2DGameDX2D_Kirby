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
	m_SpriteObject(nullptr)
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

	CIMGUIButton* Button = AddWidget<CIMGUIButton>("LoadTexture");
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::LoadTextureButton);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("SpriteEdit");
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SpriteEditButton);

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

	m_Sprite = AddWidget<CIMGUIImage>("SpriteOrigin", 200.f, 200.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(300.f);

	m_SpriteSampled = AddWidget<CIMGUIImage>("SpriteSampled", 200.f, 200.f);

	// ==============================

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("AnimationListName", 200.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_AnimInputName = AddWidget<CIMGUITextInput>("AnimNameInput");
	m_AnimInputName->SetHideName(true);
	m_AnimInputName->SetSize(80.f, 30.f);

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

	Button = AddWidget<CIMGUIButton>("AddAnim", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::AddAnimationButton);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_AnimationFrameList = AddWidget<CIMGUIListBox>("AnimationFrameList", 200.f, 300.f);
	m_AnimationFrameList->SetHideName(true);
	m_AnimationFrameList->SetPageItemCount(6);
	m_AnimationFrameList->SetSelectCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::SelectAnimationFrame);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("AddFrame", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::AddAnimationFrameButton);

	// ==============================
	m_AnimationLoop = AddWidget<CIMGUIComboBox>("Loop", 80.f, 30.f);
	m_AnimationLoop->SetHideName(true);
	m_AnimationLoop->AddItem("True");
	m_AnimationLoop->AddItem("False");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

	m_AnimationReverse = AddWidget<CIMGUIComboBox>("Reverse", 80.f, 30.f);
	m_AnimationReverse->SetHideName(true);
	m_AnimationReverse->AddItem("True");
	m_AnimationReverse->AddItem("False");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(310.f);

	Button = AddWidget<CIMGUIButton>("Play", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::PlayAnimationButton);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(400.f);

	Button = AddWidget<CIMGUIButton>("Stop", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::StopAnimationButton);

	// ==============================
	Button = AddWidget<CIMGUIButton>("DelFrame", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::DeleteFrameButton);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(90.f);

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

	// =================================================

	Button = AddWidget<CIMGUIButton>("EditFrame", 80.f, 30.f);
	Button->SetClickCallback<CSpriteEditWindow>(this, &CSpriteEditWindow::EditFrameButton);

	// =================================================
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

		// Update 된 이후, 현재 Frame의 Animation Frame Data를 보여준다
		int                CurrentFrame = m_Animation->GetCurrentAnimation()->GetCurrentFrame();
		AnimationFrameData FrameData    = m_Animation->GetCurrentAnimation()->GetFrameData(CurrentFrame);
		m_SpriteSampled->SetImageStart(FrameData.Start);
		m_SpriteSampled->SetImageEnd(FrameData.Start + FrameData.Size);
	}
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
		TEXT("모든파일\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
	OpenFile.lpstrFile = FilePath;
	// buffer ! filename used to initialize the file name edit control -> 정상적으로 save,open할시 여기에 filePath 정보가 들어온다.
	OpenFile.nMaxFile        = MAX_PATH; // size of buffer, pointed to by lpstrFile
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path; // Initial Directory

	if (GetOpenFileName(&OpenFile) != 0) // NonZero --> specifies file name, clicks ok button
	{
		int   TextureIndex       = 0;
		TCHAR FileName[MAX_PATH] = {};
		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, MAX_PATH, nullptr, 0);

		char ConvertFileName[MAX_PATH] = {};
		int  Length                    = WideCharToMultiByte(CP_ACP, 0, FileName, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, FileName, -1, ConvertFileName, Length, nullptr, nullptr);

		m_Sprite->SetTextureFullPath(ConvertFileName, FilePath);
		m_SpriteObject->GetSpriteComponent()->SetTextureFullPath(TextureIndex, 0,
		                                                         static_cast<int>(ConstantBuffer_Shader_Type::Pixel),
		                                                         ConvertFileName, FilePath);

		CTexture* Texture = m_SpriteObject->GetSpriteComponent()->GetTexture(TextureIndex);
		m_SpriteObject->SetWorldScale(static_cast<float>(Texture->GetWidth()), static_cast<float>(Texture->GetHeight()),
		                              1.f);
	}
}

void CSpriteEditWindow::SpriteEditButton()
{
	CEditorManager::GetInst()->SetEditMode(EditMode::Sprite);

	if (!m_SpriteObject)
	{
		m_SpriteObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
		m_SpriteObject->SetEditWindow(this);

		// m_Sprite UIImage 에도 세팅
		CTexture* Texture = m_SpriteObject->GetSpriteComponent()->GetTexture();
		m_Sprite->SetTexture(Texture);
	}
}

void CSpriteEditWindow::AddAnimationButton()
{
	// Sprite Object가 있을 때에만 세팅한다.
	if (!m_SpriteObject)
		return;

	// Input으로부터 입력받은 Text 정보를 받아온다. 
	// Text가 비었다면, 입력 받지 않는다.
	if (m_AnimInputName->Empty())
		return;

	// Text 중복방지 
	const std::string  SequenceName  = m_AnimInputName->GetTextMultibyte();
	const std::string& AnimationName = SequenceName;

	if (m_AnimationList->CheckItem(SequenceName))
		return;

	// Loop, Reverse 선택 
	if (m_AnimationLoop->GetSelectIndex() < 0 || m_AnimationReverse->GetSelectIndex() < 0)
		return;

	// Animation Sequence 2D 만들기 --> Sprite Edit Object상에 불러놓은 Texture로 
	CSceneResource* Resource      = CSceneManager::GetInst()->GetScene()->GetResource();
	CTexture*       LoadedTexture = m_SpriteObject->GetSpriteComponent()->GetTexture();

	if (!Resource->CreateAnimationSequence2D(SequenceName, LoadedTexture))
		return;

	// Text 추가하기 
	m_AnimationList->AddItem(SequenceName);

	// Animation 내용 추가 
	bool Loop    = StringToBool(m_AnimationLoop->GetSelectItem());
	bool Reverse = StringToBool(m_AnimationReverse->GetSelectItem());
	m_Animation->AddAnimation(SequenceName, AnimationName, Loop, 1.f, 1.f, Reverse);

	// Animation 진행
	m_Animation->Play();
}

void CSpriteEditWindow::AddAnimationFrameButton()
{
	// Animation List의 내용이 선택되어 있어야 한다.
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	float                 XDiff           = -1, YDiff = -1;
	CSceneResource*       Resource        = CSceneManager::GetInst()->GetScene()->GetResource();
	Vector2               FrameStartPos   = CEditorManager::GetInst()->GetDragObject()->GetStartPos();
	std::string           SequenceName    = m_AnimationList->GetSelectItem();
	const std::string&    AnimationName   = SequenceName;
	CAnimationSequence2D* Sequence        = Resource->FindAnimationSequence2D(SequenceName);
	CTexture*             SequenceTexture = Sequence->GetTexture();

	// 220 ? --> 300 - 220  = 80
	// AnimationList 에서 선택한 Sequence2D의 Name으로 Texture 정보를 얻어온다
	Vector2 SequenceImageSize = Vector2((float)(SequenceTexture->GetWidth()),(float)(SequenceTexture->GetHeight()));

	XDiff = SequenceImageSize.x - FrameStartPos.x;
	YDiff = SequenceImageSize.y - FrameStartPos.y;

	// 범위 조정
	// 초과 (FrameStartPos.x 가 너무 왼쪽에 위치 ex) -20.f )
	FrameStartPos.x = XDiff > SequenceImageSize.x ? 0.f : FrameStartPos.x;
	FrameStartPos.y = YDiff > SequenceImageSize.y ? 0.f : FrameStartPos.y;

	FrameStartPos.x = XDiff >= 0 ? FrameStartPos.x : SequenceImageSize.x - 0.1f;
	FrameStartPos.y = YDiff >= 0 ? SequenceImageSize.y - FrameStartPos.y : 0;

	Vector2 FrameEndPos = CEditorManager::GetInst()->GetDragObject()->GetEndPos();
	XDiff               = SequenceImageSize.x - FrameEndPos.x;
	YDiff               = SequenceImageSize.y - FrameEndPos.y;

	FrameEndPos.x = XDiff > SequenceImageSize.x ? 0.f : FrameEndPos.x;
	FrameEndPos.y = YDiff > SequenceImageSize.y ? 0.f : FrameEndPos.y;

	FrameEndPos.x       = XDiff > 0 ? FrameEndPos.x : SequenceImageSize.x - 0.1f;
	FrameEndPos.y       = YDiff > 0 ? SequenceImageSize.y - FrameEndPos.y : 0;

	Vector2 StartPos;
	Vector2 EndPos;

	// 더 작은 것 선택 
	StartPos.x = FrameStartPos.x < FrameEndPos.x ? FrameStartPos.x : FrameEndPos.x;
	StartPos.y = FrameStartPos.y < FrameEndPos.y ? FrameStartPos.y : FrameEndPos.y;

	// 더 큰 것 선택 
	EndPos.x = FrameStartPos.x > FrameEndPos.x ? FrameStartPos.x : FrameEndPos.x;
	EndPos.y = FrameStartPos.y > FrameEndPos.y ? FrameStartPos.y : FrameEndPos.y;

	Vector2 FrameSize = EndPos - StartPos;
	// 해당 FramePos 정보로 Animation Frame 만들어서 넣어주기 
	// Animation Sequence 2D 만들기 --> Sprite Edit Object상에 불러놓은 Texture로 
	Sequence->AddFrame(StartPos, EndPos - StartPos);

	// Frame List Box에 넣어주기 
	char FrameName[1024] = {};
	int  AnimItemCount   = m_AnimationFrameList->GetItemCount();
	sprintf_s(FrameName, "%d", AnimItemCount);
	m_AnimationFrameList->AddItem(FrameName);

	// Sampled에 Image 세팅해주기 
	CSpriteComponent* SpriteObjectComponent = dynamic_cast<CSpriteComponent*>(m_SpriteObject->GetRootComponent());
	m_SpriteSampled->SetTexture(SpriteObjectComponent->GetTextureName());

	// Image, End 세팅 
	m_SpriteSampled->SetImageStart(StartPos);
	m_SpriteSampled->SetImageEnd(EndPos);

	// AnimationFrameListBox 내의 SelectIdx 정보도 바꿔주기 
	m_AnimationFrameList->SetSelectIndex(AnimItemCount);

	// Animation Instance에 추가하기 
	bool Loop    = StringToBool(m_AnimationLoop->GetSelectItem());
	bool Reverse = StringToBool(m_AnimationReverse->GetSelectItem());

	// 위에서 가져온 Animation Sequence 2D를 세팅하면 될 것 같다
	CAnimationSequence2DData* Animation = m_Animation->GetCurrentAnimation();
	if (!Animation)
		return;

	// Animation --> 중복해서 Animation이 들어가게 된다.
	// Animation->AddFrame(StartPos, EndPos - StartPos);

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
			// 0 이라면, 만약 2개 이상의 Frame이 Add된 상태라면, 지운 다음 녀석, 즉, 다시 0을 가리키게 한다 ( 1에서 0이 된 녀석 )
			// 아무것도 안하면 0을 가리키게 될 것이다. 
			m_AnimationFrameList->SetSelectIndex(m_AnimationFrameList->GetSelectIndex());
		}
		else
		{
			// 그게 아니라면, 다 지워진 것 --> Clear 해준다.
			ClearFrameButton();
			return;
		}
	}
	else
	{
		// 1 이상의 수라면, -1을 해준다. 
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


	// 범위 초과 검사 
	DStartFrameX = DStartFrameX < SequenceSize.x ? DStartFrameX : SequenceSize.x;
	DStartFrameY = DStartFrameY < SequenceSize.y ? DStartFrameY : SequenceSize.y;
	DEndFrameX   = DEndFrameX < SequenceSize.x ? DEndFrameX : SequenceSize.x;
	DEndFrameY   = DEndFrameY < SequenceSize.y ? DEndFrameY : SequenceSize.y;

	Sequence->SetFrame(std::stoi(m_AnimationFrameList->GetSelectItem()),
	                   DStartFrameX, DStartFrameY, DEndFrameX - DStartFrameX, DEndFrameY - DStartFrameY);

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

void CSpriteEditWindow::SaveSequence()
{
	if (m_AnimationList->GetSelectIndex() < 0)
		return;

	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile    = {};
	OpenFile.lStructSize     = sizeof(OPENFILENAME);
	OpenFile.hwndOwner       = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter     = TEXT("모든파일\0*.*\0.Sequence File\0*.sqc");
	OpenFile.lpstrFile       = FilePath;
	OpenFile.nMaxFile        = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	if (GetSaveFileName(&OpenFile) != 0)
	{
		// 저장에 성공했다면
		char FilePathMultibyte[MAX_PATH] = {};
		int  ConvertLength               = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FilePathMultibyte, ConvertLength, nullptr, nullptr);

		m_Animation->GetCurrentAnimation()->GetAnimationSequence()->SaveFullPath(FilePathMultibyte);
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
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0.Sequence File\0*.sqc");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FullPathMultibyte[MAX_PATH] = {};

		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, 0, 0);

		std::string SequenceName;
		CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();
		Resource->LoadSequence2D(SequenceName, FullPathMultibyte);
	}
}

void CSpriteEditWindow::SaveAnimation()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
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

		m_Animation->Save(FilePathMultibyte);
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

		m_Animation->Load(FilePathMultibyte);
	}
}

void CSpriteEditWindow::SelectAnimationSequence(int Index, const char* TextureName)
{
	// 해당 idx의 Sequence 정보 가져오기
	CSceneResource*       Resource               = CSceneManager::GetInst()->GetScene()->GetResource();
	std::string           ChangedSequenceName    = m_AnimationList->GetItem(Index);
	CAnimationSequence2D* ChangedSequence        = Resource->FindAnimationSequence2D(ChangedSequenceName);
	CTexture*             ChangedSequenceTexture = ChangedSequence->GetTexture();
	Vector2               FrameStartPos          = CEditorManager::GetInst()->GetDragObject()->GetStartPos();
	int                   MaterialTextureIdx     = 0;

	// SpriteObject 의 Texture 바꿔주기
	m_SpriteObject->GetSpriteComponent()->SetTexture(MaterialTextureIdx, ChangedSequenceTexture);
	m_SpriteObject->SetWorldScale((float)(ChangedSequenceTexture->GetWidth()),(float)(ChangedSequenceTexture->GetHeight()), 1.f);

	// m_Sprite의 Texture 바꿔주기 
	m_Sprite->SetTexture(ChangedSequenceTexture);

	// AnimationFrameList 안에 있는 내용 모두 다시 세팅하기 
	m_AnimationFrameList->Clear();

	// 해당 Sequence에 대해서 추가해놓은 Frame정보가 없다면 
	if (ChangedSequence->GetFrameCount() <= 0)
		return;

	// 새로운 Frame 정보들 추가하기 
	char IndexChar[1024];
	for (int i = 0; i < ChangedSequence->GetFrameCount(); i++)
	{
		sprintf_s(IndexChar, "%d", i);
		m_AnimationFrameList->AddItem(IndexChar);
	}

	// 첫번째 Seq로 세팅하기 
	AnimationFrameData FrameData = ChangedSequence->GetFrameData(0);
	m_AnimationFrameList->SetSelectIndex(0);

	// Texture 세팅 
	m_SpriteSampled->SetTexture(ChangedSequenceTexture);
	m_SpriteSampled->SetImageStart(FrameData.Start);
	m_SpriteSampled->SetImageEnd(FrameData.Start + FrameData.Size);

	// 현재 애니메이션으로 세팅 
	m_Animation->SetCurrentAnimation(ChangedSequenceName);
}

void CSpriteEditWindow::SelectAnimationFrame(int Index, const char* Name)
{
	CSceneResource*       Resource        = CSceneManager::GetInst()->GetScene()->GetResource();
	std::string           SequenceName    = m_AnimationList->GetSelectItem();
	CAnimationSequence2D* Sequence        = Resource->FindAnimationSequence2D(SequenceName);
	CTexture*             SequenceTexture = Sequence->GetTexture();
	AnimationFrameData    FrameData       = Sequence->GetFrameData(Index);

	// SpriteSampled만 바꿔주기 
	m_SpriteSampled->SetImageStart(FrameData.Start);
	m_SpriteSampled->SetImageEnd(FrameData.Start + FrameData.Size);

	// Drag Object Setting
	CDragObject* DragObject = CEditorManager::GetInst()->GetDragObject();

	Vector2 ImageSize = Vector2(static_cast<float>(SequenceTexture->GetWidth()),
	                            static_cast<float>(SequenceTexture->GetHeight()));
	Vector2 StartPos = FrameData.Start;
	Vector2 EndPos   = FrameData.Start + FrameData.Size;

	StartPos.y = ImageSize.y - StartPos.y;
	EndPos.y   = ImageSize.y - EndPos.y;

	Vector2 SpriteObject2DPos = Vector2(m_SpriteObject->GetWorldPos().x, m_SpriteObject->GetWorldPos().y);
	StartPos += SpriteObject2DPos;
	EndPos += SpriteObject2DPos;

	DragObject->SetStartPos(StartPos);
	DragObject->SetEndPos(EndPos);

	// 클릭시 Animation 멈춰주기
	m_Animation->Stop();
}
