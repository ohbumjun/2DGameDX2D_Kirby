#include "SpriteWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "Engine.h"
#include "PathManager.h"
#include "../EditorManager.h"
#include "../Object/SpriteEditObject.h"
#include "../Object/DragObject.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"
#include "Device.h"
#include "Resource/Texture/Texture.h"
#include "Animation/AnimationSequence2DInstance.h"

CSpriteWindow::CSpriteWindow()  :
    m_SpriteObject(nullptr)
{
}

CSpriteWindow::~CSpriteWindow()
{
    SAFE_DELETE(m_Animation);
}

bool CSpriteWindow::Init()
{
    CIMGUIWindow::Init();

    m_Animation = new CAnimationSequence2DInstance;
    m_Animation->Stop();

    // ==============================

    CIMGUIButton* Button = AddWidget<CIMGUIButton>("LoadTexture");
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::LoadTextureButton);

    CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("SpriteEdit");
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::SpriteEditButton);

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
    m_AnimationList->SetSelectCallback<CSpriteWindow>(this, &CSpriteWindow::SelectAnimationSequence);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("AddAnim", 80.f, 30.f);
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::AddAnimationButton);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_AnimationFrameList = AddWidget<CIMGUIListBox>("AnimationFrameList", 200.f, 300.f);
    m_AnimationFrameList->SetHideName(true);
    m_AnimationFrameList->SetPageItemCount(6);
    m_AnimationFrameList->SetSelectCallback<CSpriteWindow>(this, &CSpriteWindow::SelectAnimationFrame);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("AddFrame", 80.f, 30.f);
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::AddAnimationFrameButton);

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
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::PlayAnimationButton);
    
    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(400.f);
    
    Button = AddWidget<CIMGUIButton>("Stop", 80.f, 30.f);
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::StopAnimationButton);

    // ==============================
    Button = AddWidget<CIMGUIButton>("DelFrame", 80.f, 30.f);
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::DeleteFrameButton);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(90.f);

    Button = AddWidget<CIMGUIButton>("ClearFrame", 80.f, 30.f);
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::ClearFrameButton);

    // =============================

    m_StartFramePosXInput = AddWidget<CIMGUITextInput>("StartX");
    m_StartFramePosXInput->SetSize(80.f, 30.f);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(160.f);

    m_StartFramePosYInput = AddWidget<CIMGUITextInput>("StartY");
    m_StartFramePosYInput->SetSize(80.f, 30.f);

    m_EndFramePosXInput = AddWidget<CIMGUITextInput>("EndX");
    m_EndFramePosXInput->SetSize(80.f,30.f);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(160.f);

    m_EndFramePosYInput = AddWidget<CIMGUITextInput>("EndY");
    m_EndFramePosYInput->SetSize(80.f,30.f);

    // =================================================

    Button = AddWidget<CIMGUIButton>("EditFrame", 80.f, 30.f);
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::EditFrameButton);

    return true;
}

void CSpriteWindow::Update(float DeltaTime)
{
    CIMGUIWindow::Update(DeltaTime);
    /*
    if (m_Animation)
    {
        m_Animation->Update(DeltaTime);

        if (!m_Animation->GetCurrentAnimation())
            return;

        if (m_Animation->GetCurrentAnimation()->GetFrameCount() <= 0)
            return;

        AnimationFrameData FrameData = m_Animation->GetCurrentAnimation()->GetCurrentFrameData();
        m_SpriteSampled->SetImageStart(FrameData.Start);
        m_SpriteSampled->SetImageEnd(FrameData.Size);
    }
    */

}

void CSpriteWindow::LoadTextureButton()
{
    if (CEditorManager::GetInst()->GetEditMode() != EditMode::Sprite)
        return;

    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OpenFile = {};

    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle(); // handle to window that owns the dialog box
    OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
    OpenFile.lpstrFile = FilePath; // buffer ! filename used to initialize the file name edit control -> 정상적으로 save,open할시 여기에 filePath 정보가 들어온다.
    OpenFile.nMaxFile = MAX_PATH; // size of buffer, pointed to by lpstrFile
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path; // Initial Directory

    if (GetOpenFileName(&OpenFile) != 0) // NonZero --> specifies file name, clicks ok button
    {
        int TextureIndex = 0;
        TCHAR FileName[MAX_PATH] = {};
        _wsplitpath_s(FilePath, 0, 0, 0, 0, FileName, MAX_PATH, 0, 0);

        char ConvertFileName[MAX_PATH] = {};
        int Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FileName, -1, ConvertFileName, Length, 0, 0);

        m_Sprite->SetTextureFullPath(ConvertFileName, FilePath);
        m_SpriteObject->GetSpriteComponent()->SetTextureFullPath(TextureIndex, 0, (int)ConstantBuffer_Shader_Type::Pixel,
        ConvertFileName, FilePath);

        CTexture* Texture = m_SpriteObject->GetSpriteComponent()->GetTexture(TextureIndex);
        m_SpriteObject->SetWorldScale((float)Texture->GetWidth(), (float)Texture->GetHeight(), 1.f);
    }
    
}

void CSpriteWindow::SpriteEditButton()
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

void CSpriteWindow::AddAnimationButton()
{
    // Sprite Object가 있을 때에만 세팅한다.
    if (!m_SpriteObject)
        return;

    // Input으로부터 입력받은 Text 정보를 받아온다. 
    // Text가 비었다면, 입력 받지 않는다.
    if (m_AnimInputName->Empty())
        return;

    // Text 중복방지 
    const std::string SequenceName = m_AnimInputName->GetTextMultibyte();
    const std::string& AnimationName = SequenceName;

    if (m_AnimationList->CheckItem(SequenceName))
         return;

    // Loop, Reverse 선택 
    if (m_AnimationLoop->GetSelectIndex() < 0 || m_AnimationReverse->GetSelectIndex() < 0)
        return;

    // Animation Sequence 2D 만들기 --> Sprite Edit Object상에 불러놓은 Texture로 
    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();
    CTexture* LoadedTexture = m_SpriteObject->GetSpriteComponent()->GetTexture();

    if (!Resource->CreateAnimationSequence2D(SequenceName, LoadedTexture))
        return;

    // Text 추가하기 
    m_AnimationList->AddItem(SequenceName);

    // Animation 내용 추가 
    bool Loop      = StringToBool(m_AnimationLoop->GetSelectItem());
    bool Reverse = StringToBool(m_AnimationReverse->GetSelectItem());
    m_Animation->AddAnimation(SequenceName, AnimationName, Loop, 1.f, 1.f, Reverse);
    // m_Animation->SetCurrentAnimation(AnimationName);

    // Animation 진행
    m_Animation->Play();

}

void CSpriteWindow::AddAnimationFrameButton()
{
    // Animation List의 내용이 선택되어 있어야 한다.
    bool IsSelected = m_AnimationList->IsSelected();
    if (IsSelected == false)
        return;

    float XDiff = -1, YDiff = -1;
    CSceneResource* Resource              = CSceneManager::GetInst()->GetScene()->GetResource();
    Vector2 FrameStartPos                    = CEditorManager::GetInst()->GetDragObject()->GetStartPos();
    std::string SequenceName               = m_AnimationList->GetSelectItem();
    const std::string& AnimationName    = SequenceName;
    CAnimationSequence2D* Sequence  = Resource->FindAnimationSequence2D(SequenceName);
    CTexture* SequenceTexture             = Sequence->GetTexture();

    // 220 ? --> 300 - 220  = 80
    // AnimationList 에서 선택한 Sequence2D의 Name으로 Texture 정보를 얻어온다
    Vector2 SequenceImageSize = Vector2((float)SequenceTexture->GetWidth(), (float)SequenceTexture->GetHeight());

    XDiff = SequenceImageSize.x - FrameStartPos.x;
    YDiff = SequenceImageSize.y - FrameStartPos.y;

    // 범위 조정 
    FrameStartPos.x = XDiff > 0 ? FrameStartPos.x : m_SpriteObject->GetWorldScale().x;
    FrameStartPos.y = YDiff > 0 ? m_SpriteObject->GetWorldScale().y - FrameStartPos.y : 0;

    Vector2 FrameEndPos = CEditorManager::GetInst()->GetDragObject()->GetEndPos();
    XDiff = SequenceImageSize.x - FrameEndPos.x;
    YDiff = SequenceImageSize.y - FrameEndPos.y;
    FrameEndPos.x = XDiff > 0 ? FrameEndPos.x : m_SpriteObject->GetWorldScale().x;
    FrameEndPos.y = YDiff > 0 ? m_SpriteObject->GetWorldScale().y - FrameEndPos.y : 0;

    Vector2 StartPos;
    Vector2 EndPos;

    // 더 작은 것 선택 
    StartPos.x = FrameStartPos.x < FrameEndPos.x ? FrameStartPos.x : FrameEndPos.x;
    StartPos.y = FrameStartPos.y < FrameEndPos.y ? FrameStartPos.y : FrameEndPos.y;

    // 더 큰 것 선택 
    EndPos.x = FrameStartPos.x > FrameEndPos.x ? FrameStartPos.x : FrameEndPos.x;
    EndPos.y = FrameStartPos.y > FrameEndPos.y ? FrameStartPos.y : FrameEndPos.y;

    // 해당 FramePos 정보로 Animation Frame 만들어서 넣어주기 
    // Animation Sequence 2D 만들기 --> Sprite Edit Object상에 불러놓은 Texture로 
    Sequence->AddFrame(StartPos, EndPos - StartPos);

    // Frame List Box에 넣어주기 
    char FrameName[1024] = {};
    int   AnimItemCount = m_AnimationFrameList->GetItemCount();
    sprintf_s(FrameName, "%d", AnimItemCount);
    m_AnimationFrameList->AddItem(FrameName);

    // Sampled에 Image 세팅해주기 
    CSpriteComponent* SpriteObjectComponent = dynamic_cast<CSpriteComponent*>(m_SpriteObject->GetRootComponent());
    m_SpriteSampled->SetTexture(SpriteObjectComponent->GetTextureName());
    
    // Image, End 세팅 
    Vector2 SpriteSize = m_SpriteSampled->GetSize();
    m_SpriteSampled->SetImageStart(StartPos);
    m_SpriteSampled->SetImageEnd(EndPos);

    // AnimationFrameListBox 내의 SelectIdx 정보도 바꿔주기 
    m_AnimationFrameList->SetSelectIndex(AnimItemCount);

    // Animation Instance에 추가하기 
    bool Loop = StringToBool(m_AnimationLoop->GetSelectItem());
    bool Reverse = StringToBool(m_AnimationReverse->GetSelectItem());

    // 위에서 가져온 Animation Sequence 2D를 세팅하면 될 것 같다
    CAnimationSequence2DData* Animation =  m_Animation->GetCurrentAnimation();
    if (!Animation)
        return;

    // Animation
    Animation->AddFrame(StartPos, EndPos - StartPos);

    // Stop Animation
    m_Animation->Stop();

}

void CSpriteWindow::DeleteFrameButton()
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

    // Delete Frame Data
    CSceneResource* Resource             = CSceneManager::GetInst()->GetScene()->GetResource();
    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetSelectItem());
    int SelectedFrameIdx = std::stoi(m_AnimationFrameList->GetSelectItem());
    Sequence->DeleteFrame(SelectedFrameIdx);

    // Delete Text
    m_AnimationFrameList->DeleteItem(SelectedFrameIdx);

    // Update Names Of FrameDatas
    int ListNums = m_AnimationFrameList->GetItemCount();
    for (int i = SelectedFrameIdx; i < ListNums; i++)
    {
        m_AnimationFrameList->SetItem(i, std::to_string(i));
    }
}

void CSpriteWindow::ClearFrameButton()
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
    CSceneResource* Resource            = CSceneManager::GetInst()->GetScene()->GetResource();
    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetSelectItem());
    Sequence->ClearFrame();
    
    // Clear Texts
    m_AnimationFrameList->Clear();

    // Set Idx 
    m_AnimationFrameList->SetSelectIndex(-1);

    // Set Default Image 
    m_SpriteSampled->SetTexture("DefaultUI");
}

void CSpriteWindow::EditFrameButton()
{
    // Empty
    if (m_StartFramePosXInput->Empty() || m_StartFramePosYInput->Empty())
        return;
    if (m_EndFramePosXInput->Empty() || m_EndFramePosYInput->Empty())
        return;

    // Not Selected 
    if (!m_AnimationList->IsSelected() || !m_AnimationFrameList->IsSelected())
        return;

    const std::string StartFrameX = m_StartFramePosXInput->GetTextUTF8();
    const std::string StartFrameY = m_StartFramePosYInput->GetTextUTF8();
    const std::string EndFrameX    = m_EndFramePosXInput->GetTextUTF8();
    const std::string EndFrameY    = m_EndFramePosYInput->GetTextUTF8();

    if (!CheckIfStringIsDigit(StartFrameX) || !CheckIfStringIsDigit(StartFrameY))
        return;    
    if (!CheckIfStringIsDigit(EndFrameX) || !CheckIfStringIsDigit(EndFrameY))
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();
    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetSelectItem());
    Vector2 SequenceSize = Vector2((float)Sequence->GetTexture()->GetWidth(), (float)Sequence->GetTexture()->GetHeight());

    float  FloatStartFrameX = std::stof(StartFrameY);
    float  FloatStartFrameY = std::stof(StartFrameX);

    float  FloatEndFrameX = std::stof(EndFrameX);
    float  FloatEndFrameY = std::stof(EndFrameY);

    float DStartFrameX = FloatStartFrameX < FloatEndFrameX ? FloatStartFrameX : FloatEndFrameX;
    float DStartFrameY = FloatStartFrameY < FloatEndFrameY ? FloatStartFrameY : FloatEndFrameY;

    float DEndFrameX = FloatStartFrameX > FloatEndFrameX ? FloatStartFrameX : FloatEndFrameX;
    float DEndFrameY = FloatStartFrameY > FloatEndFrameY ? FloatStartFrameY : FloatEndFrameY;


    // 범위 초과 검사 
    DStartFrameX = DStartFrameX < SequenceSize.x ? DStartFrameX : SequenceSize.x;
    DStartFrameY = DStartFrameY < SequenceSize.y ? DStartFrameY : SequenceSize.y;
    DEndFrameX   = DEndFrameX  < SequenceSize.x ? DEndFrameX : SequenceSize.x;
    DEndFrameY   = DEndFrameY  < SequenceSize.y ? DEndFrameY : SequenceSize.y;

    Sequence->SetFrame(std::stoi(m_AnimationFrameList->GetSelectItem()), 
        DStartFrameX, DStartFrameY, DEndFrameX - DStartFrameX, DEndFrameY - DStartFrameY);

    m_SpriteSampled->SetImageStart(DStartFrameX, DStartFrameY);
    m_SpriteSampled->SetImageEnd(DEndFrameX, DEndFrameY);
}

void CSpriteWindow::PlayAnimationButton()
{
    m_Animation->Play();
}

void CSpriteWindow::StopAnimationButton()
{
    m_Animation->Stop();
}

void CSpriteWindow::SelectAnimationSequence(int Index, const char* TextureName)
{
    // 해당 idx의 Sequence 정보 가져오기
    CSceneResource* Resource                                 = CSceneManager::GetInst()->GetScene()->GetResource();
    std::string ChangedSequenceName                     = m_AnimationList->GetItem(Index);
    CAnimationSequence2D* ChangedSequence         = Resource->FindAnimationSequence2D(ChangedSequenceName);
    CTexture* ChangedSequenceTexture                    = ChangedSequence->GetTexture();
    Vector2 FrameStartPos                                       = CEditorManager::GetInst()->GetDragObject()->GetStartPos();
    int MaterialTextureIdx = 0;

    // SpriteObject 의 Texture 바꿔주기
    m_SpriteObject->GetSpriteComponent()->SetTexture(MaterialTextureIdx,ChangedSequenceTexture);
    m_SpriteObject->SetWorldScale((float)ChangedSequenceTexture->GetWidth(), (float)ChangedSequenceTexture->GetHeight(), 1.f);

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

    AnimationFrameData       FrameData                   = ChangedSequence->GetFrameData(0);
   m_AnimationFrameList->SetSelectIndex(0);

    // Texture 세팅 
    m_SpriteSampled->SetTexture(ChangedSequenceTexture);
    m_SpriteSampled->SetImageStart(FrameData.Start);
    m_SpriteSampled->SetImageEnd(FrameData.Start+ FrameData.Size.x);

    // 현재 애니메이션으로 세팅 
    m_Animation->SetCurrentAnimation(ChangedSequenceName);
}

void CSpriteWindow::SelectAnimationFrame(int Index, const char* Name)
{
    CSceneResource* Resource               = CSceneManager::GetInst()->GetScene()->GetResource();
    std::string SequenceName                = m_AnimationList->GetSelectItem();
    CAnimationSequence2D* Sequence   = Resource->FindAnimationSequence2D(SequenceName);
    CTexture* SequenceTexture              = Sequence->GetTexture();
    AnimationFrameData  FrameData      = Sequence->GetFrameData(Index);
    
    // SpriteSampled만 바꿔주기 
    m_SpriteSampled->SetImageStart(FrameData.Start);
    m_SpriteSampled->SetImageEnd(FrameData.Start + FrameData.Size);
}

