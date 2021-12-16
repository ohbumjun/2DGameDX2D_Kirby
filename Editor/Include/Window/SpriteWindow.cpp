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

CSpriteWindow::CSpriteWindow()  :
    m_SpriteObject(nullptr)
{
}

CSpriteWindow::~CSpriteWindow()
{
}

bool CSpriteWindow::Init()
{
    CIMGUIWindow::Init();

    CIMGUIButton* Button = AddWidget<CIMGUIButton>("LoadTexture");
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::LoadTextureButton);

    CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("SpriteEdit");
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::SpriteEditButton);

    m_Sprite = AddWidget<CIMGUIImage>("SpriteOrigin", 200.f, 200.f);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(300.f);

    m_SpriteSampled = AddWidget<CIMGUIImage>("SpriteSampled", 200.f, 200.f);

    // ================

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

    
    // ================
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

    return true;
}

void CSpriteWindow::Update(float DeltaTime)
{
    CIMGUIWindow::Update(DeltaTime);
}

void CSpriteWindow::LoadTextureButton()
{
    if (CEditorManager::GetInst()->GetEditMode() != EditMode::Sprite)
        return;

    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OpenFile = {};

    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle(); // handle to window that owns the dialog box
    OpenFile.lpstrFilter = TEXT("�������\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
    OpenFile.lpstrFile = FilePath; // buffer ! filename used to initialize the file name edit control -> ���������� save,open�ҽ� ���⿡ filePath ������ ���´�.
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
        m_SpriteObject->SetWorldScale(Texture->GetWidth(), Texture->GetHeight(), 1.f);
    }
    
}

void CSpriteWindow::SpriteEditButton()
{
    CEditorManager::GetInst()->SetEditMode(EditMode::Sprite);

    if (!m_SpriteObject)
    {
        m_SpriteObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
        m_SpriteObject->SetEditWindow(this);

        // m_Sprite UIImage ���� ����
        CTexture* Texture = m_SpriteObject->GetSpriteComponent()->GetTexture();
        m_Sprite->SetTexture(Texture);
    }
}

void CSpriteWindow::AddAnimationButton()
{
    // Sprite Object�� ���� ������ �����Ѵ�.
    if (!m_SpriteObject)
        return;

    // Input���κ��� �Է¹��� Text ������ �޾ƿ´�. 
    // Text�� ����ٸ�, �Է� ���� �ʴ´�.
    if (m_AnimInputName->Empty())
        return;

    // Text �ߺ����� 
    const std::string Name = m_AnimInputName->GetTextMultibyte();
    if (m_AnimationList->CheckItem(Name))
         return;

    // Animation Sequence 2D ����� --> Sprite Edit Object�� �ҷ����� Texture�� 
    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();
    CTexture* LoadedTexture = m_SpriteObject->GetSpriteComponent()->GetTexture();

    if (!Resource->CreateAnimationSequence2D(Name, LoadedTexture))
        return;

    // Text �߰��ϱ� 
    m_AnimationList->AddItem(Name);

}

void CSpriteWindow::AddAnimationFrameButton()
{
    // Animation List�� ������ ���õǾ� �־�� �Ѵ�.
    bool IsSelected = m_AnimationList->IsSelected();
    if (IsSelected == false)
        return;

    float XDiff = -1, YDiff = -1;
    CSceneResource* Resource              = CSceneManager::GetInst()->GetScene()->GetResource();
    Vector2 FrameStartPos                    = CEditorManager::GetInst()->GetDragObject()->GetStartPos();
    std::string SequenceName               = m_AnimationList->GetSelectItem();
    CAnimationSequence2D* Sequence  = Resource->FindAnimationSequence2D(SequenceName);
    CTexture* SequenceTexture             = Sequence->GetTexture();

    // 220 ? --> 300 - 220  = 80
    // AnimationList ���� ������ Sequence2D�� Name���� Texture ������ ���´�
    Vector2 SequenceImageSize = Vector2(SequenceTexture->GetWidth(), SequenceTexture->GetHeight());

    XDiff = SequenceImageSize.x - FrameStartPos.x;
    YDiff = SequenceImageSize.y - FrameStartPos.y;

    // ���� ���� 
    FrameStartPos.x = XDiff > 0 ? FrameStartPos.x : m_SpriteObject->GetWorldScale().x;
    FrameStartPos.y = YDiff > 0 ? m_SpriteObject->GetWorldScale().y - FrameStartPos.y : 0;

    Vector2 FrameEndPos = CEditorManager::GetInst()->GetDragObject()->GetEndPos();
    XDiff = SequenceImageSize.x - FrameEndPos.x;
    YDiff = SequenceImageSize.y - FrameEndPos.y;
    FrameEndPos.x = XDiff > 0 ? FrameEndPos.x : m_SpriteObject->GetWorldScale().x;
    FrameEndPos.y = YDiff > 0 ? m_SpriteObject->GetWorldScale().y - FrameEndPos.y : 0;

    Vector2 StartPos;
    Vector2 EndPos;

    // �� ���� �� ���� 
    StartPos.x = FrameStartPos.x < FrameEndPos.x ? FrameStartPos.x : FrameEndPos.x;
    StartPos.y = FrameStartPos.y < FrameEndPos.y ? FrameStartPos.y : FrameEndPos.y;

    // �� ū �� ���� 
    EndPos.x = FrameStartPos.x > FrameEndPos.x ? FrameStartPos.x : FrameEndPos.x;
    EndPos.y = FrameStartPos.y > FrameEndPos.y ? FrameStartPos.y : FrameEndPos.y;

    // �ش� FramePos ������ Animation Frame ���� �־��ֱ� 
    // Animation Sequence 2D ����� --> Sprite Edit Object�� �ҷ����� Texture�� 
    Sequence->AddFrame(StartPos, EndPos - StartPos);

    // Frame List Box�� �־��ֱ� 
    char FrameName[1024] = {};
    int   AnimItemCount = m_AnimationFrameList->GetItemCount();
    sprintf_s(FrameName, "%d", AnimItemCount);
    m_AnimationFrameList->AddItem(FrameName);

    // Sampled�� Image �������ֱ� 
    CSpriteComponent* SpriteObjectComponent = dynamic_cast<CSpriteComponent*>(m_SpriteObject->GetRootComponent());
    m_SpriteSampled->SetTexture(SpriteObjectComponent->GetTextureName());
    
    // Image, End ���� 
    Vector2 SpriteSize = m_SpriteSampled->GetSize();
    m_SpriteSampled->SetImageStart(StartPos);
    m_SpriteSampled->SetImageEnd(EndPos);

    // AnimationFrameListBox ���� SelectIdx ������ �ٲ��ֱ� 
    m_AnimationFrameList->SetSelectIndex(AnimItemCount);

}


void CSpriteWindow::SelectAnimationSequence(int Index, const char* TextureName)
{
    // �ش� idx�� Sequence ���� ��������
    CSceneResource* Resource                                 = CSceneManager::GetInst()->GetScene()->GetResource();
    std::string ChangedSequenceName                     = m_AnimationList->GetItem(Index);
    CAnimationSequence2D* ChangedSequence         = Resource->FindAnimationSequence2D(ChangedSequenceName);
    CTexture* ChangedSequenceTexture                    = ChangedSequence->GetTexture();
    Vector2 FrameStartPos                                       = CEditorManager::GetInst()->GetDragObject()->GetStartPos();
    int MaterialTextureIdx = 0;

    // SpriteObject �� Texture �ٲ��ֱ�
    m_SpriteObject->GetSpriteComponent()->SetTexture(MaterialTextureIdx,ChangedSequenceTexture);
    m_SpriteObject->SetWorldScale(ChangedSequenceTexture->GetWidth(), ChangedSequenceTexture->GetHeight(), 1.f);

    // m_Sprite�� Texture �ٲ��ֱ� 
    m_Sprite->SetTexture(ChangedSequenceTexture);
    
    // AnimationFrameList �ȿ� �ִ� ���� ��� �ٽ� �����ϱ� 
    m_AnimationFrameList->Clear();

    // �ش� Sequence�� ���ؼ� �߰��س��� Frame������ ���ٸ� 
    if (ChangedSequence->GetFrameCount() <= 0)
        return;

    // ���ο� Frame ������ �߰��ϱ� 
    char IndexChar[1024];
    for (int i = 0; i < ChangedSequence->GetFrameCount(); i++)
    { 
        sprintf_s(IndexChar, "%d", i);
        m_AnimationFrameList->AddItem(IndexChar);
    }

    AnimationFrameData       FrameData                   = ChangedSequence->GetFrameData(0);
   m_AnimationFrameList->SetSelectIndex(0);

    // Texture ���� 
    m_SpriteSampled->SetTexture(ChangedSequenceTexture);
    m_SpriteSampled->SetImageStart(FrameData.Start);
    m_SpriteSampled->SetImageEnd(FrameData.Start + FrameData.Size);
}

void CSpriteWindow::SelectAnimationFrame(int Index, const char* Name)
{
    CSceneResource* Resource               = CSceneManager::GetInst()->GetScene()->GetResource();
    std::string SequenceName                = m_AnimationList->GetSelectItem();
    CAnimationSequence2D* Sequence   = Resource->FindAnimationSequence2D(SequenceName);
    CTexture* SequenceTexture              = Sequence->GetTexture();
    AnimationFrameData  FrameData      = Sequence->GetFrameData(Index);
    
    // SpriteSampled�� �ٲ��ֱ� 
    m_SpriteSampled->SetImageStart(FrameData.Start);
    m_SpriteSampled->SetImageEnd(FrameData.Start + FrameData.Size);
}

